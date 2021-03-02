#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QMatrix4x4>
#include "Renderable.h"
#include "../common/types.h"
#include "Camera.h"
#include "VertexBufferObject.h"
#include "grid2d/Point.h"
#include "ChangeableRenderable.h"

class ConstructionBlock : public Renderable, public ChangeableRenderable
{
public:
	ConstructionBlock(QVector3D startPoint, QVector3D endPoint);
	~ConstructionBlock();
	inline int verticesCount() { return m_linesVerticesCount; }
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor) override;
	void createLinesVertices();
	void doMoveStep(Axis axis, QVector2D pos, float step);
	void calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps);
	inline QList<QVector3D>* getVertices() { return &m_vertices; }

	bool m_isEditingMode = false;

private:
	VertexBufferObject m_vbo;
	int m_linesVerticesCount = 12 * 2;
	QList<QVector3D> m_vertices;
	int m_indices[12 * 2];
	float m_linesVertices[12 * 2 * 3];
	Point m_resizePoint;
};
