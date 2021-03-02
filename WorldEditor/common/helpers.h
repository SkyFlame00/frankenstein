#pragma once

#include <cmath>
#include <QVector3D>
#include "types.h"

namespace Helpers
{
	float trunc(float num, int digits = 0);
	QVector3D get3DPointFrom2D(Axis axis, float hor, float ver, float thirdAxisVal = 0.0f);
	QVector3D getTop3DPointFrom2D(Axis axis, float hor, float ver);
	QVector2D get2DPointFrom3D(Axis axis, QVector3D point);
	float getRandom();
	BlockToolState mapToBlockToolState(Types::BrushAction state);
	Types::SelectionToolState mapToSelectionToolState(Types::BrushAction state);
}
