#include "helpers.h"
#include "../editor/Grid2D.h"

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
