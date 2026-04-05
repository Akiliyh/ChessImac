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
Cube square(0.0125f, 0.125f, 0.125f);

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

    glGenBuffers(1, &boardVbo);
    glBindBuffer(GL_ARRAY_BUFFER, boardVbo);

    const ShapeVertex* vertices    = board.getDataPointer();
    const int          nb_vertices = board.getVertexCount();

    glBufferData(
        GL_ARRAY_BUFFER, board.getVertexCount() * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &boardVao);
    glBindVertexArray(boardVao);

    const GLuint VERTEX_SHADER_POSITION   = 0;
    const GLuint VERTEX_SHADER_NORMAL     = 1;
    const GLuint VERTEX_SHADER_TEX_COORDS = 2;

    glEnableVertexAttribArray(VERTEX_SHADER_POSITION);
    glEnableVertexAttribArray(VERTEX_SHADER_NORMAL);
    glEnableVertexAttribArray(VERTEX_SHADER_TEX_COORDS);

    glBindBuffer(GL_ARRAY_BUFFER, boardVbo);
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
    
    glGenBuffers(1, &squareVbo);
    glBindBuffer(GL_ARRAY_BUFFER, squareVbo);

    glBufferData(
        GL_ARRAY_BUFFER, square.getVertexCount() * sizeof(ShapeVertex), square.getDataPointer(), GL_STATIC_DRAW
    );
    
    glGenVertexArrays(1, &squareVao);
    glBindVertexArray(squareVao);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)offsetof(ShapeVertex, texCoords));
    
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
    glUniform1i(chessProgram->uUseTexture, 1);

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

    glBindVertexArray(boardVao);
    glUniform3f(chessProgram->uColor, 0.6f, 0.4f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, board.getVertexCount());

    glBindVertexArray(0);

    float square_width = square.getWidth();
    float square_height = square.getHeight();
    float square_depth = square.getDepth();

    float board_width = board.getWidth();

    glm::mat4 squareMVMatrix = globalMVMatrix; // Translation
    squareMVMatrix           = glm::translate(
            squareMVMatrix, glm::vec3(
                                -1, square_height*5,
                                -1 + (board_width/8.0f)
                            )
        );

    for (size_t i = 0; i < 8; i++)
    {
        
        squareMVMatrix           = glm::translate(
            squareMVMatrix, glm::vec3(
                                (i != 0) ? square_width*2 : board_width/8.0, 0,
                                0
                            )
        );

        glm::mat4 squareMVPMatrix    = ProjMatrix * squareMVMatrix;
        glm::mat3 squareNormalMatrix = glm::transpose(glm::inverse(glm::mat3(squareMVMatrix)));

        glUniformMatrix4fv(chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(squareMVPMatrix));
        glUniformMatrix4fv(chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(squareMVMatrix));
        glUniformMatrix3fv(
            chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(squareNormalMatrix)
        );
        glBindVertexArray(squareVao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);

        glUniform3f(chessProgram->uColor, 0.1f, 0.6f, 0.8f);
        glUniform1i(chessProgram->uUseTexture, 1);

        glDrawArrays(GL_TRIANGLES, 0, square.getVertexCount());

        // deal with pawn here
        // for now all squares has a pawn on it
        
        glm::mat4 pieceMVMatrix = glm::translate(squareMVMatrix, glm::vec3(square_width * 0.15f, square_height, 0.0f));
        pieceMVMatrix = glm::scale(pieceMVMatrix, glm::vec3(0.075, 0.075, 0.075));
        
        glm::mat4 pieceMVPMatrix    = ProjMatrix * pieceMVMatrix;
        glm::mat4 pieceNormalMatrix = glm::transpose(glm::inverse(glm::mat3(pieceMVMatrix)));
        
        glUniformMatrix4fv(chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVPMatrix));
        glUniformMatrix4fv(chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVMatrix));
        glUniformMatrix3fv(
            chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(pieceNormalMatrix)
        );

        glUniform1i(chessProgram->uUseTexture, 0); // we only color for now as textures dont work yet
        glUniform3f(chessProgram->uColor, 1.0f, 0.0f, 0.0f);

        pawnOBJ.draw(); // for now it has no texture cause no texcoords or all 0,0, to fix later
    }


    return 0;
}

void Renderer_3D::terminate()
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &squareVbo);
    glDeleteVertexArrays(1, &squareVao);
    glDeleteBuffers(1, &boardVbo);
    glDeleteVertexArrays(1, &boardVao);
}