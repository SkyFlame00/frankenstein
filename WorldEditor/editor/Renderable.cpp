#include "Renderable.h"

void Renderable::render2D(QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	if (!shouldDraw())
	{
		return;
	}

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

	m_vao.bind();
	$->glDrawArrays(getDrawMode(), 0, verticesCount());
}
