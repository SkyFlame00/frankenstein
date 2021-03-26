#pragma once

#include <Qt>
#include <unordered_map>
#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <qopengl.h>
#include "../editor/Texture.h"
#include <nlohmann/json.hpp>

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
	X,
	Y,
	Z,
	NONE
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
	BLOCK_MODE,
	CAMERA_MODE,
	CLIPPING_MODE,
	TEXTURE_TOOL
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
		QList<QVector3D*> vertices;
		QList<QVector3D*> beforeSorted;
		std::unordered_map<QVector3D*, QVector2D> verticesMap; // vertices to tex coordinates mapping
		QVector3D norm;
		QList<Triangle> triangles;
		QList<Edge> edges;
		QList<Edge> trianglesLines;
		bool isUsingColor;
		QVector3D color;
		GLuint textureId;
		int begin = 0;
		QVector2D scale;
		QVector2D shift;
		float minX;
		float minY;
		float arccos;
		int textureWidth;
		int textureHeight;
		bool isSelected = false;
		int rotationAngle = 0;
		QVector3D center;
	};

	enum class SelectionToolState
	{
		READY_TO_SELECT,
		RESIZE,
		MOVE
	};

	enum class ClippingToolState
	{
		READY,
		POINT_PLACED,
		INTERSECTION,
		POINT_DISPLACEMENT
	};

	enum class BrushAction
	{
		READY,
		RESIZE,
		MOVE
	};

	typedef void (*undo_t)(void* data);
	typedef void (*redo_t)(void* data);
	typedef void (*cleanup_t)(void* data);

	struct Vec2JSON
	{
		float x;
		float y;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vec2JSON, x, y)
	};

	struct Vec3JSON
	{
		float x;
		float y;
		float z;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vec3JSON, x, y, z)
	};

	struct Color3JSON
	{
		float r;
		float g;
		float b;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Color3JSON, r, g, b)
	};

	struct EdgeJSON
	{
		int v0;
		int v1;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(EdgeJSON, v0, v1)
	};

	struct TexCoordsJSON
	{
		int vertex_index;
		float u;
		float v;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(TexCoordsJSON, vertex_index, u, v)
	};

	struct TriangleJSON
	{
		int v0;
		int v1;
		int v2;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(TriangleJSON, v0, v1, v2)
	};

	struct PolygonJSON
	{
		Vec3JSON norm;
		Vec2JSON shift;
		Vec2JSON scale;
		std::string texture_path;
		bool is_texture_missing;
		std::list<TexCoordsJSON> tex_coords;
		std::list<TriangleJSON> triangles;
		std::list<int> vertices;
		std::list<EdgeJSON> edges;
		std::list<EdgeJSON> trianglesLines;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PolygonJSON, norm, shift, scale, texture_path, is_texture_missing,
			tex_coords, triangles, vertices, edges, trianglesLines)
	};

	struct BrushJSON
	{
		Vec3JSON origin;
		Color3JSON color;
		std::string default_texture_path;
		bool is_default_texture_missing;
		std::string type;
		std::list<Vec3JSON> unique_vertices;
		std::list<EdgeJSON> unique_edges;
		std::list<PolygonJSON> polygons;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(BrushJSON, origin, color, default_texture_path,
			is_default_texture_missing, type, unique_vertices, unique_edges, polygons)
	};

	struct MapJSON
	{
		std::list<BrushJSON> objects;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapJSON, objects)
	};
}

namespace TextureBrowser
{
	enum class NodeType
	{
		TEXTURE,
		DIRECTORY
	};

	typedef struct Node
	{
		NodeType type;
		QString path;
		QString name;
		Node* parent = nullptr;
		Texture texture;
		QList<Node*> children;
	};
}

namespace Configuration
{
	struct TextureSettingsJSON
	{
		std::string texture_path;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(TextureSettingsJSON, texture_path)
	};

	struct ConfigurationJSON
	{
		TextureSettingsJSON texture_settings;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConfigurationJSON, texture_settings)
	};
}
