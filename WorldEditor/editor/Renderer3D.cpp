#include "Renderer3D.h"

#include "GL.h"

Renderer3D::Renderer3D(Camera* camera, int width, int height)
	: m_camera(camera)
{
	float fov = 75.0f;
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(fov, (float)width / (float)height, 0.0f, 1000.0f);
}

Renderer3D::~Renderer3D()
{
}

void Renderer3D::render(QList<Brush*>& objects)
{
	$->glClearColor(0.0, 1.0, 0.0, 1.0);
	$->glClear(GL_COLOR_BUFFER_BIT);

	for (auto& object : objects)
	{
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(0.0f, 0.0f, -1.0f);

		auto& program = object->m_program;
		program->bind();
		program->setUniformValue("proj", m_projMatrix);
		program->setUniformValue("view", m_camera->getViewMatrix());
		program->setUniformValue("model", model);

		object->m_vao.bind();
		$->glDrawArrays(GL_TRIANGLES, 0, object->verticesCount());
	}
}
