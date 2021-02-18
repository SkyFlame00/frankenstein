#include "Renderable.h"
#include "../common/GlobalData.h"

void Renderable::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	if (!shouldDraw())
	{
		return;
	}

	QOpenGLVertexArrayObject* vao;
	auto res1 = GlobalData::openglContexts.find(context);

	if (res1 == GlobalData::openglContexts.end())
	{
		qInfo() << "Renderable::render2D: Corresponding VAO map was not found";
		return;
	}

	auto vaoMap = res1->second;
	auto res2 = vaoMap->find(this);

	if (res2 == vaoMap->end())
	{
		qInfo() << "Renderable::render2D: Corresponding VAO was not found";
		return;
	}

	vao = res2->second;

	QMatrix4x4 model;
	QVector3D translationVec(m_origin.x() * zoomVec.x(), m_origin.y() * zoomVec.y(), m_origin.z() * zoomVec.z());

	model.setToIdentity();

	model.translate(translationVec);
	model.scale(m_scaleVec);

	m_program->bind();
	m_program->setUniformValue("proj", proj);
	m_program->setUniformValue("view", camera.getViewMatrix());
	m_program->setUniformValue("model", model);
	m_program->setUniformValue("color", 1.0f, 1.0f, 1.0f);

	vao->bind();
	$->glDrawArrays(getDrawMode(), 0, verticesCount());
}

void Renderable::render3D(QOpenGLContext* context, QMatrix4x4& proj, Camera& camera)
{
	if (!shouldDraw())
	{
		return;
	}

	QOpenGLVertexArrayObject* vao;
	auto res1 = GlobalData::openglContexts.find(context);

	if (res1 == GlobalData::openglContexts.end())
	{
		qInfo() << "Renderable::render2D: Corresponding VAO map was not found";
		return;
	}

	auto vaoMap = res1->second;
	auto res2 = vaoMap->find(this);

	if (res2 == vaoMap->end())
	{
		qInfo() << "Renderable::render2D: Corresponding VAO was not found";
		return;
	}

	vao = res2->second;

	QMatrix4x4 model;
	model.setToIdentity();

	m_program->bind();
	m_program->setUniformValue("proj", proj);
	m_program->setUniformValue("view", camera.getViewMatrix());
	m_program->setUniformValue("model", model);
	m_program->setUniformValue("color", 0.0f, 1.0f, 1.0f);

	vao->bind();
	$->glDrawArrays(getDrawMode(), 0, verticesCount());
}
