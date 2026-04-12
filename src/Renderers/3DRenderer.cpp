#include "3DRenderer.hpp"
#include <cstddef>
#include <vector>
#include "Cylinder.hpp"
#include "GameManager.hpp"
#include "Image.hpp"
#include "Pieces.hpp"
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
#include <Cylinder.hpp>
#include <FilePath.hpp>
#include <Program.hpp>
#include <Sphere.hpp>
#include <getTime.hpp>
#include <glm.hpp>

using namespace glimac;

Cube     board(0.05f, 1.125f, 1.125f);
Cube     square(0.0125f, 0.125f, 0.125f);
Cylinder cylinder(0.1f, 0.02f, 20, 20);

const float texture_clipping_delta{0.001f};

float skyboxVertices[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                          1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                          -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                          -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                          1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                          -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                          -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                          -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                          1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

void Renderer_3D::initVertexObject(const ShapeVertex* data, size_t count, GLuint& vbo, GLuint& vao)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, count * sizeof(ShapeVertex), data, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)0);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal)
    );
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const GLvoid*)offsetof(ShapeVertex, texCoords)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint loadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        auto image = glimac::loadImage(faces[i]);

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

int Renderer_3D::init(int width, int height)
{
    this->height = height;
    this->width  = width;

    glViewport(0, 0, width, height);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // deal with obj here

    pawnOBJ.loadOBJ("./assets/models/pawn_body.obj", "./assets/models/pawn_body.mtl", true);
    bishopOBJ.loadOBJ("./assets/models/bishop_body.obj", "./assets/models/bishop_body.mtl", true);
    kingOBJ.loadOBJ("./assets/models/king_body.obj", "./assets/models/king_body.mtl", true);
    queenOBJ.loadOBJ("./assets/models/queen_body.obj", "./assets/models/queen_body.mtl", true);
    rookOBJ.loadOBJ("./assets/models/rook_body.obj", "./assets/models/rook_body.mtl", true);
    knightOBJ.loadOBJ("./assets/models/knight_body.obj", "./assets/models/knight_body.mtl", true);

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

    ProjMatrix             = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.f);
    glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 MVPMatrix    = ProjMatrix * MVMatrix;

    glEnable(GL_DEPTH_TEST);

    initVertexObject(board.getDataPointer(), board.getVertexCount(), boardVbo, boardVao);
    initVertexObject(square.getDataPointer(), square.getVertexCount(), squareVbo, squareVao);
    initVertexObject(
        cylinder.getDataPointer(), cylinder.getVertexCount(), cylinderVbo, cylinderVao
    );

    this->skyboxProgram = std::make_unique<SkyboxProgram>(applicationPath);

    std::vector<std::string> faces = {
        "./assets/textures/skybox/right.jpg", "./assets/textures/skybox/left.jpg",
        "./assets/textures/skybox/top.jpg",   "./assets/textures/skybox/bottom.jpg",
        "./assets/textures/skybox/front.jpg", "./assets/textures/skybox/back.jpg"
    };

    cubemapTexture = loadCubemap(faces);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    return 0;
}

