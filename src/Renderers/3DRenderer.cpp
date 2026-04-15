#include "3DRenderer.hpp"
#include <cmath>
#include <cstddef>
#include <vector>
#include "Cylinder.hpp"
#include "GameManager.hpp"
#include "Image.hpp"
#include "Pieces.hpp"
#include "Shader.hpp"
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
#include <Skybox.hpp>
#include <Sphere.hpp>
#include <getTime.hpp>
#include <glm.hpp>

using namespace glimac;

Cube     board(0.05f, 1.125f, 1.125f);
Cube     square(0.0125f, 0.125f, 0.125f);
Cylinder cylinder(0.1f, 0.02f, 20, 20);

const float texture_clipping_delta{0.001f};

void Renderer_3D::init_vertex_object(
    const ShapeVertex* data, size_t count, GLuint& vbo, GLuint& vao
)
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

void Renderer_3D::change_camera()
{
    if (!use_trackball_camera)
    {
        camera               = trackball_camera.get();
        use_trackball_camera = true;
    }
    else
    {
        camera               = freefly_camera.get();
        use_trackball_camera = false;
    }
}

int Renderer_3D::init(int width, int height)
{
    trackball_camera = std::make_unique<glimac::TrackballCamera>();
    freefly_camera   = std::make_unique<glimac::FreeflyCamera>();

    camera = freefly_camera.get();

    square_width  = square.getWidth();
    square_height = square.getHeight();
    square_depth  = square.getDepth();

    board_width = board.getWidth();

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

    init_vertex_object(board.getDataPointer(), board.getVertexCount(), boardVbo, boardVao);
    init_vertex_object(square.getDataPointer(), square.getVertexCount(), squareVbo, squareVao);
    init_vertex_object(
        cylinder.getDataPointer(), cylinder.getVertexCount(), cylinderVbo, cylinderVao
    );

    skybox = std::make_unique<Skybox>();

    std::vector<std::string> faces = {
        "./assets/textures/skybox/right.jpg", "./assets/textures/skybox/left.jpg",
        "./assets/textures/skybox/top.jpg",   "./assets/textures/skybox/bottom.jpg",
        "./assets/textures/skybox/front.jpg", "./assets/textures/skybox/back.jpg"
    };

    skybox->init(applicationPath, faces);

    return 0;
}

