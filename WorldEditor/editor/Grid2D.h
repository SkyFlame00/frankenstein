#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <unordered_map>
#include "Renderable.h"
#include "../common/types.h"

class Grid2D : public Renderable
{
public:
	enum class UnitSize
	{
		X1 = 1, X2 = 2, X4 = 4, X8 = 8, X16 = 16, X32 = 32
	};
	
	enum class Zoom
	{
		X001, X005, X01, X02, X04, X06, X08, X1, X2, X4, X8
	};

	const float HALF_CUBE = 4096.0f;
	const float MIN_UNIT = 1.0f;
	const std::unordered_map<Zoom, float> ZOOM_FACTORS{
		{ Zoom::X001, 0.01f },
		{ Zoom::X005, 0.05f },
		{ Zoom::X01, 0.1f },
		{ Zoom::X02, 0.2f },
		{ Zoom::X04, 0.4f },
		{ Zoom::X06, 0.6f },
		{ Zoom::X08, 0.8f },
		{ Zoom::X1, 1.0f },
		{ Zoom::X2, 2.0f },
		{ Zoom::X4, 4.0f },
		{ Zoom::X8, 8.0f },
	};
	const std::unordered_map<UnitSize, std::unordered_map<Zoom, UnitSize>> SIZES {
		{ UnitSize::X1,
			{
				{ Zoom::X001, UnitSize::X32 },
				{ Zoom::X005, UnitSize::X16 },
				{ Zoom::X01, UnitSize::X16 },
				{ Zoom::X02, UnitSize::X16 },
				{ Zoom::X04, UnitSize::X8 },
				{ Zoom::X06, UnitSize::X4 },
				{ Zoom::X08, UnitSize::X2 },
				{ Zoom::X1, UnitSize::X1 },
				{ Zoom::X2, UnitSize::X1 },
				{ Zoom::X4, UnitSize::X1 },
				{ Zoom::X8, UnitSize::X1 },
			}
		}
	};

	Axis m_axis;
	int m_size = 0;
	Zoom m_zoom = Zoom::X1;

	Grid2D(Axis axis);
	~Grid2D();

	void increaseScale();
	void decreaseScale();
	void zoomIn();
	void zoomOut();
	inline int verticesCount() { return m_verticesCount; }
	void updateView(QVector3D position, float width, float height);
	inline bool ShouldDraw() { return m_shouldDraw; }

private:
	QVector3D* m_vertices;
	int m_verticesCount = 0;
	int m_gridLeft = -HALF_CUBE;
	int m_gridRight = HALF_CUBE;
	int m_gridTop = HALF_CUBE;
	int m_gridBottom = -HALF_CUBE;
	UnitSize m_step = UnitSize::X1;
	bool m_shouldDraw = true;
	
	void setScale(UnitSize size);
	void setVertices();
	void addVertex(float *vertices, int i, float horPos, float verPos);
};