int Renderer_3D::draw(int width, int height, GameManager& game)
{
    ProjMatrix = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.f);
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

    float const square_width  = square.getWidth();
    float const square_height = square.getHeight();
    float const square_depth  = square.getDepth();

    float const board_width = board.getWidth();

    int       piece_position{};
    int const game_board_size = game.board.get_size();

    Piece* current_square = nullptr;

    glm::mat4 squareMVMatrix = globalMVMatrix; // Translation
    squareMVMatrix =
        glm::translate(squareMVMatrix, glm::vec3(-1, square_height * 5, -1 + (board_width / 8.0f)));

    glm::mat4 baseSquareMVMatrix = squareMVMatrix;

    for (size_t row = 0; row < 8; row++)
    {
        // at the start of the loop we wanna go back to the left of the board
        squareMVMatrix = baseSquareMVMatrix;

        squareMVMatrix = glm::translate(squareMVMatrix, glm::vec3(0, 0, (square_width * 2) * row));

        for (size_t col = 0; col < 8; col++)
        {
            piece_position                         = col + (row * game_board_size);
            const std::vector<int>& possible_moves = game.get_possible_moves();

            squareMVMatrix = glm::translate(
                squareMVMatrix, glm::vec3((col != 0) ? square_width * 2 : board_width / 8.0, 0, 0)
            );

            glm::mat4 squareMVPMatrix    = ProjMatrix * squareMVMatrix;
            glm::mat3 squareNormalMatrix = glm::transpose(glm::inverse(glm::mat3(squareMVMatrix)));

            glUniformMatrix4fv(
                chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(squareMVPMatrix)
            );
            glUniformMatrix4fv(
                chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(squareMVMatrix)
            );
            glUniformMatrix3fv(
                chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(squareNormalMatrix)
            );
            glBindVertexArray(squareVao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, woodTexture);

            bool isDark = (row + col) % 2 == 1;

            isDark ? glUniform3f(chessProgram->uColor, 0.f, 0.f, 0.f)
                   : glUniform3f(chessProgram->uColor, 1.f, 1.f, 1.f);

            glUniformMatrix4fv(
                chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(squareMVPMatrix)
            );
            glUniformMatrix4fv(
                chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(squareMVMatrix)
            );
            glUniformMatrix3fv(
                chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(squareNormalMatrix)
            );

            glUniform1i(chessProgram->uUseTexture, 1);

            glBindVertexArray(squareVao);
            glDrawArrays(GL_TRIANGLES, 0, square.getVertexCount());

            // we wanna display possible moves

            glUniform1i(chessProgram->uUseTexture, 0);
            bool is_possible_square = false;

            for (const int& move : possible_moves)
            {
                if (move == piece_position)
                {
                    glUniform3f(chessProgram->uColor, 0.5f, 0.5f, 1.f);
                    is_possible_square = true;
                    break;
                }
            }

            if (!is_possible_square)
            {
                glUniform1i(chessProgram->uUseTexture, 1);
            }
            else
            {
                glBindVertexArray(cylinderVao);
                glm::mat4 possible_moveMVMatrix = squareMVMatrix;

                possible_moveMVMatrix = glm::translate(
                    squareMVMatrix, glm::vec3(0, square_height + texture_clipping_delta, 0)
                );

                glm::mat4 possible_moveMVPMatrix = ProjMatrix * possible_moveMVMatrix;
                glm::mat4 possible_moveNormalMatrix =
                    glm::transpose(glm::inverse(glm::mat3(possible_moveMVMatrix)));

                glUniformMatrix4fv(
                    chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(possible_moveMVPMatrix)
                );
                glUniformMatrix4fv(
                    chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(possible_moveMVMatrix)
                );
                glUniformMatrix3fv(
                    chessProgram->uNormalMatrix, 1, GL_FALSE,
                    glm::value_ptr(possible_moveNormalMatrix)
                );

                glDrawArrays(GL_TRIANGLES, 0, cylinder.getVertexCount());
            }

            // deal with pawn here

            // we want to display a piece only if it exists in the game board

            current_square = game.board.get_board_data(piece_position).get();

            if (current_square)
            {
                glm::mat4 pieceMVMatrix = glm::translate(
                    squareMVMatrix,
                    glm::vec3(
                        0,
                        square_height + texture_clipping_delta, // to avoid texture clipping
                        0.0f
                    )
                );
                pieceMVMatrix = glm::scale(pieceMVMatrix, glm::vec3(0.0625, 0.0625, 0.0625));

                (current_square->get_color() == White)
                    ? pieceMVMatrix = glm::rotate(
                          pieceMVMatrix, 3.f * glm::pi<float>() * .5f, glm::vec3(0, 1, 0)
                      )
                    : pieceMVMatrix =
                          glm::rotate(pieceMVMatrix, glm::pi<float>() * .5f, glm::vec3(0, 1, 0));

                glm::mat4 pieceMVPMatrix = ProjMatrix * pieceMVMatrix;
                glm::mat4 pieceNormalMatrix =
                    glm::transpose(glm::inverse(glm::mat3(pieceMVMatrix)));

                glUniformMatrix4fv(
                    chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVPMatrix)
                );
                glUniformMatrix4fv(
                    chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVMatrix)
                );
                glUniformMatrix3fv(
                    chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(pieceNormalMatrix)
                );

                glUniform1i(chessProgram->uUseTexture, 0);

                (current_square->get_color() == White)
                    ? glUniform3f(chessProgram->uColor, 1.0f, 1.0f, 1.0f)
                    : glUniform3f(chessProgram->uColor, 0.2f, 0.1f, 0.1f);

                // here we choose which model to draw

                switch (std::tolower(current_square->get_label()))
                {
                case 'b': bishopOBJ.draw(); break;

                case 'k': kingOBJ.draw(); break;

                case 'q': queenOBJ.draw(); break;

                case 'r': rookOBJ.draw(); break;

                case 'n': knightOBJ.draw(); break;

                default: pawnOBJ.draw();
                }
            }
        }
    }

    glDepthFunc(GL_LEQUAL); // important

    skyboxProgram->m_Program.use();

    // remove translation from camera
    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));

    glUniformMatrix4fv(skyboxProgram->uView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(skyboxProgram->uProjection, 1, GL_FALSE, glm::value_ptr(ProjMatrix));

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glDepthFunc(GL_LESS);

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