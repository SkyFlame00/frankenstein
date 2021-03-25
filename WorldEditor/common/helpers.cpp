#include "helpers.h"
#include "../editor/Grid2D.h"
#include <cmath>

float Helpers::trunc(float num, int digits)
{
	float factor = std::pow(10, digits);
	num = std::trunc(num * factor);
	return num / factor;
}

QVector3D Helpers::get3DPointFrom2D(Axis axis, float hor, float ver, float thirdAxisVal)
{
	switch (axis)
	{
	case Axis::X:
		return QVector3D(thirdAxisVal, ver, hor);
	case Axis::Y:
		return QVector3D(hor, thirdAxisVal, ver);
	case Axis::Z:
		return QVector3D(hor, ver, thirdAxisVal);
	}
}

QVector3D Helpers::getTop3DPointFrom2D(Axis axis, float hor, float ver)
{
	switch (axis)
	{
	case Axis::X:
		return QVector3D(-Grid2D::HALF_LENGTH, ver, hor);
	case Axis::Y:
		return QVector3D(hor, -Grid2D::HALF_LENGTH, ver);
	case Axis::Z:
		return QVector3D(hor, ver, Grid2D::HALF_LENGTH);
	}
}

QVector2D Helpers::get2DPointFrom3D(Axis axis, QVector3D p)
{
	switch (axis)
	{
	case Axis::X:
		return QVector2D(p.z(), p.y());
	case Axis::Y:
		return QVector2D(p.x(), p.z());
	case Axis::Z:
		return QVector2D(p.x(), p.y());
	}
}

float Helpers::getRandom()
{
	return (float)std::rand() / (float)RAND_MAX;
}

BlockToolState Helpers::mapToBlockToolState(Types::BrushAction state)
{
	switch (state)
	{
	case Types::BrushAction::READY:
		return BlockToolState::READY_TO_EDIT;
	case Types::BrushAction::MOVE:
		return BlockToolState::MOVE;
	case Types::BrushAction::RESIZE:
		return BlockToolState::RESIZE;
	}
}

Types::SelectionToolState Helpers::mapToSelectionToolState(Types::BrushAction state)
{
	switch (state)
	{
	case Types::BrushAction::READY:
		return Types::SelectionToolState::READY_TO_SELECT;
	case Types::BrushAction::MOVE:
		return Types::SelectionToolState::MOVE;
	case Types::BrushAction::RESIZE:
		return Types::SelectionToolState::RESIZE;
	}
}

bool Helpers::lineSegmentPlaneIntersection(QVector3D v1, QVector3D v2, QVector3D p0, QVector3D norm, QVector3D* output)
{
	float dist1 = QVector3D::dotProduct(norm, v1 - p0);
	float dist2 = QVector3D::dotProduct(norm, v2 - p0);

	if (dist1 * dist2 > 0.0f)
	{
		/* No intersection */
		return false;
	}

	QVector3D x = (v2 - v1) / (v2 - v1).length();

	float cos = QVector3D::dotProduct(norm, x);

	if (cos == 0.0f)
	{
		/* Line segment and plane are parallel */
		return false;
	}

	*output = v2 - x * (dist2 / cos);

	return true;
}

float Helpers::roundIfDelta(float num, float delta)
{
	float round = std::round(num);

	if (std::abs(round - num) <= delta)
		return round;

	return num;
}

float Helpers::round(float num, int precision)
{
	float factor = std::pow(10, precision);
	return std::round(num * factor) / factor;
}

bool Helpers::areEqual(float x1, float x2, float delta)
{
	return std::abs(x1 - x2) <= delta;
}

bool Helpers::areEqual(QVector3D v1, QVector3D v2, float delta)
{
	return std::abs(v1.x() - v2.x()) <= delta &&
		std::abs(v1.y() - v2.y()) <= delta &&
		std::abs(v1.z() - v2.z()) <= delta;
}

void Helpers::callCalcResizeParams(Brush* brush, ResizeDirection resizeDirection, Axis axis, float stepsX, float stepsY)
{
	if (resizeDirection == ResizeDirection::LEFT_TOP)
	{
		brush->calcResize(axis, true, true, stepsX);
		brush->calcResize(axis, false, false, stepsY);
	}
	else if (resizeDirection == ResizeDirection::CENTER_TOP)
	{
		brush->calcResize(axis, false, false, stepsY);
	}
	else if (resizeDirection == ResizeDirection::RIGHT_TOP)
	{
		brush->calcResize(axis, true, false, stepsX);
		brush->calcResize(axis, false, false, stepsY);
	}
	else if (resizeDirection == ResizeDirection::RIGHT_CENTER)
	{
		brush->calcResize(axis, true, false, stepsX);
	}
	else if (resizeDirection == ResizeDirection::RIGHT_BOTTOM)
	{
		brush->calcResize(axis, true, false, stepsX);
		brush->calcResize(axis, false, true, stepsY);
	}
	else if (resizeDirection == ResizeDirection::CENTER_BOTTOM)
	{
		brush->calcResize(axis, false, true, stepsY);
	}
	else if (resizeDirection == ResizeDirection::LEFT_BOTTOM)
	{
		brush->calcResize(axis, true, true, stepsX);
		brush->calcResize(axis, false, true, stepsY);
	}
	else if (resizeDirection == ResizeDirection::LEFT_CENTER)
	{
		brush->calcResize(axis, true, true, stepsX);
	}
}
