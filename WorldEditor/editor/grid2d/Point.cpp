#include "Point.h"
#include "../ResourceManager.h"

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

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();

	m_vbo.allocate(&vertices[0], m_verticesCount * 3 * sizeof(float));

	$->glEnableVertexAttribArray(0);
	$->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	m_vbo.release();
	m_vao.release();

	m_program = ResourceManager::getProgram("point", "point");
	m_program->bind();

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
