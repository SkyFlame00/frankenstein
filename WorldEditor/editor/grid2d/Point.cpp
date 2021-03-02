#include "Point.h"
#include "../ResourceManager.h"
#include "../../common/GlobalData.h"

Point::Point(float size, float x, float y, float z)
	: Renderable(), m_size(size)
{
	m_drawMode = GL_TRIANGLES;
	m_shouldScale = true;
	m_verticesCount = 36;
	float halfLength = size / 2.0f;
	float vertices[]{
		// Face 1
		-halfLength, -halfLength, -halfLength,
		-halfLength, halfLength, -halfLength,
		halfLength, halfLength, -halfLength,

		-halfLength, -halfLength, -halfLength,
		halfLength, halfLength, -halfLength,
		halfLength, -halfLength, -halfLength,

		// Face 2
		-halfLength, -halfLength, halfLength,
		-halfLength, halfLength, halfLength,
		-halfLength, halfLength, -halfLength,

		-halfLength, -halfLength, halfLength,
		-halfLength, halfLength, -halfLength,
		-halfLength, -halfLength, -halfLength,

		// Face 3
		halfLength, -halfLength, halfLength,
		halfLength, halfLength, halfLength,
		-halfLength, halfLength, halfLength,

		halfLength, -halfLength, halfLength,
		-halfLength, halfLength, halfLength,
		-halfLength, -halfLength, halfLength,

		// Face 4
		halfLength, -halfLength, halfLength,
		halfLength, -halfLength, -halfLength,
		halfLength, halfLength, -halfLength,

		halfLength, -halfLength, halfLength,
		halfLength, halfLength, -halfLength,
		halfLength, halfLength, halfLength,

		// Face 5
		-halfLength, halfLength, halfLength,
		halfLength, halfLength, halfLength,
		halfLength, halfLength, -halfLength,

		-halfLength, halfLength, halfLength,
		halfLength, halfLength, -halfLength,
		-halfLength, halfLength, -halfLength,

		// Face 6
		-halfLength, -halfLength, halfLength,
		halfLength, -halfLength, halfLength,
		halfLength, -halfLength, -halfLength,

		-halfLength, -halfLength, halfLength,
		halfLength, -halfLength, -halfLength,
		-halfLength, -halfLength, -halfLength
	};

	m_vbo.addAttribute<float>(3);
	m_vbo.allocate(&vertices[0], m_verticesCount * 3 * sizeof(float));
	createVAO(m_vbo);

	m_program = ResourceManager::getProgram("point", "point");

	m_origin.setX(x);
	m_origin.setY(y);
	m_origin.setZ(z);
	
	m_scaleVec.setX(5.0f);
	m_scaleVec.setY(5.0f);
	m_scaleVec.setZ(5.0f);
}

Point::Point(float size, QVector3D position)
	: Point(size, position.x(), position.y(), position.z())
{
}

Point::~Point()
{
	Renderable::~Renderable();
}

bool Point::hasHover(Axis axis, float x, float y, float zoomFactor)
{
	float originHor;
	float originVer;

	switch (axis)
	{
	case Axis::X:
		originHor = m_origin.z();
		originVer = m_origin.y();
		break;
	case Axis::Y:
		originHor = m_origin.x();
		originVer = m_origin.z();
		break;
	case Axis::Z:
		originHor = m_origin.x();
		originVer = m_origin.y();
	}

	float size = m_size * zoomFactor;
	QVector2D leftTop(originHor - size / 2, originVer + size / 2);
	QVector2D rightTop(originHor + size / 2, originVer + size / 2);
	QVector2D leftBottom(originHor - size / 2, originVer - size / 2);
	QVector2D rightBottom(originHor + size / 2, originVer - size / 2);

	if (leftTop.x() <= x && y <= leftTop.y() &&
		x <= rightTop.x() && y <= rightTop.y() &&
		leftBottom.x() <= x && leftBottom.y() <= y &&
		x <= rightBottom.x() && rightBottom.y() <= y)
	{
		return true;
	}

	return false;
}

void Point::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);

	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(m_origin * zoomVec);

	if (m_enableScale)
		model.scale(m_scaleVec);
	
	useContext(context);
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("proj", proj));
	GLCall(m_program->setUniformValue("view", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("model", model));
	GLCall(m_program->setUniformValue("color", 1.0f, 1.0f, 1.0f));

	vao->bind();
	GLCall($->glDrawArrays(getDrawMode(), 0, verticesCount()));
}
