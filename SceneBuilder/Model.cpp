#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::loadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP Could not load model: " << importer.GetErrorString() << std::endl;
    }
    else {
        this->directory = filename.substr(0, filename.find_last_of('/'));

        this->processNode(scene->mRootNode, scene);
    }
}

void Model::draw(glm::mat4& projection, glm::mat4& view, Shader& shader, STModel& structModel, glm::vec3 lightPos)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, structModel.position); // translate it down so it's at the center of the scene
    model = glm::scale(model, structModel.scale);	// it's a bit too big for our scene, so scale it down

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    shader.setVec3("lightPos", lightPos);


    for (unsigned int i = 0; i < meshes.size(); i++) {
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

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices = this->vertices(mesh);
    std::vector<unsigned int> indices = this->indices(mesh);
    std::vector<Texture> textures = this->textures(mesh, scene);

    return Mesh(
        vertices,
        indices,
        textures
    );
}

std::vector<Vertex> Model::vertices(aiMesh* mesh)
{
    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        glm::vec3 vector3;

        // Vertices
        vector3.x = mesh->mVertices[i].x;
        vector3.y = mesh->mVertices[i].y;
        vector3.z = mesh->mVertices[i].z;
        vertex.position = vector3;

        // Normals
        vector3.x = mesh->mNormals[i].x;
        vector3.y = mesh->mNormals[i].y;
        vector3.z = mesh->mNormals[i].z;
        vertex.normal = vector3;

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

        // Tangent
        vector3.x = mesh->mTangents[i].x;
        vector3.y = mesh->mTangents[i].y;
        vector3.z = mesh->mTangents[i].z;
        vertex.tangent = vector3;

        // Bitangent
        vector3.x = mesh->mBitangents[i].x;
        vector3.y = mesh->mBitangents[i].y;
        vector3.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector3;

        vertices.push_back(vertex);
    }

    return vertices;
}

std::vector<unsigned int> Model::indices(aiMesh* mesh)
{
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
         aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
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
