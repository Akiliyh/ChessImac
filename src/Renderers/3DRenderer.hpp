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

    // to improve and put inside a method, + dynamicity
    double lastX = 800/2; //start mouse position, here center of screen
    double lastY = 800/2;

    Renderer_3D() = default;
};