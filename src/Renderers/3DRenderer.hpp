#pragma once
#include <glfw/src/internal.h>
#include "GameManager.hpp"
#include <TrackballCamera.hpp>
#include <Program.hpp>
#include <FilePath.hpp>

struct EarthProgram {
    glimac::Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const glimac::FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "./assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "./assets/shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }

    EarthProgram() = default;
};

class Renderer_3D {
  public:
    int init(int width, int height);
    int draw(int width, int height, GameManager& game);
    void terminate();
    void moveFront(float delta);
    glimac::TrackballCamera camera;
    bool is_panning = false;

    int width {800};
    int height {800};

    GLuint vbo;
    GLuint vao;

    GLuint earthTexture;
    GLuint cloudTexture;

    std::unique_ptr<EarthProgram> earthProgram;

    glm::mat4 ProjMatrix;

    // to improve and put inside a method, + dynamicity
    double lastX = width/2; //start mouse position, here center of screen
    double lastY = height/2;

    Renderer_3D() = default;
};