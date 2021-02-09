#pragma once

#include <QMatrix4x4>
#include <QList>
#include <QOpenGLExtraFunctions>
#include "Brush.h"
#include "Camera.h"
#include "GL.h"

class Renderer3D : OpenGLFunctions
{
public:
	Renderer3D(Camera* camera, int width, int height);
	~Renderer3D();

	void render(QList<Brush*>& objects);
		
private:
	QMatrix4x4 m_projMatrix;
	QMatrix4x4 m_viewMatrix;
	Camera* m_camera;
};
