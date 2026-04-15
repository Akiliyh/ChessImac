#pragma once
#include <glfw/src/internal.h>
#include <tiny_obj_loader.h>
#include <FilePath.hpp>
#include <Program.hpp>
#include <TrackballCamera.hpp>
#include <FreeflyCamera.hpp>
#include <Camera.hpp>
#include "GameManager.hpp"
#include "Geometry.hpp"
#include "Pieces.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"
#include "common.hpp"


struct ChessProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uBoardTexture;
    GLint uUseTexture;

    GLint uLightPos;
    GLint uLightColor;
    GLint uLightPos2;
    GLint uLightColor2;
    GLint uCamPos;
    GLint uIsSecondLightActive;

    GLint uColor;

    ChessProgram(const glimac::FilePath& applicationPath)
        : m_Program(loadProgram(
              applicationPath.dirPath() + "./assets/shaders/3D.vs.glsl",
              applicationPath.dirPath() + "./assets/shaders/colorTex3D.fs.glsl"
          ))
    {
        uMVPMatrix           = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix            = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix        = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uBoardTexture        = glGetUniformLocation(m_Program.getGLId(), "uBoardTexture");
        uColor               = glGetUniformLocation(m_Program.getGLId(), "uColor");
        uUseTexture          = glGetUniformLocation(m_Program.getGLId(), "uUseTexture");
        uLightColor          = glGetUniformLocation(m_Program.getGLId(), "uLightColor");
        uLightPos            = glGetUniformLocation(m_Program.getGLId(), "uLightPos");
        uLightColor2         = glGetUniformLocation(m_Program.getGLId(), "uLightColor2");
        uLightPos2           = glGetUniformLocation(m_Program.getGLId(), "uLightPos2");
        uCamPos              = glGetUniformLocation(m_Program.getGLId(), "uCamPos");
        uIsSecondLightActive = glGetUniformLocation(m_Program.getGLId(), "uIsSecondLightActive");
    }

    ChessProgram() = default;
};

class Renderer_3D {
  public:
    int  init(int width, int height);
    void init_vertex_object(const glimac::ShapeVertex* data, size_t count, GLuint& vbo, GLuint& vao);
    int  draw(int width, int height, GameManager& game);
    void draw_pieces(int piece_position, Piece* current_square, int col, int row);
    void set_lights(bool is_white_turn);
    void draw_possible_moves(const std::vector<int>& possible_moves, int piece_position, glm::mat4 squareMVMatrix) const;
    void terminate();
    void move_front(float delta);

    void change_camera();
    bool use_trackball_camera = false;
    std::unique_ptr<glimac::TrackballCamera>         trackball_camera;
    std::unique_ptr<glimac::FreeflyCamera>        freefly_camera;
    glimac::Camera* camera = nullptr;
    int selected_square_col;
    int selected_square_row;
    glm::vec3 ffly_cam_target_pos = glm::vec3(0.0, 1.0, 0.0);
    bool camera_oriented = false;


    std::unique_ptr<glimac::Skybox> skybox;

    // params
    float square_height;
    float square_width;
    float square_depth;
    float board_width;
    int game_board_size;
    int current_move; // to check if a piece moved


    bool      is_panning                  = false;
    bool      is_skybox_active            = true;
    bool      is_alternative_light_active = false;
    glm::vec3 light_pos                   = glm::vec3(0.0, 2.0, 0.0);
    glm::vec3 light_color                 = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 light_color2                = glm::vec3(.0f, .0f, 1.0f);
    glm::vec3 light_pos2                  = glm::vec3(0.0f, 1.f, 1.0f);

    int   anim_from    = -1;
    int   anim_to      = -1;
    float anim_time    = 0.3f;
    float anim_elapsed = 0.0f;
    bool  is_animating = false;

    double last_frame = 0.0;

    const float anim_duration = 0.3f;

    float fov = 70.f;

    glimac::Geometry pawnOBJ;
    glimac::Geometry bishopOBJ;
    glimac::Geometry kingOBJ;
    glimac::Geometry queenOBJ;
    glimac::Geometry rookOBJ;
    glimac::Geometry knightOBJ;

    int width{800};
    int height{800};

    GLuint boardVbo;
    GLuint squareVbo;
    GLuint cylinderVbo;

    GLuint boardVao;
    GLuint squareVao;
    GLuint cylinderVao;

    GLuint woodTexture;

    std::unique_ptr<ChessProgram> chessProgram;

    glm::mat4 ProjMatrix;
    glm::mat4 globalMVMatrix;

    // to improve and put inside a method, + dynamicity
    double lastX = width / 2; // start mouse position, here center of screen
    double lastY = height / 2;

    Renderer_3D() = default;
};