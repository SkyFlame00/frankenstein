#include "Renderer3D.h"

#include "GL.h"

Renderer3D::Renderer3D(Camera* camera, int width, int height)
	: m_camera(camera)
{
	float fov = 75.0f;
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(fov, (float)width / (float)height, 0.001f, 1000.0f);
}

Renderer3D::~Renderer3D()
{
}

void Renderer3D::render(QOpenGLContext* context, QList<Brush*>& objects, QList<Renderable*>& guiObjects)
{
	GLCall($->glEnable(GL_DEPTH_TEST));
	GLCall($->glClearColor(0.1, 0.2, 0.4, 1.0));
	GLCall($->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	for (auto& object : objects)
	{
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(0.0f, 0.0f, -1.0f);

		GLCall(auto& program = object->m_program);
		GLCall(program->bind());
		GLCall(program->setUniformValue("proj", m_projMatrix));
		GLCall(program->setUniformValue("view", m_camera->getViewMatrix()));
		GLCall(program->setUniformValue("model", model));
		GLCall(program->setUniformValue("color", 0.9f, 0.9f, 0.9f));
		GLCall(object->m_vao.bind());
		GLCall($->glDrawArrays(GL_TRIANGLES, 0, object->verticesCount()));
	}

	for (auto& guiObject : guiObjects)
	{
		guiObject->render3D(context, m_projMatrix, *m_camera);
	}
}
