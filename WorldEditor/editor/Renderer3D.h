#pragma once

#include <QMatrix4x4>
#include <QList>
#include <QOpenGLExtraFunctions>
#include "Brush.h"
#include "Camera.h"
#include "GL.h"
#include "Renderable.h"

class Renderer3D : OpenGLFunctions
{
public:
	Renderer3D(Camera* camera, int width, int height);
	~Renderer3D();

	void render(QOpenGLContext* context, QList<Brush*>& objects, QList<Renderable*>& guiObjects);
	void setFrustrum(int width, int height);
	inline float getNearPlane() { return m_near; }
	inline float getFarPlane() { return m_far; }
	inline QMatrix4x4& getProjMatrix() { return m_projMatrix; }
		
private:
	QMatrix4x4 m_projMatrix;
	QMatrix4x4 m_viewMatrix;
	Camera* m_camera;
	float m_fov = 75.0f;
	float m_near = 0.001f;
	float m_far = 1000.0f;
};
