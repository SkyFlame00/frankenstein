#pragma once

#include <QVector3D>
#include "../Renderable.h"
#include "../VertexBufferObject.h"
#include <QOpenGLContext>

class Point : public Renderable
{
public:
	Point(float size, float x, float y, float z);
	Point(float size, QVector3D position);
	~Point();

	inline int verticesCount() { return m_verticesCount; };

	float m_size = 0.0f;

private:
	int m_verticesCount = 0;
	VertexBufferObject m_vbo;
};
