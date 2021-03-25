#include "Line.h"
#include "../ResourceManager.h"
#include "../../common/GlobalData.h"

Line::Line()
	: Renderable()
{
	m_vbo.allocate(m_vertices, m_verticesCount * 7 * sizeof(float));
	m_vbo.addAttribute<float>(3); // Position
	m_vbo.addAttribute<float>(3); // Color
	m_vbo.addAttribute<float>(1); // Index
	createVAO(m_vbo);

	m_program = ResourceManager::getProgram("line_2d", "line_2d");
}

void Line::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);
	QMatrix4x4 pt1Model, pt2Model;

	pt1Model.setToIdentity();
	pt1Model.scale(zoomVec);
	pt1Model.translate(m_pt1Origin);

	pt2Model.setToIdentity();
	pt2Model.scale(zoomVec);
	pt2Model.translate(m_pt2Origin);

	useContext(context);
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("u_Proj", proj));
	GLCall(m_program->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("u_Pt1Model", pt1Model));
	GLCall(m_program->setUniformValue("u_Pt2Model", pt2Model));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_LINES, 0, m_verticesCount));
}

void Line::render3D(QOpenGLContext* context, QMatrix4x4& proj, const QVector3D& scaleVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);
	QMatrix4x4 pt1Model, pt2Model;

	pt1Model.setToIdentity();
	pt1Model.translate(m_pt1Origin * scaleVec);

	pt2Model.setToIdentity();
	pt2Model.translate(m_pt2Origin * scaleVec);

	useContext(context);
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("u_Proj", proj));
	GLCall(m_program->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("u_Pt1Model", pt1Model));
	GLCall(m_program->setUniformValue("u_Pt2Model", pt2Model));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_LINES, 0, m_verticesCount));
}
