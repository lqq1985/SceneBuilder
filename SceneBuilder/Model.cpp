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

void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation)
{ 
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        // only apply transformation on meshs not entities such as lights or camera.
        transformation *= node->mTransformation;

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
    glm::mat4 transform = this->aiMatrix4x4ToGlm(transformation);
    glm::vec3 origin = transform[3];

    std::vector<Vertex> vertices = this->getVertices(mesh, extents, transform);
    std::vector<unsigned int> indices = this->getIndices(mesh);
    std::vector<Texture> textures = this->getTexures(mesh, scene);

    std::string name = mesh->mName.C_Str();

    return Mesh(
        vertices,
        indices,
        textures,
        extents,
        origin,
        name,
        this->aiMatrix4x4ToGlm(transformation)
    );
}

std::vector<Vertex> Model::getVertices(aiMesh* mesh, glm::vec3& extents, glm::mat4& transform)
{
    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Convert vertices from model space to world space
        glm::vec3 vector3 = glm::vec3(transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f));

        vertex.position = vector3;

        // Normals
        if (mesh->mNormals) {
                
            // Convert normals from model space to world space
            vector3 = glm::vec3(transform * glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0.0f));
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

    glm::vec3 min = glm::vec3(transform * glm::vec4(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z, 1.0f));
    glm::vec3 max = glm::vec3(transform * glm::vec4(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z, 1.0f));

    extents = (max - min) * 0.5f;

    printf("extents: %f, %f, %f\n", extents.x, extents.y, abs(extents.z));

    return vertices;
}

std::vector<unsigned int> Model::getIndices(aiMesh* mesh)
{
    std::vector<unsigned int> indices;

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

std::vector<Texture> Model::getTexures(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Texture> textures;

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadTextureFiles(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadTextureFiles(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadTextureFiles(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadTextureFiles(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return textures;
}

std::vector<Texture> Model::loadTextureFiles(aiMaterial* mat, aiTextureType type, std::string typeName)
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

void Model::draw(glm::mat4& projection, glm::mat4& view, Shader& shader)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        shader.setMat4("model", meshes[i].mTransform);

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

glm::mat4 Model::aiMatrix4x4ToGlm(aiMatrix4x4 from)
{
    glm::mat4 to;

    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}
