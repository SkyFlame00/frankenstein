#include "helpers.h"

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
