#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include "../common/types.h"

class Camera
{
public:
	const float MAX_FOV = 90.0f;
	static const float _DEFAULT_YAW;
	static const float _DEFAULT_PITCH;

	enum class Direction
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	Camera(
		QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
		QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f,
		float fov = 45.0f,
		float mouseSensitivity = 0.1f,
		float movementSpeed = 2.5f
	);

	QMatrix4x4 getViewMatrix();
	void processKeyboard(Camera::Direction direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset);
	void updateCameraVectors(bool shouldUpdateFrontVec = true);
	inline QVector3D getPosition() { return m_position; }
	inline void setPosition(QVector3D position) { m_position = position; }
	void moveRelativelyToAxis(Axis axis, float horShift, float verShift);
	void inline setWorldUpVec(QVector3D vec) { m_worldUp = vec; }
	void inline setFrontVec(QVector3D vec) { m_front = vec; }
	QVector3D getPickingRay(int x, int y, int screenWidth, int screenHeight, float nearPlane, float farPlane, QMatrix4x4& proj);
	void inline setYaw(float yaw) { m_yaw = yaw; }
	void inline setPitch(float pitch) { m_pitch = pitch; }

private:
	QVector3D m_position;
	QVector3D m_front;
	QVector3D m_worldUp;
	QVector3D m_up;
	QVector3D m_right;
	float m_yaw;
	float m_pitch;
	float m_fov;
	float m_mouseSensitivity;
	float m_movementSpeed;
};
