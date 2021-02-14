#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "Renderable.h"
#include "../common/types.h"

class Grid2D : public Renderable
{
public:
	enum class UnitSize
	{
		X1 = 1, X2 = 2, X4 = 4, X8 = 8, X16 = 16, X32 = 32
	};

	const float HALF_CUBE = 4096.0f;
	const float MIN_UNIT = 1.0f;
	
	const std::unordered_map<UnitSize, std::unordered_map<SceneZoom, UnitSize>> SIZES {
		{ UnitSize::X1,
			{
				{ SceneZoom::X001, UnitSize::X32 },
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X02, UnitSize::X16 },
				{ SceneZoom::X04, UnitSize::X8 },
				{ SceneZoom::X06, UnitSize::X4 },
				{ SceneZoom::X08, UnitSize::X2 },
				{ SceneZoom::X1, UnitSize::X1 },
				{ SceneZoom::X2, UnitSize::X1 },
				{ SceneZoom::X4, UnitSize::X1 },
				{ SceneZoom::X8, UnitSize::X1 },
			}
		}
	};

	Axis m_axis;
	SceneZoom m_zoom = SceneZoom::X1;

	Grid2D(Axis axis);
	~Grid2D();

	void increaseScale();
	void decreaseScale();
	inline int verticesCount() { return m_verticesCount; }
	void updateView(QVector3D position, float width, float height);
	inline bool ShouldDraw() { return m_shouldDraw; }

private:
	int m_verticesCount = 0;
	int m_gridLeft = -HALF_CUBE;
	int m_gridRight = HALF_CUBE;
	int m_gridTop = HALF_CUBE;
	int m_gridBottom = -HALF_CUBE;
	UnitSize m_step = UnitSize::X1;
	bool m_shouldDraw = true;

	void addVertex(float *vertices, int i, float horPos, float verPos);
};
