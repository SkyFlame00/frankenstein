#include "Renderer3D.h"

#include "GL.h"

Renderer3D::Renderer3D(Camera* camera, int width, int height)
	: m_camera(camera)
{
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(m_fov, (float)width / (float)height, 0.001f, 1000.0f);
}

Renderer3D::~Renderer3D()
{
}

void Renderer3D::setFrustrum(int width, int height)
{
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(m_fov, (float)width / (float)height, 0.001f, 1000.0f);
}

void Renderer3D::render(QOpenGLContext* context, QList<Brush*>& objects, QList<Renderable*>& guiObjects)
{
	GLCall($->glEnable(GL_DEPTH_TEST));
	GLCall($->glClearColor(0.1, 0.2, 0.4, 1.0));
	GLCall($->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	QVector3D zoomVec(1.0f, 1.0f, 1.0f);

	for (auto& object : objects)
	{
		object->render3D(context, m_projMatrix, zoomVec, *m_camera);
	}

	for (auto& guiObject : guiObjects)
	{
		guiObject->render3D(context, m_projMatrix, *m_camera);
	}
}
