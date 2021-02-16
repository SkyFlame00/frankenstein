#pragma once

#include <unordered_map>

enum class ButtonState
{
	NOT_ACTIVE,
	PRESSED,
	RELEASED
};

enum class ButtonDownState
{
	DOWN_NOT_PROCESSED,
	DOWN_PROCESSED,
	RELEASED_NOT_PROCESSED,
	RELEASED_PROCESSED
};

enum class Axis
{
	X, Y, Z
};

enum class SceneZoom
{
	X005, X01, X025, X05, X1, X2, X4, X8, X16, X32, X64, X128
};

const std::unordered_map<SceneZoom, float> SCENE_ZOOM_FACTORS{
	{ SceneZoom::X005, 0.05f },
	{ SceneZoom::X01, 0.1f },
	{ SceneZoom::X025, 0.25f },
	{ SceneZoom::X05, 0.5f },
	{ SceneZoom::X1, 1.0f },
	{ SceneZoom::X2, 2.0f },
	{ SceneZoom::X4, 4.0f },
	{ SceneZoom::X8, 8.0f },
	{ SceneZoom::X16, 16.0f },
	{ SceneZoom::X32, 32.0f },
	{ SceneZoom::X64, 64.0f },
	{ SceneZoom::X128, 128.0f }
};

enum class EdtitorMode
{
	SELECTING_MODE,
	BLOCK_MODE
};

enum class BlockToolState
{
	CREATING,
	READY_TO_EDIT,
	RESIZE,
	MOVE
};
