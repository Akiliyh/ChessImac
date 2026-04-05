#include "OBJModel.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "common.hpp"
#include "tiny_obj_loader.h"


namespace glimac {

OBJModel::~OBJModel()
{
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
}

bool OBJModel::load(const std::string& path)
{
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err = tinyobj::LoadObj(shapes, materials, path.c_str());
    if (!err.empty())
    {
        std::cerr << "TinyObjLoader error: " << err << std::endl;
        return false;
    }

    std::vector<ShapeVertex> vertices;

    for (const auto& shape : shapes)
    {

        const std::vector<float>& pos = shape.mesh.positions;
        const std::vector<float>& nor = shape.mesh.normals;
        const std::vector<float>& tex = shape.mesh.texcoords;

        for (size_t i = 0; i < shape.mesh.indices.size(); i++)
        {

            unsigned int idx = shape.mesh.indices[i];
            ShapeVertex  v;

            v.position = glm::vec3(pos[3 * idx + 0], pos[3 * idx + 1], pos[3 * idx + 2]);

            if (!nor.empty())
            {
                v.normal = glm::vec3(nor[3 * idx + 0], nor[3 * idx + 1], nor[3 * idx + 2]);
            }
            else
            {
                v.normal = glm::vec3(0.f, 1.f, 0.f);
            }

            // tex seems empty here, look on how the tinyobj gets its texcoords

            if (!tex.empty())
            {
                v.texCoords = glm::vec2(tex[2 * idx + 0], tex[2 * idx + 1]);
            }
            else
            {
                v.texCoords = glm::vec2(0.f, 0.f);
            }

            vertices.push_back(v);
        }
    }

    vertexCount = vertices.size();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, vertices.size() * sizeof(ShapeVertex), vertices.data(), GL_STATIC_DRAW
    );

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint POSITION_ATTR = 0;
    const GLuint NORMAL_ATTR   = 1;
    const GLuint TEXCOORD_ATTR = 2;

    glEnableVertexAttribArray(POSITION_ATTR);
    glEnableVertexAttribArray(NORMAL_ATTR);
    glEnableVertexAttribArray(TEXCOORD_ATTR);

    glVertexAttribPointer(
        POSITION_ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const void*)0
    );
    glVertexAttribPointer(
        NORMAL_ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const void*)offsetof(ShapeVertex, normal)
    );
    glVertexAttribPointer(
        TEXCOORD_ATTR, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const void*)offsetof(ShapeVertex, texCoords)
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void OBJModel::draw() const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

} // namespace glimac