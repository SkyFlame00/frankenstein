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
