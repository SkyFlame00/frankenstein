#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "Renderable.h"
#include <unordered_map>
#include "../common/types.h"
#include "VertexBufferObject.h"
#include <QOpenGLContext>

class Brush : public Renderable
{
public:
	Brush(QList<QVector3D>& cubeVertices, QVector3D color = QVector3D(0.0f, 0.8f, 0.2f));
	~Brush();

	inline int verticesCount() { return m_verticesCount; }
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera);
	void render3D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera);

	QVector3D m_origin;

private:
	class BrushRenderable : public Renderable
	{
	public:
		BrushRenderable(VertexBufferObject* vbo, int verticesCount);

		inline int verticesCount() { return m_verticesCount; }
	private:
		int m_verticesCount;
		VertexBufferObject* m_vbo;
	};

	void makeBufferData();
	void makeLinesData();
	void makeTrianglesData();
	void calcNorm(Types::Polygon* polygon);

	float* m_vertices;
	int m_verticesCount = 0;
	QList<Types::Polygon*> polygons;
	QVector3D m_uniformColor;
	VertexBufferObject m_linesVbo;
	VertexBufferObject m_trianglesVbo;
	BrushRenderable* m_linesRenderable;
	BrushRenderable* m_trianglesRenderable;
	float m_trianglesVerticesCount = 0;
	float m_linesVerticesCount = 0;
	QOpenGLShaderProgram* m_program2D;
	QOpenGLShaderProgram* m_program3D;
};
