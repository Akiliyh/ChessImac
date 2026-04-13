#include <cmath>
#include <vector>
#include <iostream>
#include "common.hpp"
#include "Cube.hpp"

namespace glimac {

void Cube::build(GLfloat height, GLfloat width, GLfloat depth) {

    m_depth = depth;
    m_height = height;
    m_width = width;

    // here we put the uv also in this one, for each vertice we set the uv
    // if u want to reverse the texture just reverse each u and v coord
    GLfloat g_vertex_buffer_data[] = {
        // back
        -width, -height, -depth, 1.f, 1.f,
         width, -height, -depth, 0.f, 1.f,
         width,  height, -depth, 0.f, 0.f,
        -width, -height, -depth, 1.f, 1.f,
         width,  height, -depth, 0.f, 0.f,
        -width,  height, -depth, 1.f, 0.f,

        // front
         width, -height,  depth, 1.f, 1.f,
        -width, -height,  depth, 0.f, 1.f,
        -width,  height,  depth, 0.f, 0.f,
         width, -height,  depth, 1.f, 1.f,
        -width,  height,  depth, 0.f, 0.f,
         width,  height,  depth, 1.f, 0.f,

        // left
        -width, -height,  depth, 1.f, 1.f,
        -width, -height, -depth, 0.f, 1.f,
        -width,  height, -depth, 0.f, 0.f,
        -width, -height,  depth, 1.f, 1.f,
        -width,  height, -depth, 0.f, 0.f,
        -width,  height,  depth, 1.f, 0.f,

        // rigt
         width, -height, -depth, 1.f, 1.f,
         width, -height,  depth, 0.f, 1.f,
         width,  height,  depth, 0.f, 0.f,
         width, -height, -depth, 1.f, 1.f,
         width,  height,  depth, 0.f, 0.f,
         width,  height, -depth, 1.f, 0.f,

        // top
        -width,  height, -depth, 0.f, 0.f,
         width,  height, -depth, 1.f, 0.f,
         width,  height,  depth, 1.f, 1.f,
        -width,  height, -depth, 0.f, 0.f,
         width,  height,  depth, 1.f, 1.f,
        -width,  height,  depth, 0.f, 1.f,

        // bot
        -width, -height,  depth, 0.f, 0.f,
         width, -height,  depth, 1.f, 0.f,
         width, -height, -depth, 1.f, 1.f,
        -width, -height,  depth, 0.f, 0.f,
         width, -height, -depth, 1.f, 1.f,
        -width, -height, -depth, 0.f, 1.f
    };

    //here we set the normals for each vertex

    GLfloat g_normal_buffer_data[] = {
    // back
    0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1,
    // front
    0,0,1,   0,0,1,   0,0,1,   0,0,1,   0,0,1,   0,0,1,
    // left
    -1,0,0,  -1,0,0,  -1,0,0,  -1,0,0,  -1,0,0,  -1,0,0,
    // right
    1,0,0,   1,0,0,   1,0,0,   1,0,0,   1,0,0,   1,0,0,
    // top
    0,1,0,   0,1,0,   0,1,0,   0,1,0,   0,1,0,   0,1,0,
    // bot
    0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,
    };

    // pour chaque vertex de triangle on set une position etc
    for(GLsizei i = 0; i < 12 * 3; ++i) { // 12 triangles x 3 vertices
        ShapeVertex vertex;
        vertex.position.x = g_vertex_buffer_data[i*5 + 0];
        vertex.position.y = g_vertex_buffer_data[i*5 + 1];
        vertex.position.z = g_vertex_buffer_data[i*5 + 2];

        vertex.normal.x = g_normal_buffer_data[i*3 + 0];
        vertex.normal.y = g_normal_buffer_data[i*3 + 1];
        vertex.normal.z = g_normal_buffer_data[i*3 + 2];

        vertex.texCoords.x = g_vertex_buffer_data[i*5 + 3];
        vertex.texCoords.y = g_vertex_buffer_data[i*5 + 4];

        

        m_Vertices.push_back(vertex);
    }

    m_nVertexCount = m_Vertices.size();
}

float Cube::getHeight()
{
    return m_height;
}

float Cube::getWidth()
{
    return m_width;
}

float Cube::getDepth()
{
    return m_depth;
}

}  // namespace glimac