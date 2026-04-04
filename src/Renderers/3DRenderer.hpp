#pragma once
#include "GameManager.hpp"
#include <TrackballCamera.hpp>

class Renderer_3D {
  public:
    int init();
    int draw(int width, int height, GameManager& game);
    void moveFront(float delta);
    glimac::TrackballCamera camera;
    bool is_panning = false;

    int width {800};
    int height {800};

    // to improve and put inside a method, + dynamicity
    double lastX = width/2; //start mouse position, here center of screen
    double lastY = height/2;

    

    Renderer_3D() = default;
};