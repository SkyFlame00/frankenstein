#include "Renderer2D.h"

Renderer2D::Renderer2D(Camera* camera, float width, float height)
	: m_camera(camera), m_width(width), m_height(height), m_zoom(1.0f, 1.0f, 1.0f)
{
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::setup(float _near, float _far)
{
	m_near = _near;
	m_far = _far;
	m_projMatrix.setToIdentity();
	//m_projMatrix.ortho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, _near, _far);
	m_projMatrix.ortho(-32.0f, 32.0f, -32.0f, 32.0f, 1.0f, -32.0f * 2);
	//m_camera->setPosition(QVector3D(0.0f, 0.0f, m_grid->HALF_CUBE));
	m_camera->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
	m_camera->updateCameraVectors();
}

void Renderer2D::setFrustrum(float width, float height)
{
	m_width = width;
	m_height = height;
	m_projMatrix.setToIdentity();
	m_projMatrix.ortho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, m_near, m_far);
}

void Renderer2D::zoomIn()
{
	m_zoom.setX(m_axis == Axis::X ? 1.0f : m_zoom.x() + 1);
	m_zoom.setY(m_axis == Axis::Y ? 1.0f : m_zoom.y() + 1);
	m_zoom.setZ(m_axis == Axis::Z ? 1.0f : m_zoom.z() + 1);
}

void Renderer2D::zoomOut()
{
	m_zoom.setX(m_axis == Axis::X ? 1.0f : m_zoom.x() - 1);
	m_zoom.setY(m_axis == Axis::Y ? 1.0f : m_zoom.y() - 1);
	m_zoom.setZ(m_axis == Axis::Z ? 1.0f : m_zoom.z() - 1);
}

void Renderer2D::render(QList<Brush*>& objects, QList<Renderable*>& guiObjects)
{
	$->glClearColor(0.0, 0.0, 0.0, 1.0);
	$->glClear(GL_COLOR_BUFFER_BIT);


	for (auto& object : objects)
	{
		auto program = object->m_program;

		QMatrix4x4 model;
		model.setToIdentity();
		model.scale(m_zoom);

		program->bind();
		program->setUniformValue("proj", m_projMatrix);
		program->setUniformValue("view", m_camera->getViewMatrix());
		program->setUniformValue("model", model);
		program->setUniformValue("color", 1.0f, 1.0f, 1.0f);
		
		object->m_vao.bind();
		$->glDrawArrays(GL_LINES, 0, object->verticesCount());
	}

	for (auto& guiObject : guiObjects)
	{
		auto program = guiObject->m_program;

		QMatrix4x4 model;
		model.setToIdentity();
		model.scale(m_zoom);

		program->bind();
		program->setUniformValue("proj", m_projMatrix);
		program->setUniformValue("view", m_camera->getViewMatrix());
		program->setUniformValue("model", model);
		program->setUniformValue("color", 1.0f, 1.0f, 1.0f);

		guiObject->m_vao.bind();
		$->glDrawArrays(GL_LINES, 0, guiObject->verticesCount());
	}
}
