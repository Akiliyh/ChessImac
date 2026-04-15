#pragma once

#include "common.hpp"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "Camera.hpp"

namespace glimac {


class FreeflyCamera : public Camera
{
    private: 
        glm::vec3 m_Position;
        float m_fPhi;
        float m_fTheta;

        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector;
        glm::vec3 m_UpVector;

        void computeDirectionVectors();

    public:
    FreeflyCamera(): 
        m_Position(glm::vec3(0,0,0)), m_fPhi(glm::pi<float>()), m_fTheta(0.0) 
    {
        computeDirectionVectors();
    };

    void moveLeft(float t);
    void moveFront(float t) override;
    void rotateLeft(float degrees) override;
    void rotateUp(float degrees) override;
    glm::mat4 getViewMatrix() const override;
};

}  // namespace glimac