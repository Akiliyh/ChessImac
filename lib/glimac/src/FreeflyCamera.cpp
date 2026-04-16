#include <cmath>
#include <vector>
#include <iostream>
#include "common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "FreeflyCamera.hpp"

namespace glimac {

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos(m_fTheta) * glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta) * glm::cos(m_fPhi));
    m_LeftVector = glm::vec3(glm::sin(m_fPhi + (glm::pi<float>()/2)), 0, glm::cos(m_fPhi + (glm::pi<float>() / 2)));
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t*m_LeftVector;
}

void FreeflyCamera::moveFront(float t)
{
    // m_Position += t*m_FrontVector;
    // we don't want it to move so that's one technique
}

void FreeflyCamera::rotateLeft(float degrees)
{
    degrees = glm::radians(degrees);
    m_fPhi+=degrees;
    computeDirectionVectors();
}

void FreeflyCamera::set_position(glm::vec3 position)
{
    m_Position = position;
    computeDirectionVectors();
}

glm::vec3 FreeflyCamera::get_position()
{
    return m_Position;
}

void FreeflyCamera::set_pov(bool is_white)
{
    m_fPhi = is_white ? glm::radians(180.0f) : glm::radians(0.0f);
    m_fTheta = 0.0;
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    degrees = glm::radians(degrees);
    m_fTheta+=degrees;
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    glm::mat4 ViewMatrix = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    return ViewMatrix;
}

}  // namespace glimac