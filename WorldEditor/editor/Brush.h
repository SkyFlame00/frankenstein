#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "Renderable.h"
#include <unordered_map>
#include "../common/types.h"
#include "VertexBufferObject.h"
#include <QOpenGLContext>
#include "grid2d/Point.h"
#include "ChangeableRenderable.h"
#include "../common/cgal_bindings.h"

class Brush : public Renderable, public ChangeableRenderable
{
public:
	Brush(QList<QVector3D>& cubeVertices, QVector3D color = QVector3D(0.0f, 0.8f, 0.2f));
	Brush(Polyhedron_3& polyhedron, QVector3D oldOrigin, QVector3D color = QVector3D(0.0f, 0.8f, 0.2f));
	~Brush();

	void setup();
	inline int verticesCount() { return m_verticesCount; }
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor);
	void render3D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera);
	inline QList<Types::Polygon*>& getPolygons() { return m_polygons; }
	inline QList<Types::Edge>& getUniqueEdges() { return m_uniqueEdges; }
	inline QList<QVector3D*>& getUniqueVertices() { return m_uniqueVertices; }
	inline QVector3D getUniformColor() { return m_uniformColor; }
	void writeSelectionBuffer(QOpenGLContext* context, float renderId, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera);
	void doMoveStep(Axis axis, QVector2D pos, float step);

	QVector3D m_origin;
	bool m_selected = false;
	QVector3D m_selectionColor;
	Brush* m_clippedBrush = nullptr;
	Brush* m_remainingBrush = nullptr;
	bool m_beingClipped = false;
	bool m_beingCut = false;
	bool m_isInClippingMode = false;

private:
	class BrushRenderable : public Renderable
	{
	public:
		BrushRenderable(VertexBufferObject* vbo, int verticesCount);
		inline int verticesCount() { return m_verticesCount; }
		inline void setVerticesCount(int n) { m_verticesCount = n; }

	private:
		int m_verticesCount;
		VertexBufferObject* m_vbo;
	};

	void makeLinesBufferData();
	void makeTrianglesBufferData();
	void makeTrianglesLinesBufferData();
	void calcNorm(Types::Polygon* polygon);
	void calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps) override;
	QVector2D get2DOrigin(Axis axis);

	QList<QVector3D*> m_uniqueVertices;
	QList<Types::Edge> m_uniqueEdges;
	int m_verticesCount = 0;
	QList<Types::Polygon*> m_polygons;
	QVector3D m_uniformColor;
	VertexBufferObject m_linesVbo;
	VertexBufferObject m_trianglesVbo;
	VertexBufferObject m_trianglesLinesVbo;
	BrushRenderable* m_linesRenderable;
	BrushRenderable* m_trianglesRenderable;
	BrushRenderable* m_trianglesLinesRenderable;
	float m_trianglesVerticesCount = 0;
	float m_linesVerticesCount = 0;
	float m_trianglesLinesVerticesCount = 0;
	QOpenGLShaderProgram* m_program2D;
	QOpenGLShaderProgram* m_program3D;
	QOpenGLShaderProgram* m_programSelection;
	Point m_resizePoint;
	float* m_linesVerticesX;
	float* m_linesVerticesY;
	float* m_linesVerticesZ;
	VertexBufferObject m_linesVerticesX_vbo;
	VertexBufferObject m_linesVerticesY_vbo;
	VertexBufferObject m_linesVerticesZ_vbo;
	BrushRenderable* m_linesVerticesX_renderable;
	BrushRenderable* m_linesVerticesY_renderable;
	BrushRenderable* m_linesVerticesZ_renderable;
	int m_bboxLinesVerticesCount;
};
