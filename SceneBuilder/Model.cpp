#include "Model.h"

// https://gamedev.stackexchange.com/questions/82833/problem-loading-collada-dae-model-using-assimp-in-oepngl-4-4

Model::Model(std::string filename)
{
    this->loadModel(filename);
}

Model::~Model()
{
}

void Model::loadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

    if (!scene || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP Could not load model: " << importer.GetErrorString() << std::endl;
    }
    else {
        this->directory = filename.substr(0, filename.find_last_of('/'));

        this->processNode(scene->mRootNode, scene, aiMatrix4x4());
    }
}

void Model::draw(glm::mat4& projection, glm::mat4& view, Shader& shader)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        // if (i == 0) continue;

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, meshes[i].origin); // translate it down so it's at the center of the scene

        shader.setMat4("model", model);

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int j = 0; j < meshes[i].textures.size(); j++)
        {
            glActiveTexture(GL_TEXTURE0 + j); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = meshes[i].textures[j].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), j);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
        }

        // draw mesh
        glBindVertexArray(meshes[i].VAO);
        glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation)
{
    transformation *= node->mTransformation;

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene, transformation));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, transformation);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation)
{
    glm::vec3 extents;
    glm::vec3 origin;

    std::vector<Vertex> vertices = this->vertices(mesh, extents, origin, transformation);
    std::vector<unsigned int> indices = this->indices(mesh);
    std::vector<Texture> textures = this->textures(mesh, scene);

    return Mesh(
        vertices,
        indices,
        textures,
        extents,
        origin,
        mesh->mName
    );
}

std::vector<Vertex> Model::vertices(aiMesh* mesh, glm::vec3& extents, glm::vec3 &origin, aiMatrix4x4 transformation)
{
    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        glm::vec3 vector3;

        aiVector3D v = transformation * mesh->mVertices[i];

        // Vertices
        vector3.x = v.x;
        vector3.y = v.y;
        vector3.z = v.z;

        vertex.position = vector3;

        // Normals
        if (mesh->mNormals) {
            vector3.x = mesh->mNormals[i].x;
            vector3.y = mesh->mNormals[i].y;
            vector3.z = mesh->mNormals[i].z;
            vertex.normal = vector3;
        }


        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vector2;

            vector2.x = mesh->mTextureCoords[0][i].x;
            vector2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vector2;
        }
        else {
            vertex.texCoord = glm::vec2(0, 0);
        }

        if (mesh->mTangents) {
            vector3.x = mesh->mTangents[i].x;
            vector3.y = mesh->mTangents[i].y;
            vector3.z = mesh->mTangents[i].z;
            vertex.tangent = vector3;
        }

        // Bitangent
        if (mesh->mBitangents) {
            vector3.x = mesh->mBitangents[i].x;
            vector3.y = mesh->mBitangents[i].y;
            vector3.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector3;
        }


        vertices.push_back(vertex);
    }

    glm::vec3 min = glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z);
    glm::vec3 max = glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z);

    extents = (max - min) * 0.5f;
    origin = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

    printf("%f,%f,%f\n", origin.x, origin.y, origin.z);

    // box blender: x = 3, y = -3, z = 6.4697
    // opengl x = 3, y = 6.4697, z = -3
    // 3,6.4,3

    return vertices;
}

std::vector<unsigned int> Model::indices(aiMesh* mesh)
{
    std::vector<unsigned int> indices;

    aiMatrix4x4 m = aiMatrix4x4();

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
         aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
         for (unsigned int j = 0; j < face.mNumIndices; j++) {
             indices.push_back(face.mIndices[j]);
         }

    }

    return indices;
}

std::vector<Texture> Model::textures(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Texture> textures;

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return textures;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            unsigned int id;

            std::string filename = std::string(str.C_Str());
            std::string fullpath = this->directory + '/' + filename;

            if (!OglGenTexture::bind2dLinear(fullpath.c_str(), id)) {
                std::cout << "Could not load texture" << std::endl;
            }

            Texture texture;
            texture.id = id;
            texture.type = typeName;
            texture.path = fullpath;
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

glm::mat4 Model::aiMatrix4x4ToGlm(aiMatrix4x4 from)
{
    glm::mat4 to;


    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}
