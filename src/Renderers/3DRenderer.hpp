#pragma once
#include <glfw/src/internal.h>
#include "GameManager.hpp"
#include <TrackballCamera.hpp>
#include <Program.hpp>
#include <FilePath.hpp>
#include <tiny_obj_loader.h>
#include <OBJModel.hpp>

struct ChessProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uBoardTexture;

    GLint uColor;

    ChessProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "./assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "./assets/shaders/colorTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uBoardTexture = glGetUniformLocation(m_Program.getGLId(), "uBoardTexture");
        uColor = glGetUniformLocation(m_Program.getGLId(), "uColor");
    }

    ChessProgram() = default;
};

class Renderer_3D {
  public:
    int init(int width, int height);
    int draw(int width, int height, GameManager& game);
    void terminate();
    void moveFront(float delta);
    glimac::TrackballCamera camera;
    bool is_panning = false;
    glimac::OBJModel pawnOBJ;

    int width {800};
    int height {800};

    GLuint boardVbo;
    GLuint squareVbo;
    GLuint boardVao;
    GLuint squareVao;

    GLuint woodTexture;

    std::unique_ptr<ChessProgram> chessProgram;

    glm::mat4 ProjMatrix;

    // to improve and put inside a method, + dynamicity
    double lastX = width/2; //start mouse position, here center of screen
    double lastY = height/2;

    Renderer_3D() = default;
};