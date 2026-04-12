#include "Skybox.hpp"
#include <cmath>
#include <iostream>
#include <vector>


namespace glimac {

Cube const cube(1.0f, 1.0f, 1.0f);

GLuint Skybox::loadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        auto image = loadImage(faces[i]);

        if (image)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image->getWidth(),
                image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels()
            );
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox::init(const glimac::FilePath& appPath, const std::vector<std::string>& faces)
{
    m_program = loadProgram(
        appPath.dirPath() + "./assets/shaders/skybox.vs.glsl",
        appPath.dirPath() + "./assets/shaders/skybox.fs.glsl"
    );

    m_uView       = glGetUniformLocation(m_program.getGLId(), "view");
    m_uProjection = glGetUniformLocation(m_program.getGLId(), "projection");
    m_uSkybox     = glGetUniformLocation(m_program.getGLId(), "skybox");

    m_cubemap_texture = loadCubemap(faces);

    // create cube
    glGenVertexArrays(1, &m_skybox_VAO);
    glGenBuffers(1, &m_skybox_VBO);

    glBindVertexArray(m_skybox_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skybox_VBO);

    glBufferData(
        GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(ShapeVertex), cube.getDataPointer(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)0);

    glBindVertexArray(0);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    m_program.use();

    glm::mat4 view_no_translation = glm::mat4(glm::mat3(view));

    glUniformMatrix4fv(m_uView, 1, GL_FALSE, glm::value_ptr(view_no_translation));
    glUniformMatrix4fv(m_uProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(m_uSkybox, 0);

    glBindVertexArray(m_skybox_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap_texture);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

} // namespace glimac