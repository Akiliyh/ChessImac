#pragma once
#include <cmath>
#include <vector>
#include "glm.hpp"
#include "Camera.hpp"

namespace glimac {


class TrackballCamera : public Camera
{
    private: 
        float m_fDistance;
        float m_fAngleX;
        float m_fAngleY;

    public:
    TrackballCamera(): 
        m_fDistance(5), m_fAngleX(0), m_fAngleY(0) 
    {};
    void moveFront(float delta) override;
    void moveTo(float distance);
    void rotateLeft(float degrees) override;
    void rotateUp(float degrees) override;
    glm::mat4 getViewMatrix() const override;
};

}