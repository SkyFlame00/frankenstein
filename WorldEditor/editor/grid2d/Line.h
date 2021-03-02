#pragma once

#include "../Renderable.h"
#include "../VertexBufferObject.h"

class Line : public Renderable
{
public:
	Line();
	int verticesCount() { return m_verticesCount; };
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor) override;

	QVector3D m_pt1Origin;
	QVector3D m_pt2Origin;

private:
	float m_vertices[14]{
		// Position       // Color          // Index
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	int m_verticesCount = 2;
	VertexBufferObject m_vbo;
};
