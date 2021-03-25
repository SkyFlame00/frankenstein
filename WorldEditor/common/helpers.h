#pragma once

#include <cmath>
#include <QVector3D>
#include "types.h"
#include "../editor/Brush.h"

namespace Helpers
{
	float trunc(float num, int digits = 0);
	QVector3D get3DPointFrom2D(Axis axis, float hor, float ver, float thirdAxisVal = 0.0f);
	QVector3D getTop3DPointFrom2D(Axis axis, float hor, float ver);
	QVector2D get2DPointFrom3D(Axis axis, QVector3D point);
	float getRandom();
	BlockToolState mapToBlockToolState(Types::BrushAction state);
	Types::SelectionToolState mapToSelectionToolState(Types::BrushAction state);
	bool lineSegmentPlaneIntersection(QVector3D v1, QVector3D v2, QVector3D p0, QVector3D norm, QVector3D* output);
	float round(float num, int precision = 0);
	float roundIfDelta(float num, float delta = 0.00001f);
	bool areEqual(float x1, float x2, float delta = 0.01f);
	bool areEqual(QVector3D v1, QVector3D v2, float delta = 0.01f);
	void callCalcResizeParams(Brush* brush, ResizeDirection resizeDirection, Axis axis, float stepsX, float stepsY);
}
