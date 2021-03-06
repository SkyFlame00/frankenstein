#include "Camera.h"
#include <QtMath>

const float Camera::_DEFAULT_YAW = -90.0f;
const float Camera::_DEFAULT_PITCH = 0.0f;

Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch,
	float fov, float mouseSensitivity, float movementSpeed)
	: m_position(position), m_up(up), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
	  m_mouseSensitivity(mouseSensitivity), m_movementSpeed(movementSpeed)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors(bool shouldUpdateFrontVec)
{
    if (shouldUpdateFrontVec)
    {
        m_front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
        m_front.setY(qSin(qDegreesToRadians(m_pitch)));
        m_front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
        m_front.normalize();
    }

    m_right = QVector3D::crossProduct(m_front, m_worldUp);
    m_right.normalize();

    m_up = QVector3D::crossProduct(m_right, m_front);
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

void Camera::moveRelativelyToAxis(Axis axis, float horShift, float verShift)
{
    switch (axis)
    {
    case Axis::X:
    {
        m_position.setZ(m_position.z() + horShift);
        m_position.setY(m_position.y() + verShift);
        break;
    }
    case Axis::Y:
    {
        m_position.setX(m_position.x() + horShift);
        m_position.setZ(m_position.z() + verShift);
        break;
    }
    case Axis::Z:
    {
        m_position.setX(m_position.x() + horShift);
        m_position.setY(m_position.y() + verShift);
        break;
    }
    }
}

QVector3D Camera::getPickingRay(int x, int y, int screenWidth, int screenHeight, float nearPlane, float farPlane, QMatrix4x4& proj)
{
    float x_ndc = (2.0f * x) / screenWidth - 1.0f;
    float y_ndc = 1.0f - (2.0f * y) / screenHeight;
    float z_ndc = 1.0f;
    QVector3D ray_ndc = QVector3D(x_ndc, y_ndc, z_ndc);
    QVector4D ray_clip = QVector4D(ray_ndc.x(), ray_ndc.y(), -1.0f, 1.0f);

    QVector4D ray_eye = proj.inverted() * ray_clip;
    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0f, 0.0f);

    QVector4D ray_wor = getViewMatrix().inverted() * ray_eye;
    QVector3D ray(ray_wor.x(), ray_wor.y(), ray_wor.z());
    ray.normalize();

    return ray;
}
