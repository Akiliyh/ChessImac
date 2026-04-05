#include "3DRenderer.hpp"
#include <cstddef>
#include <vector>
#include "GameManager.hpp"
#include "Image.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"

#define GLFW_INCLUDE_NONE
#include "common.hpp"
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include <Cube.hpp>
#include <FilePath.hpp>
#include <Program.hpp>
#include <Sphere.hpp>
#include <getTime.hpp>
#include <glm.hpp>


using namespace glimac;

Cube board(0.05f, 1.125f, 1.125f);
Cube square(0.05f, 0.125f, 0.125f);

int Renderer_3D::init(int width, int height)
{
    this->height = height;
    this->width  = width;

    glViewport(0, 0, width, height);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // deal with obj here

    pawnOBJ.load("./assets/models/pawn.obj");

    auto woodImage = glimac::loadImage("./assets/textures/BoardWood.jpg");

    glGenTextures(1, &woodTexture);

    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, woodImage->getWidth(), woodImage->getHeight(), 0, GL_RGBA,
        GL_FLOAT, woodImage->getPixels()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FilePath applicationPath("color2D.vs.glsl");
    this->chessProgram = std::make_unique<ChessProgram>(applicationPath);

    ProjMatrix             = glm::perspective(glm::radians(70.f), 1.0f, 0.1f, 100.f);
    glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 MVPMatrix    = ProjMatrix * MVMatrix;

    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const ShapeVertex* vertices    = board.getDataPointer();
    const int          nb_vertices = board.getVertexCount();

    glBufferData(
        GL_ARRAY_BUFFER, board.getVertexCount() * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_SHADER_POSITION   = 0;
    const GLuint VERTEX_SHADER_NORMAL     = 1;
    const GLuint VERTEX_SHADER_TEX_COORDS = 2;

    glEnableVertexAttribArray(VERTEX_SHADER_POSITION);
    glEnableVertexAttribArray(VERTEX_SHADER_NORMAL);
    glEnableVertexAttribArray(VERTEX_SHADER_TEX_COORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
        VERTEX_SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)0
    );
    glVertexAttribPointer(
        VERTEX_SHADER_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, normal)
    );
    glVertexAttribPointer(
        VERTEX_SHADER_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, texCoords)
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return 0;
}

int Renderer_3D::draw(int width, int height, GameManager& game)
{
    chessProgram->m_Program.use();
    glViewport(0, 0, width, height);

    /* Loop until the user closes the window */
    // while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/

    glUniform1i(chessProgram->uBoardTexture, 0);

    glm::mat4 globalMVMatrix = camera.getViewMatrix();

    glm::mat4 chessMVMatrix = globalMVMatrix;
    glUniformMatrix4fv(chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(chessMVMatrix));
    glUniformMatrix4fv(
        chessProgram->uNormalMatrix, 1, GL_FALSE,
        glm::value_ptr(glm::transpose(glm::inverse(chessMVMatrix)))
    );
    glUniformMatrix4fv(
        chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * chessMVMatrix)
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);

    glBindVertexArray(vao);
    glUniform3f(chessProgram->uColor, 0.6f, 0.4f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, board.getVertexCount());

    glBindVertexArray(0);

    glm::mat4 moonMVMatrix = globalMVMatrix; // Translation
    moonMVMatrix           = glm::translate(
        moonMVMatrix, glm::vec3(
                          1, 0.05,
                          0
                      )
    ); // Translation * Rotation * Rotation * Translation
    moonMVMatrix = glm::scale(
        moonMVMatrix, glm::vec3(
                          0.125, 0.125,
                          0.125
                      )
    ); // Translation * Rotation * Rotation * Translation * Scale

    glm::mat4 moonMVPMatrix    = ProjMatrix * moonMVMatrix;
    glm::mat3 moonNormalMatrix = glm::transpose(glm::inverse(glm::mat3(moonMVMatrix)));

    glUniformMatrix4fv(chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(moonMVPMatrix));
    glUniformMatrix4fv(chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
    glUniformMatrix3fv(chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(moonNormalMatrix));
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);

    glUniform3f(chessProgram->uColor, 0.1f, 0.6f, 0.8f);

    glDrawArrays(GL_TRIANGLES, 0, square.getVertexCount());

    glUniform3f(chessProgram->uColor, 1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, woodTexture);
glUniform1i(chessProgram->uBoardTexture, 0);

    pawnOBJ.draw(); // for now it has no texture cause no texcoords or all 0,0, to fix later

    // setup attributes like before
    glBindVertexArray(0);

    return 0;
}

void Renderer_3D::terminate()
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}