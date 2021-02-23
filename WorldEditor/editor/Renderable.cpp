#include "Renderable.h"
#include "../common/GlobalData.h"
#include "../gui/Debug.h"

Renderable::~Renderable()
{
	for (auto& [context, map] : GlobalData::openglContexts)
	{
		context->makeCurrent(context->surface());
		auto vaoRes = map->find(this);
				
		if (vaoRes != map->end())
		{
			auto vao = vaoRes->second;
			delete vao;
		}
		
		map->erase(this);
	}
}

void Renderable::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	if (!shouldDraw())
	{
		return;
	}

	auto vao = GlobalData::getRenderableVAO(*context, *this);

	QMatrix4x4 model;
	QVector3D translationVec(m_origin.x() * zoomVec.x(), m_origin.y() * zoomVec.y(), m_origin.z() * zoomVec.z());

	model.setToIdentity();

	model.translate(translationVec);
	model.scale(m_scaleVec);

	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("proj", proj));
	GLCall(m_program->setUniformValue("view", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("model", model));
	GLCall(m_program->setUniformValue("color", 1.0f, 1.0f, 1.0f));

	vao->bind();
	GLCall($->glDrawArrays(getDrawMode(), 0, verticesCount()));
}

void Renderable::createVAO(VertexBufferObject& vbo)
{
	for (const auto& [context, map] : GlobalData::openglContexts)
	{
		context->makeCurrent(context->surface());

		QOpenGLVertexArrayObject* vao = new QOpenGLVertexArrayObject;
		vao->create();
		vao->bind();
		vbo.apply();

		auto res = GlobalData::openglContexts.find(context);

		if (res == GlobalData::openglContexts.end())
		{
			qInfo() << "ConstructionBlock::ConstructionBlock: Corresponding context was not found";
		}

		res->second->insert(std::make_pair(this, vao));
	}
}

void Renderable::render3D(QOpenGLContext* context, QMatrix4x4& proj, Camera& camera)
{
	if (!shouldDraw())
	{
		return;
	}

	auto vao = GlobalData::getRenderableVAO(*context, *this);

	QMatrix4x4 model;
	model.setToIdentity();

	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("proj", proj));
	GLCall(m_program->setUniformValue("view", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("model", model));
	GLCall(m_program->setUniformValue("color", 0.0f, 1.0f, 1.0f));

	vao->bind();
	GLCall($->glDrawArrays(getDrawMode(), 0, verticesCount()));
}

void Renderable::bindVAO(QOpenGLContext* context)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);
	vao->bind();
}
