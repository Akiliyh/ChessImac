#include <iostream>
#include "glm/trigonometric.hpp"
#include "TrackballCamera.hpp"

namespace glimac {

void TrackballCamera::moveFront(float delta)
{
    m_fDistance+=delta;
}

void TrackballCamera::moveTo(float distance)
{
    m_fDistance = distance;
}

glm::vec3 TrackballCamera::get_position()
{
    float x = m_fDistance * std::cos(m_fAngleX) * std::sin(m_fAngleY);
    float y = m_fDistance * std::sin(m_fAngleX);
    float z = m_fDistance * std::cos(m_fAngleX) * std::cos(m_fAngleY);

    return {x, y, z};
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_fAngleX+=degrees;
}

void TrackballCamera::rotateUp(float degrees)
{/*
    // you don't want to see what's below the board ⠀⠀⠀⠀⠀⠀⠀⠀
    //     ⣀⣤⣶⣾⡧⠀⠀⠀⠀⠀⠀⠀⣾⣷⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠰⠿⠟⣋⡭⠒⠉⠉⠀⠀⠀⠀⠀⠀⠈⠉⠛⠿⣿⡿⠆⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡠⠒⠅⢁⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⡈⢳⣄⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⡠⠊⠀⠀⠀⠀⠀⠀⠈⠓⢄⠀⠀⠀⠀⢀⠔⠉⠀⠀⠀⠈⠑⠻⣷⣄⠀⠀⠀⠀
⠀⠀⢠⠊⣴⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠳⠀⠀⡰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣷⣄⠀⠀
⠀⢰⠁⢈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢰⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⡆⠀
⢠⠃⠀⡜⠀⠀⠀⠀⠀⣴⣿⡟⠑⡀⠀⠀⡇⢈⠀⠀⠀⢰⣾⡉⣀⡄⠀⠀⠀⠀⢸⣿⣿⡄
⡆⠀⠀⡇⠀⠀⠀⠀⠀⠙⠿⠷⠞⠀⠀⠀⣯⠘⡆⠀⠀⠘⠿⣿⠿⠁⠀⠀⠀⠀⢸⢻⣿⣿
⠀⠀⠀⢻⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠃⠀⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣼⣿⣿
⠀⠀⠀⠈⢳⡄⠀⠀⠀⠀⠀⠀⠀⣀⡾⠁⠀⠀⠀⠙⣄⠀⠀⠀⠀⠀⠀⠀⠀⣴⡟⠀⠹⣿
⡀⠀⠀⠀⠀⠈⠓⠤⣀⣀⡠⠴⠞⠉⠀⢀⠀⠀⠀⠀⠐⢝⣶⣤⣀⣀⣠⠴⠿⠋⠀⠀⠀⣿
⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣷⣿⣷⣿⣶⣀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸
⠘⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃
⠀⠘⣿⣷⣀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀
⠀⠀⠘⢿⣿⣷⡄⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⡠⠋⠀⠀
⠀⠀⠀⠀⠙⢿⠇⠀⠀⠀⠀⢰⣿⣿⡿⠟⠛⠛⠛⠻⢿⣿⣿⣿⠀⠀⠀⠀⡠⠊⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠑⠦⣀⠀⠀⢸⡿⠧⢄⣀⣀⣀⣀⡠⠤⠽⢻⣿⠀⣀⠤⠊⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠒⢾⢡⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⡰⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠡⢍⡓⢒⠶⡶⠶⠶⠒⣉⠔⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠢⢀⠀⠀⠀⢀⠠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀*/
    if ((m_fAngleY + degrees) > 0.0f && (m_fAngleY + degrees) < 180.0f)
    {
        m_fAngleY+=degrees;
    }
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0,0,-m_fDistance));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleY), glm::vec3(1,0,0));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleX), glm::vec3(0,1,0));

    return ViewMatrix;
}

}  // namespace glimac