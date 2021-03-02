#pragma once

#include <Qt>
#include <unordered_map>
#include <QVector3D>
#include <QVector2D>
#include <QList>

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

enum class EditorMode
{
	SELECTION_MODE,
	BLOCK_MODE
};

enum class BlockToolState
{
	CREATING,
	READY_TO_EDIT,
	RESIZE,
	MOVE
};

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

namespace Types
{
	struct Triangle
	{
		QVector3D* v0, * v1, * v2;
	};

	struct Edge
	{
		QVector3D* v0, * v1;
	};

	struct Polygon
	{
		float a, b, c, d;
		int count = 0;
		std::unordered_map<QVector3D*, QVector2D> verticesMap; // vertices to tex coordinates mapping
		QVector3D norm;
		QList<Triangle> triangles;
		QList< QPair<QVector3D, int> > indexedVertices;
		QList<Edge> allEdges;
		QList<Edge> borderEdges;
	};

	enum class SelectionToolState
	{
		READY_TO_SELECT,
		RESIZE,
		MOVE
	};

	enum class BrushAction
	{
		READY,
		RESIZE,
		MOVE
	};
}
