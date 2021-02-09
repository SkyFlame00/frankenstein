#include "Camera.h"

#include <QtMath>

Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch,
	float fov, float mouseSensitivity, float movementSpeed)
	: m_position(position), m_up(up), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
	  m_mouseSensitivity(mouseSensitivity), m_movementSpeed(movementSpeed)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    m_front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    m_front.setY(qSin(qDegreesToRadians(m_pitch)));
    m_front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    m_front.normalize();

    m_right = QVector3D::crossProduct(m_front, m_worldUp);
    m_right.normalize();

    m_up = QVector3D::crossProduct(m_front, m_right);
    m_up.normalize();
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(m_position, m_position + m_front, m_up);
    return viewMatrix;
}

void Camera::processKeyboard(Direction direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == Direction::FORWARD)
        m_position += m_front * velocity;
    if (direction == Direction::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == Direction::LEFT)
        m_position -= m_right * velocity;
    if (direction == Direction::RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
}
