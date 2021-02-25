#pragma once

#include <cmath>
#include <QVector3D>
#include "types.h"

namespace Helpers
{
	float trunc(float num, int digits = 0);
	QVector3D get3DPointFrom2D(Axis axis, float hor, float ver, float thirdAxisVal = 0.0f);
	float getRandom();
}
