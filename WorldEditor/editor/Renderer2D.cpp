#include "Renderer2D.h"

Renderer2D::Renderer2D(Camera* camera, float width, float height)
	: m_camera(camera), m_width(width), m_height(height)
{
	m_camera->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
	m_camera->updateCameraVectors();
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::setup(float nearPlane, float farPlane)
{
	m_near = nearPlane;
	m_far = farPlane;
}

void Renderer2D::setFrustrum(float width, float height)
{
	m_width = width;
	m_height = height;
	m_projMatrix.setToIdentity();
	m_projMatrix.ortho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, m_near, m_far);
}

void Renderer2D::setZoom(SceneZoom zoom)
{
	m_zoom = zoom;
	float val = SCENE_ZOOM_FACTORS.find(zoom)->second;
	
	m_zoomVec.setX(m_axis == Axis::X ? 0.0f : val);
	m_zoomVec.setY(m_axis == Axis::Y ? 0.0f : val);
	m_zoomVec.setZ(m_axis == Axis::Z ? 0.0f : val);
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
		model.scale(m_zoomVec);

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
		//model.scale(m_zoomVec);

		program->bind();
		program->setUniformValue("proj", m_projMatrix);
		program->setUniformValue("view", m_camera->getViewMatrix());
		program->setUniformValue("model", model);
		program->setUniformValue("color", 1.0f, 1.0f, 1.0f);

		guiObject->m_vao.bind();
		$->glDrawArrays(GL_LINES, 0, guiObject->verticesCount());
	}
}
