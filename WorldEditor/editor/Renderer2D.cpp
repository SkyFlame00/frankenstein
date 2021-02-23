#include "Renderer2D.h"
#include "../common/GlobalData.h"
#include "Grid2D.h"

Renderer2D::Renderer2D(Camera* camera, float width, float height)
	: m_camera(camera), m_width(width), m_height(height)
{
	m_camera->updateCameraVectors();
	setZoom(m_zoom);
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::setup(Axis axis, float nearPlane, float farPlane)
{
	switch (axis)
	{
	case Axis::X:
		// We look from the negative ray towards the positive one so that Z axis is negative on the left and positive on the right
		m_camera->setPosition(QVector3D(-1.0f, 0.0f, 0.0f));
		m_camera->setFrontVec(QVector3D(1.0f, 0.0f, 0.0f));
		m_camera->setWorldUpVec(QVector3D(0.0, 1.0f, 0.0f));
		break;
	case Axis::Y:
		// We look from the bottom to the top so that X axis is negative on the left and positive on the right
		m_camera->setPosition(QVector3D(0.0f, -1.0f, 0.0f));
		m_camera->setFrontVec(QVector3D(0.0f, 1.0f, 0.0f));
		m_camera->setWorldUpVec(QVector3D(0.0, 0.0f, 1.0f));
		break;
	case Axis::Z:
		m_camera->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
		m_camera->setFrontVec(QVector3D(0.0f, 0.0f, -1.0f));
		m_camera->setWorldUpVec(QVector3D(0.0, 1.0f, 0.0f));
	}

	m_near = nearPlane;
	m_far = farPlane;
	m_camera->updateCameraVectors(false);
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
	float val = SCENE_ZOOM_FACTORS.find(zoom)->second;
	m_zoom = zoom;
	m_zoomVec.setX(m_axis == Axis::X ? 0.0f : val);
	m_zoomVec.setY(m_axis == Axis::Y ? 0.0f : val);
	m_zoomVec.setZ(m_axis == Axis::Z ? 0.0f : val);
}

void Renderer2D::render(QOpenGLContext* context, Grid2D& grid2D, QList<Brush*>& objects, QList<Renderable*>& guiObjects, float factor)
{
	auto makeCurrent = [&]() { context->makeCurrent(context->surface()); };

	QMatrix4x4 model;
	model.setToIdentity();
	
	GLCall($->glClearColor(0.0, 0.0, 0.0, 1.0));
	GLCall($->glClear(GL_COLOR_BUFFER_BIT));
	
	{
		makeCurrent();
		auto vao = GlobalData::getRenderableVAO(*context, grid2D);
		auto program = grid2D.m_program;
		
		GLCall(program->bind());
		GLCall(program->setUniformValue("proj", m_projMatrix));
		GLCall(program->setUniformValue("view", m_camera->getViewMatrix()));
		GLCall(program->setUniformValue("model", model));
		GLCall(program->setUniformValue("color", 1.0f, 1.0f, 1.0f));

		vao->bind();
		GLCall($->glDrawArrays(grid2D.getDrawMode(), 0, grid2D.verticesCount()));
	}

	for (auto& guiObject : guiObjects)
	{
		guiObject->render2D(context, m_projMatrix, m_zoomVec, *m_camera, m_axis, factor);
	}

	for (auto& object : objects)
	{
		makeCurrent();

		auto program = object->m_program;
		QMatrix4x4 model;
		model.setToIdentity();
		//model.scale(m_zoomVec);

		GLCall(program->bind());
		GLCall(program->setUniformValue("proj", m_projMatrix));
		GLCall(program->setUniformValue("view", m_camera->getViewMatrix()));
		GLCall(program->setUniformValue("model", model));
		GLCall(program->setUniformValue("color", 1.0f, 1.0f, 1.0f));
		
		object->m_vao.bind();
		GLCall($->glDrawArrays(GL_LINES, 0, object->verticesCount()));
	}
}
