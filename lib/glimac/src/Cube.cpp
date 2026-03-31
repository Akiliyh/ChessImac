#include <cmath>
#include <vector>
#include <iostream>
#include "common.hpp"
#include "Cube.hpp"

namespace glimac {

void Cube::build(GLfloat height, GLfloat width, GLfloat depth) {

    GLfloat g_vertex_buffer_data[] = {
    -width,-height,-depth, // triangle 1 : begin
    -width,-height, depth,
    -width, height, depth, // triangle 1 : end
    width, height,-depth, // triangle 2 : begin
    -width,-height,-depth,
    -width, height,-depth, // triangle 2 : end
    width,-height, depth,
    -width,-height,-depth,
    width,-height,-depth,
    width, height,-depth,
    width,-height,-depth,
    -width,-height,-depth,
    -width,-height,-depth,
    -width, height, depth,
    -width, height,-depth,
    width,-height, depth,
    -width,-height, depth,
    -width,-height,-depth,
    -width, height, depth,
    -width,-height, depth,
    width,-height, depth,
    width, height, depth,
    width,-height,-depth,
    width, height,-depth,
    width,-height,-depth,
    width, height, depth,
    width,-height, depth,
    width, height, depth,
    width, height,-depth,
    -width, height,-depth,
    width, height, depth,
    -width, height,-depth,
    -width, height, depth,
    width, height, depth,
    -width, height, depth,
    width,-height, depth
};

    // pour chaque vertex de triangle on set une position etc
    for(GLsizei i = 0; i < 12 * 3; ++i) {
        ShapeVertex vertex;
        vertex.position.x = g_vertex_buffer_data[i*3 + 0];
        vertex.position.y = g_vertex_buffer_data[i*3 + 1];
        vertex.position.z = g_vertex_buffer_data[i*3 + 2];
        vertex.normal.x = vertex.position.x;
        vertex.normal.y = vertex.position.y;
        vertex.normal.z = vertex.position.z;
        vertex.texCoords.x = vertex.position.x;
        vertex.texCoords.y = vertex.position.y;
        m_Vertices.push_back(vertex);
    }

    m_nVertexCount = m_Vertices.size();

}

}


