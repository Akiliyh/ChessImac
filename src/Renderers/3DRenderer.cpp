#include "3DRenderer.hpp"
#include "GameManager.hpp"
#include "Image.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <cstddef>
#include <vector>
#define GLFW_INCLUDE_NONE
#include "common.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <FilePath.hpp>
#include <Program.hpp>
#include <Sphere.hpp>
#include <Cube.hpp>
#include <getTime.hpp>
#include <glm.hpp>
#include <TrackballCamera.hpp>

using namespace glimac;

int window_width = 800;
int window_height = 800;
bool is_panning = false;

TrackballCamera camera;

Cube board(0.05f, 1.0f, 1.0f);

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "./assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "./assets/shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    camera.moveFront(1.0f);

  if (key == GLFW_KEY_H && action == GLFW_PRESS)
    camera.rotateLeft(1.0f);
}

static void mouse_button_callback(GLFWwindow *window, int button,
                                  int action, int mods) 
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    is_panning = !is_panning;
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    is_panning = false;
  }
}



static void scroll_callback(GLFWwindow * window, double xoffset,
                            double yoffset) 
  {
    camera.moveFront(-yoffset);
  }

static double lastX = window_width/2; //start mouse position, here center of screen
static double lastY = window_height/2;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  const double deltaX = xpos - lastX;
  const double deltaY = ypos - lastY;

  const float intensity = 0.1f;
    
  if (is_panning) {
    camera.rotateLeft(deltaX * intensity);
    camera.rotateUp(deltaY * intensity);
  }
  
  lastX = xpos;
  lastY = ypos;
}

static void size_callback(GLFWwindow * /*window*/, int width, int height) {
  window_width = width;
  window_height = height;
}

int Renderer_3D::draw(GameManager& game)
{
   /* Initialize the library */
  if (!glfwInit()) {
    return -1;
  }

  /* Create a window and its OpenGL context */
  // #ifdef __APPLE__
  //     /* We need to explicitly ask for a 3.3 context on Mac */
  //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // #endif
  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "ChessImac", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Intialize glad (loads the OpenGL functions) */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  /* Hook input callbacks */
  glfwSetKeyCallback(window, &key_callback);
  glfwSetMouseButtonCallback(window, &mouse_button_callback);
  glfwSetScrollCallback(window, &scroll_callback);
  glfwSetCursorPosCallback(window, &cursor_position_callback);
  glfwSetWindowSizeCallback(window, &size_callback);

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  auto earthImage = glimac::loadImage("./assets/textures/EarthMap.jpg");
  auto cloudImage = glimac::loadImage("./assets/textures/CloudMap.jpg");

  GLuint earthTexture;
  GLuint cloudTexture;
  glGenTextures(1, &earthTexture);
  glGenTextures(1, &cloudTexture);

  glBindTexture(GL_TEXTURE_2D, earthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthImage->getWidth(),
               earthImage->getHeight(), 0, GL_RGBA, GL_FLOAT,
               earthImage->getPixels());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, cloudTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudImage->getWidth(),
                 cloudImage->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 cloudImage->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glimac::FilePath applicationPath("color2D.vs.glsl");
  EarthProgram earthProgram(applicationPath);

  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), 1.0f, 0.1f, 100.f);
  glm::mat4 MVMatrix =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
  glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
  glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

  glEnable(GL_DEPTH_TEST);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-0.25f,-1.0f, // triangle 1 : begin
    -1.0f,-0.25f, 1.0f,
    -1.0f, 0.25f, 1.0f, // triangle 1 : end
    1.0f, 0.25f,-1.0f, // triangle 2 : begin
    -1.0f,-0.25f,-1.0f,
    -1.0f, 0.25f,-1.0f, // triangle 2 : end
    1.0f,-0.25f, 1.0f,
    -1.0f,-0.25f,-1.0f,
    1.0f,-0.25f,-1.0f,
    1.0f, 0.25f,-1.0f,
    1.0f,-0.25f,-1.0f,
    -1.0f,-0.25f,-1.0f,
    -1.0f,-0.25f,-1.0f,
    -1.0f, 0.25f, 1.0f,
    -1.0f, 0.25f,-1.0f,
    1.0f,-0.25f, 1.0f,
    -1.0f,-0.25f, 1.0f,
    -1.0f,-0.25f,-1.0f,
    -1.0f, 0.25f, 1.0f,
    -1.0f,-0.25f, 1.0f,
    1.0f,-0.25f, 1.0f,
    1.0f, 0.25f, 1.0f,
    1.0f,-0.25f,-1.0f,
    1.0f, 0.25f,-1.0f,
    1.0f,-0.25f,-1.0f,
    1.0f, 0.25f, 1.0f,
    1.0f,-0.25f, 1.0f,
    1.0f, 0.25f, 1.0f,
    1.0f, 0.25f,-1.0f,
    -1.0f, 0.25f,-1.0f,
    1.0f, 0.25f, 1.0f,
    -1.0f, 0.25f,-1.0f,
    -1.0f, 0.25f, 1.0f,
    1.0f, 0.25f, 1.0f,
    -1.0f, 0.25f, 1.0f,
    1.0f,-0.25f, 1.0f
};

  const ShapeVertex *vertices = board.getDataPointer();
  const int nb_vertices = board.getVertexCount();

  // const ShapeVertex *vertices = sphere.getDataPointer();
  // const int nb_vertices = sphere.getVertexCount();

  glBufferData(GL_ARRAY_BUFFER, board.getVertexCount() * sizeof(ShapeVertex), vertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  const GLuint VERTEX_SHADER_POSITION = 0;
  const GLuint VERTEX_SHADER_NORMAL = 1;
  const GLuint VERTEX_SHADER_TEX_COORDS = 2;

  glEnableVertexAttribArray(VERTEX_SHADER_POSITION);
  glEnableVertexAttribArray(VERTEX_SHADER_NORMAL);
  glEnableVertexAttribArray(VERTEX_SHADER_TEX_COORDS);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(VERTEX_SHADER_POSITION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex), nullptr);
  glVertexAttribPointer(VERTEX_SHADER_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        nullptr);
  glVertexAttribPointer(VERTEX_SHADER_TEX_COORDS, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glfwSetKeyCallback(window, key_callback);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/

     earthProgram.m_Program.use();

    glUniform1i(earthProgram.uEarthTexture, 0);
    glUniform1i(earthProgram.uCloudTexture, 1);

    glm::mat4 globalMVMatrix = camera.getViewMatrix();

    glm::mat4 earthMVMatrix = globalMVMatrix;
glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, 
	glm::value_ptr(earthMVMatrix));
glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, 
	glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, 
	glm::value_ptr(ProjMatrix * earthMVMatrix));

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, earthTexture);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, cloudTexture);

glBindVertexArray(vao);
glDrawArrays(GL_TRIANGLES, 0, board.getVertexCount());

    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  return 0;
}