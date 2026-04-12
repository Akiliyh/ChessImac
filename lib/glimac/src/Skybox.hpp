#pragma once

#include <vector>
#include <string>
#include <Program.hpp>
#include <FilePath.hpp>
#include "Cube.hpp"
#include "Image.hpp"
#include <glm.hpp>

namespace glimac {

class Skybox {
public:
    Skybox() = default;

    void init(const FilePath& appPath,
              const std::vector<std::string>& faces);

    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint m_skybox_VAO{};
    GLuint m_skybox_VBO{};
    GLuint m_cubemap_texture{};

    Program m_program;

    GLint m_uView{};
    GLint m_uProjection{};
    GLint m_uSkybox{};

    GLuint loadCubemap(const std::vector<std::string>& faces);
};

}  // namespace glimac