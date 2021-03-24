#pragma once

#include "../common/types.h"

class ChangeableRenderable
{
public:
	struct BoundingBox {
		float startX, endX;
		float startY, endY;
		float startZ, endZ;
	};

	virtual Types::BrushAction startDrag(Axis axis, QVector2D pos, float zoomFactor);
	virtual void doResizeStep(Axis axis, QVector2D pos, float step);
	virtual Qt::CursorShape checkHover(float x, float y, Axis axis, float zoomFactor);
	inline BoundingBox& getBoundingBox() { return m_boundingBox; }

protected:
	virtual void get2DBounds(Axis axis, float* horStart, float* horEnd, float* verStart, float* verEnd);
	virtual QList<QVector3D>* getResizePointsTranslationVectors(Axis axis, float zoomFactor, QVector3D zoomVec);
	virtual ResizePointsBoundaries* getResizePointsBoundaries(Axis axis, float zoomFactor);
	virtual void calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps) = 0;

	const float RESIZE_POINT_MARGIN = 15.0f;
	const float RESIZE_POINT_SIZE = 12.0f;

	float m_moveDelta;
	float m_moveDeltaX;
	float m_moveDeltaY;
	QVector2D m_lastPos;
	ResizeDirection m_resizeDirection;
	BoundingBox m_boundingBox;
};