void Renderer_3D::draw_pieces(int piece_position, Piece* current_square, int col, int row) const
{
    if (current_square)
    {
        glm::mat4 pieceMVMatrix = globalMVMatrix; // Translation
        pieceMVMatrix           = glm::translate(
            pieceMVMatrix, glm::vec3(-1, square_height * 5, -1 + (board_width / 8.0f))
        );

        // here we retrieve the current correct piece_position
        int anim_row = piece_position / game_board_size;
        int anim_col = piece_position % game_board_size;

        float draw_col      = col;
        float draw_row      = row;
        float draw_y        = 1.0f;
        float height_offset = 0.0f;

        // if animation we update
        if (is_animating && piece_position == anim_to)
        {
            float time =
                glm::sin(((anim_elapsed / anim_duration) * glm::pi<float>()) / 2); // easeout

            float jump_height = 0.3f;
            height_offset     = 4.0f * jump_height * time * (1.0f - time);

            float from_row = anim_from / game_board_size;
            float from_col = anim_from % game_board_size;
            // we set float here so the mix works great

            float to_row = anim_to / game_board_size;
            float to_col = anim_to % game_board_size;

            // interpolation time
            draw_col = glm::mix(from_col, to_col, time);
            draw_row = glm::mix(from_row, to_row, time);
        }

        bool is_knight = (std::tolower(current_square->get_label()) == 'n');

        pieceMVMatrix = glm::translate(
            pieceMVMatrix,
            glm::vec3(
                glm::vec3(
                    ((square_width * 2)) * draw_col + ((board_width / 8.0)),
                    (square_height + texture_clipping_delta) + ((is_knight) ? height_offset : 0),
                    (square_width * 2) * draw_row
                )
            )
        );

        pieceMVMatrix = glm::scale(pieceMVMatrix, glm::vec3(0.0625, 0.0625, 0.0625));

        (current_square->get_color() == White)
            ? pieceMVMatrix =
                  glm::rotate(pieceMVMatrix, 3.f * glm::pi<float>() * .5f, glm::vec3(0, 1, 0))
            : pieceMVMatrix =
                  glm::rotate(pieceMVMatrix, glm::pi<float>() * .5f, glm::vec3(0, 1, 0));

        glm::mat4 pieceMVPMatrix    = ProjMatrix * pieceMVMatrix;
        glm::mat4 pieceNormalMatrix = glm::transpose(glm::inverse(glm::mat3(pieceMVMatrix)));

        glUniformMatrix4fv(chessProgram->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVPMatrix));
        glUniformMatrix4fv(chessProgram->uMVMatrix, 1, GL_FALSE, glm::value_ptr(pieceMVMatrix));
        glUniformMatrix3fv(
            chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(pieceNormalMatrix)
        );

        glUniform1i(chessProgram->uUseTexture, 0);

        (current_square->get_color() == White)
            ? glUniform3f(chessProgram->uColor, 1.0f, 1.0f, 1.0f)
            : glUniform3f(chessProgram->uColor, 0.3f, 0.2f, 0.2f);

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

void Renderer_3D::set_lights(bool alternative_light_condition)
{
    // here we set the lights up in the shader
    alternative_light_condition ? is_alternative_light_active = false
                                : is_alternative_light_active = true;
    glm::vec3 lightColor = light_color;
    glm::vec3 lightPos   = light_pos;

    glm::vec3 lightColor2 = glm::vec3(.0f, .0f, 1.0f); // full on red
    // the second mode we move the lights in circle bcs why not
    const float rotation_speed = 5.0f;
    glm::vec3   lightPos2 =
        glm::vec3(glm::cos(getTime() * rotation_speed), 1.f, glm::sin(getTime() * rotation_speed));

    lightPos  = glm::vec3(camera->getViewMatrix() * glm::vec4(lightPos, 1.0));
    lightPos2 = glm::vec3(camera->getViewMatrix() * glm::vec4(lightPos2, 1.0));
    glUniform3f(chessProgram->uLightColor, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(chessProgram->uLightPos, lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(chessProgram->uIsSecondLightActive, is_alternative_light_active);

    glUniform3f(chessProgram->uLightColor2, lightColor2.r, lightColor2.g, lightColor2.b);
    glUniform3f(chessProgram->uLightPos2, lightPos2.x, lightPos2.y, lightPos2.z);
}

void Renderer_3D::draw_possible_moves(
    const std::vector<int>& possible_moves, int piece_position, glm::mat4 squareMVMatrix
) const
{
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

        possible_moveMVMatrix =
            glm::translate(squareMVMatrix, glm::vec3(0, square_height + texture_clipping_delta, 0));

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
            chessProgram->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(possible_moveNormalMatrix)
        );

        glDrawArrays(GL_TRIANGLES, 0, cylinder.getVertexCount());
    }
}

int Renderer_3D::draw(int width, int height, GameManager& game)
{
    // here we change camera
    (use_trackball_camera) ? camera = trackball_camera.get() : camera = freefly_camera.get();

    if (current_move != game.get_move())
    {
        auto move_opt = game.get_last_move();

        if (move_opt)
        {
            anim_from = move_opt->from;
            anim_to   = move_opt->to;

            anim_time    = 0.3f;
            anim_elapsed = 0.0f;
            is_animating = true;
        }

        current_move = game.get_move();
    }

    double current_frame = getTime();
    double delta_time    = current_frame - last_frame;
    last_frame           = current_frame;

    if (is_animating)
    {
        anim_elapsed += delta_time;

        if (anim_elapsed >= anim_duration)
        {
            is_animating = false;
            anim_elapsed = anim_duration;
        }
    }

    ProjMatrix = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.f);
    chessProgram->m_Program.use();

    const bool is_white_turn = game.is_white_turn();

    // lighting params
    set_lights(is_white_turn);

    glViewport(0, 0, width, height);

    glClearColor(0.1f, 0.f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(chessProgram->uBoardTexture, 0);
    glUniform1i(chessProgram->uUseTexture, 1);

    globalMVMatrix = camera->getViewMatrix();

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

    int piece_position{};
    game_board_size = game.board.get_size();

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
            piece_position = col + (row * game_board_size);

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

            isDark ? glUniform3f(chessProgram->uColor, 0.1f, 0.1f, 0.1f)
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
            const std::vector<int>& possible_moves = game.get_possible_moves();
            draw_possible_moves(possible_moves, piece_position, squareMVMatrix);

            // deal with pawn here
            // we want to display a piece only if it exists in the game board

            current_square = game.board.get_board_data(piece_position).get();
            Renderer_3D::draw_pieces(piece_position, current_square, col, row);
        }
    }

    // render skybox

    if (!is_skybox_active)
        return 0;

    skybox->draw(camera->getViewMatrix(), ProjMatrix);

    // here we update the move for piece animation
    current_move = game.get_move();
    return 0;
}

void Renderer_3D::terminate()
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &squareVbo);
    glDeleteVertexArrays(1, &squareVao);
    glDeleteBuffers(1, &boardVbo);
    glDeleteVertexArrays(1, &boardVao);
    glDeleteBuffers(1, &cylinderVbo);
    glDeleteVertexArrays(1, &cylinderVao);
}