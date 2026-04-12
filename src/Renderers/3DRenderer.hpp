#pragma once
#include <glfw/src/internal.h>
#include "GameManager.hpp"
#include <TrackballCamera.hpp>
#include <Program.hpp>
#include <FilePath.hpp>
#include <tiny_obj_loader.h>
#include "Geometry.hpp"
#include "Pieces.hpp"
#include "Shader.hpp"
#include "common.hpp"

struct ChessProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uBoardTexture;
    GLint uUseTexture;

    GLint uColor;

    ChessProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "./assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "./assets/shaders/colorTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uBoardTexture = glGetUniformLocation(m_Program.getGLId(), "uBoardTexture");
        uColor = glGetUniformLocation(m_Program.getGLId(), "uColor");
        uUseTexture = glGetUniformLocation(m_Program.getGLId(), "uUseTexture");
    }

    ChessProgram() = default;
};

struct SkyboxProgram {
    glimac::Program m_Program;

    GLint uView;
    GLint uProjection;
    GLint uSkybox;

    SkyboxProgram(const glimac::FilePath& appPath)
        : m_Program(loadProgram(
            appPath.dirPath() + "./assets/shaders/skybox.vs.glsl",
            appPath.dirPath() + "./assets/shaders/skybox.fs.glsl"
        )) 
    {
        uView = glGetUniformLocation(m_Program.getGLId(), "view");
        uProjection = glGetUniformLocation(m_Program.getGLId(), "projection");
        uSkybox = glGetUniformLocation(m_Program.getGLId(), "skybox");
    }

    SkyboxProgram() = default;
};

class Renderer_3D {
  public:
    int init(int width, int height);
    void initVertexObject(const glimac::ShapeVertex* data, size_t count, GLuint& vbo, GLuint& vao);
    int draw(int width, int height, GameManager& game);
    void terminate();
    void move_front(float delta);
    glimac::TrackballCamera camera;

    bool is_panning = false;
    bool is_skybox_active = true;
    float fov = 70.f;

    glimac::Geometry pawnOBJ;
    glimac::Geometry bishopOBJ;
    glimac::Geometry kingOBJ;
    glimac::Geometry queenOBJ;
    glimac::Geometry rookOBJ;
    glimac::Geometry knightOBJ;
    glimac::Geometry testOBJ;

    int width {800};
    int height {800};

    GLuint boardVbo;
    GLuint squareVbo;
    GLuint cylinderVbo;
    
    GLuint boardVao;
    GLuint squareVao;
    GLuint cylinderVao;

    GLuint woodTexture;

    std::unique_ptr<ChessProgram> chessProgram;

    std::unique_ptr<SkyboxProgram> skyboxProgram;
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint cubemapTexture;

    glm::mat4 ProjMatrix;

    // to improve and put inside a method, + dynamicity
    double lastX = width/2; //start mouse position, here center of screen
    double lastY = height/2;

    Renderer_3D() = default;
};