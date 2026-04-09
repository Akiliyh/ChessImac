#include <cmath>
#include <vector>
#include "Cylinder.hpp"

namespace glimac {

void Cylinder::build(GLfloat radius, GLfloat height, GLsizei discLat, GLsizei discLong) {

    GLfloat rcpLat  = 1.f / discLat;
    GLfloat rcpLong = 1.f / discLong;

    GLfloat dPhi    = 2 * glm::pi<float>() * rcpLat;
    GLfloat dHeight = height * rcpLong;

    std::vector<ShapeVertex> data;

    for(GLsizei j = 0; j <= discLong; ++j) {
        GLfloat y = -height / 2.f + j * dHeight;

        for(GLsizei i = 0; i <= discLat; ++i) {
            GLfloat phi = i * dPhi;

            ShapeVertex vertex;

            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = j * rcpLong;

            vertex.normal.x = cos(phi);
            vertex.normal.y = 0.f;
            vertex.normal.z = sin(phi);

            vertex.position.x = radius * vertex.normal.x;
            vertex.position.y = y;
            vertex.position.z = radius * vertex.normal.z;

            data.push_back(vertex);
        }
    }

    // Build side triangles
    for(GLsizei j = 0; j < discLong; ++j) {
        GLsizei offset = j * (discLat + 1);

        for(GLsizei i = 0; i < discLat; ++i) {
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + i + 1]);
            m_Vertices.push_back(data[offset + discLat + 1 + i + 1]);

            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + discLat + 1 + i + 1]);
            m_Vertices.push_back(data[offset + discLat + 1 + i]);
        }
    }

    GLfloat yTop = height / 2.f;

    ShapeVertex topCenter;
    topCenter.position = glm::vec3(0.f, yTop, 0.f);
    topCenter.normal   = glm::vec3(0.f, 1.f, 0.f);
    topCenter.texCoords = glm::vec2(0.5f, 0.5f);

    std::vector<ShapeVertex> topRing;

    for(GLsizei i = 0; i <= discLat; ++i) {
        GLfloat phi = i * dPhi;

        ShapeVertex v;
        v.position = glm::vec3(radius * cos(phi), yTop, radius * sin(phi));
        v.normal   = glm::vec3(0.f, 1.f, 0.f);
        v.texCoords = glm::vec2(0.5f + 0.5f * cos(phi),
                               0.5f + 0.5f * sin(phi));

        topRing.push_back(v);
    }

    for(GLsizei i = 0; i < discLat; ++i) {
        m_Vertices.push_back(topCenter);
        m_Vertices.push_back(topRing[i]);
        m_Vertices.push_back(topRing[i + 1]);
    }

    GLfloat yBottom = -height / 2.f;

    ShapeVertex bottomCenter;
    bottomCenter.position = glm::vec3(0.f, yBottom, 0.f);
    bottomCenter.normal   = glm::vec3(0.f, -1.f, 0.f);
    bottomCenter.texCoords = glm::vec2(0.5f, 0.5f);

    std::vector<ShapeVertex> bottomRing;

    for(GLsizei i = 0; i <= discLat; ++i) {
        GLfloat phi = i * dPhi;

        ShapeVertex v;
        v.position = glm::vec3(radius * cos(phi), yBottom, radius * sin(phi));
        v.normal   = glm::vec3(0.f, -1.f, 0.f);
        v.texCoords = glm::vec2(0.5f + 0.5f * cos(phi),
                               0.5f + 0.5f * sin(phi));

        bottomRing.push_back(v);
    }

    for(GLsizei i = 0; i < discLat; ++i) {
        m_Vertices.push_back(bottomCenter);
        m_Vertices.push_back(bottomRing[i + 1]);
        m_Vertices.push_back(bottomRing[i]);
    }

    m_nVertexCount =
        discLat * discLong * 6   // sides
        + discLat * 3            // top
        + discLat * 3;           // bottom
}

}  // namespace glimac