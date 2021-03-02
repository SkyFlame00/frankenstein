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
	bool hasHover(Axis axis, float x, float y, float zoomFactor);
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor);

	float m_size = 0.0f;
	bool m_enableScale = true;

private:
	int m_verticesCount = 0;
	VertexBufferObject m_vbo;
};
