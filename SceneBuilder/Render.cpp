#include "Render.h"

void Render::mesh(std::vector<Mesh> meshes, glm::mat4 projection, glm::mat4 view, Shader shader)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        //shader.setMat4("model", glm::mat4(1));
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
