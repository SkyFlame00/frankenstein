#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QMatrix4x4>
#include "Renderable.h"
#include "../common/types.h"
#include "Camera.h"

class ConstructionBlock : public Renderable
{
public:
	enum class ResizeDirection
	{
		LEFT_TOP,
		CENTER_TOP,
		RIGHT_TOP,
		RIGHT_CENTER,
		RIGHT_BOTTOM,
		CENTER_BOTTOM,
		LEFT_BOTTOM,
		LEFT_CENTER
	};

	struct ResizePointsBoundaries
	{
		struct Boundaries
		{
			float horStart;
			float horEnd;
			float verStart;
			float verEnd;
		};

		Boundaries leftTop;
		Boundaries centerTop;
		Boundaries rightTop;
		Boundaries rightCenter;
		Boundaries rightBottom;
		Boundaries centerBottom;
		Boundaries leftBottom;
		Boundaries leftCenter;
	};

	ConstructionBlock(QVector3D startPoint, QVector3D endPoint);
	~ConstructionBlock();
	inline int verticesCount() { return m_linesVerticesCount; }
	void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor) override;
	void createLinesVertices();
	BlockToolState startDrag(Axis axis, QVector2D pos, float zoomFactor);
	void doResizeStep(Axis axis, QVector2D pos, float step);
	void doMoveStep(Axis axis, QVector2D pos, float step);
	void get2DBounds(Axis axis, float* horStart, float* horEnd, float* verStart, float* verEnd);
	void calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps);
	Qt::CursorShape checkHover(float x, float y, Axis axis, float zoomFactor);
	ResizePointsBoundaries* getResizePointsBoundaries(Axis axis, float zoomFactor);

	ResizeDirection m_resizeDirection;
	bool m_isEditingMode = false;

private:
	const float RESIZE_POINT_MARGIN = 15.0f;
	const float RESIZE_POINT_SIZE = 12.0f;

	int m_linesVerticesCount = 12 * 2;
	struct {
		float startX, endX;
		float startY, endY;
		float startZ, endZ;
	} m_boundingBox;
	QList<QVector3D> m_vertices;
	int m_indices[12 * 2];
	float m_linesVertices[12 * 2 * 3];
	QVector3D m_lastPos;
	float m_moveDelta = 0.0f;
	float m_moveDeltaX = 0.0f;
	float m_moveDeltaY = 0.0f;

	QList<QVector3D>* getResizePointsTranslationVectors(Axis axis, float zoomFactor, QVector3D zoomVec);
};
