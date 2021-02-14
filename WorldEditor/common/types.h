#pragma once

#include <unordered_map>

enum class ButtonState
{
	NOT_ACTIVE,
	PRESSED,
	RELEASED
};

enum class Axis
{
	X, Y, Z
};

enum class SceneZoom
{
	X001, X005, X01, X02, X04, X06, X08, X1, X2, X4, X8
};

const std::unordered_map<SceneZoom, float> SCENE_ZOOM_FACTORS{
	{ SceneZoom::X001, 0.01f },
	{ SceneZoom::X005, 0.05f },
	{ SceneZoom::X01, 0.1f },
	{ SceneZoom::X02, 0.2f },
	{ SceneZoom::X04, 0.4f },
	{ SceneZoom::X06, 0.6f },
	{ SceneZoom::X08, 0.8f },
	{ SceneZoom::X1, 1.0f },
	{ SceneZoom::X2, 2.0f },
	{ SceneZoom::X4, 4.0f },
	{ SceneZoom::X8, 8.0f },
};
