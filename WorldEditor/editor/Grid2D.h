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

	const float HALF_LENGTH = 4096.0f;
	const float MIN_UNIT = 1.0f;
	const std::unordered_map<UnitSize, std::unordered_map<SceneZoom, UnitSize>> SIZES {
		{ UnitSize::X1,
			{
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X025, UnitSize::X8 },
				{ SceneZoom::X05, UnitSize::X4 },
				{ SceneZoom::X1, UnitSize::X4 },
				{ SceneZoom::X2, UnitSize::X4 },
				{ SceneZoom::X4, UnitSize::X4 },
				{ SceneZoom::X8, UnitSize::X2 },
				{ SceneZoom::X16, UnitSize::X1 },
				{ SceneZoom::X32, UnitSize::X1 },
				{ SceneZoom::X64, UnitSize::X1 },
				{ SceneZoom::X128, UnitSize::X1 },
			}
		},
		{ UnitSize::X2,
			{
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X025, UnitSize::X8 },
				{ SceneZoom::X05, UnitSize::X4 },
				{ SceneZoom::X1, UnitSize::X4 },
				{ SceneZoom::X2, UnitSize::X4 },
				{ SceneZoom::X4, UnitSize::X4 },
				{ SceneZoom::X8, UnitSize::X2 },
				{ SceneZoom::X16, UnitSize::X2 },
				{ SceneZoom::X32, UnitSize::X2 },
				{ SceneZoom::X64, UnitSize::X2 },
				{ SceneZoom::X128, UnitSize::X2 },
			}
		},
		{ UnitSize::X4,
			{
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X025, UnitSize::X8 },
				{ SceneZoom::X05, UnitSize::X4 },
				{ SceneZoom::X1, UnitSize::X4 },
				{ SceneZoom::X2, UnitSize::X4 },
				{ SceneZoom::X4, UnitSize::X4 },
				{ SceneZoom::X8, UnitSize::X4 },
				{ SceneZoom::X16, UnitSize::X4 },
				{ SceneZoom::X32, UnitSize::X4 },
				{ SceneZoom::X64, UnitSize::X4 },
				{ SceneZoom::X128, UnitSize::X4 },
			}
		},
		{ UnitSize::X8,
			{
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X025, UnitSize::X8 },
				{ SceneZoom::X05, UnitSize::X8 },
				{ SceneZoom::X1, UnitSize::X8 },
				{ SceneZoom::X2, UnitSize::X8 },
				{ SceneZoom::X4, UnitSize::X8 },
				{ SceneZoom::X8, UnitSize::X8 },
				{ SceneZoom::X16, UnitSize::X8 },
				{ SceneZoom::X32, UnitSize::X8 },
				{ SceneZoom::X64, UnitSize::X8 },
				{ SceneZoom::X128, UnitSize::X8 },
			}
		},
		{ UnitSize::X16,
			{
				{ SceneZoom::X005, UnitSize::X16 },
				{ SceneZoom::X01, UnitSize::X16 },
				{ SceneZoom::X025, UnitSize::X16 },
				{ SceneZoom::X05, UnitSize::X16 },
				{ SceneZoom::X1, UnitSize::X16 },
				{ SceneZoom::X2, UnitSize::X16 },
				{ SceneZoom::X4, UnitSize::X16 },
				{ SceneZoom::X8, UnitSize::X16 },
				{ SceneZoom::X16, UnitSize::X16 },
				{ SceneZoom::X32, UnitSize::X16 },
				{ SceneZoom::X64, UnitSize::X16 },
				{ SceneZoom::X128, UnitSize::X16 },
			}
		},
		{ UnitSize::X32,
			{
				{ SceneZoom::X005, UnitSize::X32 },
				{ SceneZoom::X01, UnitSize::X32 },
				{ SceneZoom::X025, UnitSize::X32 },
				{ SceneZoom::X05, UnitSize::X32 },
				{ SceneZoom::X1, UnitSize::X32 },
				{ SceneZoom::X2, UnitSize::X32 },
				{ SceneZoom::X4, UnitSize::X32 },
				{ SceneZoom::X8, UnitSize::X32 },
				{ SceneZoom::X16, UnitSize::X32 },
				{ SceneZoom::X32, UnitSize::X32 },
				{ SceneZoom::X64, UnitSize::X32 },
				{ SceneZoom::X128, UnitSize::X32 },
			}
		},
	};

	Axis m_axis;
	SceneZoom m_zoom;

	Grid2D(Axis axis, SceneZoom zoom);
	~Grid2D();

	void increaseScale();
	void decreaseScale();
	inline int verticesCount() { return m_verticesCount; }
	void updateView(QVector3D position, float width, float height);
	inline bool shouldDraw() { return m_shouldDraw; }
	inline UnitSize getStep() { return m_step; }

private:
	int m_verticesCount = 0;
	int m_gridLeft = -HALF_LENGTH;
	int m_gridRight = HALF_LENGTH;
	int m_gridTop = HALF_LENGTH;
	int m_gridBottom = -HALF_LENGTH;
	UnitSize m_step = UnitSize::X1;
	bool m_shouldDraw = true;

	void addVertex(float *vertices, int i, float horPos, float verPos);
};
