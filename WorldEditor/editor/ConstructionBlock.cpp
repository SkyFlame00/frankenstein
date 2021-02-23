#include "ConstructionBlock.h"
#include "ResourceManager.h"
#include <algorithm>
#include <math.h>
#include "grid2d/Point.h"
#include "../common/helpers.h"
#include "../common/GlobalData.h"
#include "GL.h"
#include <memory>

ConstructionBlock::ConstructionBlock(QVector3D startPoint, QVector3D endPoint)
{
	m_boundingBox.startX = std::min(startPoint.x(), endPoint.x());
	m_boundingBox.endX = std::max(startPoint.x(), endPoint.x());
	m_boundingBox.startY = std::min(startPoint.y(), endPoint.y());
	m_boundingBox.endY = std::max(startPoint.y(), endPoint.y());
	m_boundingBox.startZ = std::min(startPoint.z(), endPoint.z());
	m_boundingBox.endZ = std::max(startPoint.z(), endPoint.z());

	m_vertices.push_back(startPoint);
	m_vertices.push_back(QVector3D(endPoint.x(), startPoint.y(), startPoint.z()));
	m_vertices.push_back(QVector3D(endPoint.x(), startPoint.y(), endPoint.z()));
	m_vertices.push_back(QVector3D(startPoint.x(), startPoint.y(), endPoint.z()));
	m_vertices.push_back(endPoint);
	m_vertices.push_back(QVector3D(startPoint.x(), endPoint.y(), endPoint.z()));
	m_vertices.push_back(QVector3D(startPoint.x(), endPoint.y(), startPoint.z()));
	m_vertices.push_back(QVector3D(endPoint.x(), endPoint.y(), startPoint.z()));

	m_indices[0] = 0; // Line 1
	m_indices[1] = 1;
	m_indices[2] = 1; // Line 2
	m_indices[3] = 2;
	m_indices[4] = 2; // Line 3
	m_indices[5] = 3;
	m_indices[6] = 3; // Line 4
	m_indices[7] = 0;
	m_indices[8] = 4; // Line 5
	m_indices[9] = 5;
	m_indices[10] = 5; // Line 6
	m_indices[11] = 6;
	m_indices[12] = 6; // Line 7
	m_indices[13] = 7;
	m_indices[14] = 7; // Line 8
	m_indices[15] = 4;
	m_indices[16] = 0; // Line 9
	m_indices[17] = 6;
	m_indices[18] = 1; // Line 10
	m_indices[19] = 7;
	m_indices[20] = 2; // Line 11
	m_indices[21] = 4;
	m_indices[22] = 3; // Line 12
	m_indices[23] = 5;

	createLinesVertices();

	m_vbo.addAttribute<float>(3);
	m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
	createVAO(m_vbo);

	m_program = ResourceManager::getProgram("plain_with_uniform_color", "plain_with_uniform_color");
}

ConstructionBlock::~ConstructionBlock()
{
	Renderable::~Renderable();
}

void ConstructionBlock::createLinesVertices()
{
	for (int i = 0; i < sizeof(m_indices) / sizeof(int); i++)
	{
		auto v = m_vertices[m_indices[i]];
		m_linesVertices[i * 3] = v.x();
		m_linesVertices[i * 3 + 1] = v.y();
		m_linesVertices[i * 3 + 2] = v.z();
	}
}

void ConstructionBlock::get2DBounds(Axis axis, float* horStart, float* horEnd, float* verStart, float* verEnd)
{
	switch (axis)
	{
	case Axis::X:
		*horStart = m_boundingBox.startZ;
		*horEnd = m_boundingBox.endZ;
		*verStart = m_boundingBox.startY;
		*verEnd = m_boundingBox.endY;
		break;
	case Axis::Y:
		*horStart = m_boundingBox.startX;
		*horEnd = m_boundingBox.endX;
		*verStart = m_boundingBox.startZ;
		*verEnd = m_boundingBox.endZ;
		break;
	case Axis::Z:
		*horStart = m_boundingBox.startX;
		*horEnd = m_boundingBox.endX;
		*verStart = m_boundingBox.startY;
		*verEnd = m_boundingBox.endY;
	}
}

ConstructionBlock::ResizePointsBoundaries* ConstructionBlock::getResizePointsBoundaries(Axis axis, float zoomFactor)
{
	ConstructionBlock::ResizePointsBoundaries* b = new ConstructionBlock::ResizePointsBoundaries;
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);

	b->leftTop.horStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->leftTop.horEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	b->leftTop.verStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->leftTop.verEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	b->centerTop.horStart = (horBoundStart + horBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	b->centerTop.horEnd = (horBoundStart + horBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;
	b->centerTop.verStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->centerTop.verEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	b->rightTop.horStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->rightTop.horEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	b->rightTop.verStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->rightTop.verEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	b->rightCenter.horStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->rightCenter.horEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	b->rightCenter.verStart = (verBoundStart + verBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	b->rightCenter.verEnd = (verBoundStart + verBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;

	b->rightBottom.horStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	b->rightBottom.horEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	b->rightBottom.verStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->rightBottom.verEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	b->centerBottom.horStart = (horBoundStart + horBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	b->centerBottom.horEnd = (horBoundStart + horBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;
	b->centerBottom.verStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->centerBottom.verEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	b->leftBottom.horStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->leftBottom.horEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	b->leftBottom.verStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->leftBottom.verEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	b->leftCenter.horStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	b->leftCenter.horEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	b->leftCenter.verStart = (verBoundStart + verBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	b->leftCenter.verEnd = (verBoundStart + verBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;

	return b;
}

Qt::CursorShape ConstructionBlock::checkHover(float x, float y, Axis axis, float zoomFactor)
{
	ConstructionBlock::ResizePointsBoundaries* b = getResizePointsBoundaries(axis, zoomFactor);
	Qt::CursorShape cursor = Qt::ArrowCursor;
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);

	if (horBoundStart <= x && x <= horBoundEnd &&
		verBoundStart <= y && y <= verBoundEnd)
	{
		cursor = Qt::OpenHandCursor;
	}

	if (b->leftTop.horStart <= x && x <= b->leftTop.horEnd &&
		b->leftTop.verStart <= y && y <= b->leftTop.verEnd)
	{
		cursor = Qt::SizeFDiagCursor;
	}

	if (b->centerTop.horStart <= x && x <= b->centerTop.horEnd &&
		b->centerTop.verStart <= y && y <= b->centerTop.verEnd)
	{
		cursor = Qt::SizeVerCursor;
	}

	if (b->rightTop.horStart <= x && x <= b->rightTop.horEnd &&
		b->rightTop.verStart <= y && y <= b->rightTop.verEnd)
	{
		cursor = Qt::SizeBDiagCursor;
	}

	if (b->rightCenter.horStart <= x && x <= b->rightCenter.horEnd &&
		b->rightCenter.verStart <= y && y <= b->rightCenter.verEnd)
	{
		cursor = Qt::SizeHorCursor;
	}

	if (b->rightBottom.horStart <= x && x <= b->rightBottom.horEnd &&
		b->rightBottom.verStart <= y && y <= b->rightBottom.verEnd)
	{
		cursor = Qt::SizeFDiagCursor;
	}

	if (b->centerBottom.horStart <= x && x <= b->centerBottom.horEnd &&
		b->centerBottom.verStart <= y && y <= b->centerBottom.verEnd)
	{
		cursor = Qt::SizeVerCursor;
	}

	if (b->leftBottom.horStart <= x && x <= b->leftBottom.horEnd &&
		b->leftBottom.verStart <= y && y <= b->leftBottom.verEnd)
	{
		cursor = Qt::SizeBDiagCursor;
	}

	if (b->leftCenter.horStart <= x && x <= b->leftCenter.horEnd &&
		b->leftCenter.verStart <= y && y <= b->leftCenter.verEnd)
	{
		cursor = Qt::SizeHorCursor;
	}

	delete b;
	return cursor;
}

BlockToolState ConstructionBlock::startDrag(Axis axis, QVector2D pos, float zoomFactor)
{
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	float hor = pos.x();
	float ver = pos.y();
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);
	m_lastPos = pos;
	
	if (horBoundStart <= hor && hor <= horBoundEnd &&
		verBoundStart <= ver && ver <= verBoundEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		return BlockToolState::MOVE;
	}

	ConstructionBlock::ResizePointsBoundaries* b = getResizePointsBoundaries(axis, zoomFactor);

	if (b->leftTop.horStart <= hor && hor <= b->leftTop.horEnd &&
		b->leftTop.verStart <= ver && ver <= b->leftTop.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_TOP;
		return BlockToolState::RESIZE;
	}

	if (b->centerTop.horStart <= hor && hor <= b->centerTop.horEnd &&
		b->centerTop.verStart <= ver && ver <= b->centerTop.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_TOP;
		return BlockToolState::RESIZE;
	}

	if (b->rightTop.horStart <= hor && hor <= b->rightTop.horEnd &&
		b->rightTop.verStart <= ver && ver <= b->rightTop.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_TOP;
		return BlockToolState::RESIZE;
	}

	if (b->rightCenter.horStart <= hor && hor <= b->rightCenter.horEnd &&
		b->rightCenter.verStart <= ver && ver <= b->rightCenter.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_CENTER;
		return BlockToolState::RESIZE;
	}

	if (b->rightBottom.horStart <= hor && hor <= b->rightBottom.horEnd &&
		b->rightBottom.verStart <= ver && ver <= b->rightBottom.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_BOTTOM;
		return BlockToolState::RESIZE;
	}

	if (b->centerBottom.horStart <= hor && hor <= b->centerBottom.horEnd &&
		b->centerBottom.verStart <= ver && ver <= b->centerBottom.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_BOTTOM;
		return BlockToolState::RESIZE;
	}

	if (b->leftBottom.horStart <= hor && hor <= b->leftBottom.horEnd &&
		b->leftBottom.verStart <= ver && ver <= b->leftBottom.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_BOTTOM;
		return BlockToolState::RESIZE;
	}

	if (b->leftCenter.horStart <= hor && hor <= b->leftCenter.horEnd &&
		b->leftCenter.verStart <= ver && ver <= b->leftCenter.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_CENTER;
		return BlockToolState::RESIZE;
	}

	return BlockToolState::READY_TO_EDIT;
}

void ConstructionBlock::doResizeStep(Axis axis, QVector2D pos, float step)
{
	float hor = pos.x();
	float ver = pos.y();
	float lastHor = m_lastPos.x();
	float lastVer = m_lastPos.y();
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);

	/* LEFT TOP */
	if (m_resizeDirection == ResizeDirection::LEFT_TOP)
	{
		m_moveDeltaX += hor - lastHor;
		m_moveDeltaY += ver - lastVer;
		
		if (std::abs(m_moveDeltaX) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaX / step) * step;
			m_moveDeltaX -= steps;

			if (steps > 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f) 
				calcResize(axis, true, true, steps);
		}

		if (std::abs(m_moveDeltaY) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaY / step) * step;
			m_moveDeltaY -= steps;

			if (steps < 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, false, steps);
		}
	}
	/* CENTER TOP */
	else if (m_resizeDirection == ResizeDirection::CENTER_TOP)
	{
		m_moveDelta += ver - lastVer;

		if (std::abs(m_moveDelta) >= step)
		{
			float steps = Helpers::trunc(m_moveDelta / step) * step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, false, steps);
		}
	}
	/* RIGHT TOP */
	else if (m_resizeDirection == ResizeDirection::RIGHT_TOP)
	{
		m_moveDeltaX += hor - lastHor;
		m_moveDeltaY += ver - lastVer;

		if (std::abs(m_moveDeltaX) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaX / step) * step;
			m_moveDeltaX -= steps;

			if (steps < 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, true, false, steps);
		}

		if (std::abs(m_moveDeltaY) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaY / step) * step;
			m_moveDeltaY -= steps;

			if (steps < 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, false, steps);
		}
	}
	/* RIGHT CENTER */
	else if (m_resizeDirection == ResizeDirection::RIGHT_CENTER)
	{
		m_moveDelta += hor - lastHor;

		if (std::abs(m_moveDelta) >= step)
		{
			float steps = Helpers::trunc(m_moveDelta / step) * step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, true, false, steps);
		}
	}
	/* RIGHT BOTTOM */
	else if (m_resizeDirection == ResizeDirection::RIGHT_BOTTOM)
	{
		m_moveDeltaX += hor - lastHor;
		m_moveDeltaY += ver - lastVer;

		if (std::abs(m_moveDeltaX) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaX / step) * step;
			m_moveDeltaX -= steps;

			if (steps < 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length + steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, true, false, steps);
		}

		if (std::abs(m_moveDeltaY) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaY / step) * step;
			m_moveDeltaY -= steps;

			if (steps > 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, true, steps);
		}
	}
	/* CENTER BOTTOM */
	else if (m_resizeDirection == ResizeDirection::CENTER_BOTTOM)
	{
		m_moveDelta += ver - lastVer;

		if (std::abs(m_moveDelta) >= step)
		{
			float steps = Helpers::trunc(m_moveDelta / step) * step;
			m_moveDelta -= steps;

			if (steps > 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, true, steps);
		}
	}
	/* LEFT BOTTOM */
	else if (m_resizeDirection == ResizeDirection::LEFT_BOTTOM)
	{
		m_moveDeltaX += hor - lastHor;
		m_moveDeltaY += ver - lastVer;

		if (std::abs(m_moveDeltaX) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaX / step) * step;
			m_moveDeltaX -= steps;

			if (steps > 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, true, true, steps);
		}

		if (std::abs(m_moveDeltaY) >= step)
		{
			float steps = Helpers::trunc(m_moveDeltaY / step) * step;
			m_moveDeltaY -= steps;

			if (steps > 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, false, true, steps);
		}
	}
	/* LEFT CENTER */
	else if (m_resizeDirection == ResizeDirection::LEFT_CENTER)
	{
		m_moveDelta += hor - lastHor;

		if (std::abs(m_moveDelta) >= step)
		{
			float steps = Helpers::trunc(m_moveDelta / step) * step;
			m_moveDelta -= steps;

			if (steps > 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (Helpers::trunc(length) <= Helpers::trunc(step))
				{
					steps = 0.0f;
				}
				else if (length - steps < step)
				{
					steps = length - step;
				}
			}

			if (steps != 0.0f)
				calcResize(axis, true, true, steps);
		}
	}

	m_lastPos = pos;
}

void ConstructionBlock::calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps)
{
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	float horStart, horEnd, verStart, verEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);
	
	if (isReversed)
	{
		horStart = horBoundEnd;
		horEnd = horBoundStart;
		verStart = verBoundEnd;
		verEnd = verBoundStart;
	}
	else
	{
		horStart = horBoundStart;
		horEnd = horBoundEnd;
		verStart = verBoundStart;
		verEnd = verBoundEnd;
	}

	for (auto& v : m_vertices)
	{
		switch (axis)
		{
		case Axis::X:
			if (isHorizontal)
			{
				float factor = std::abs((v.z() - horStart) / (horEnd - horStart));
				v.setZ(v.z() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.y() - verStart) / (verEnd - verStart));
				v.setY(v.y() + steps * factor);
			}
			break;
		case Axis::Y:
			if (isHorizontal)
			{
				float factor = std::abs((v.x() - horStart) / (horEnd - horStart));
				v.setX(v.x() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.z() - verStart) / (verEnd - verStart));
				v.setZ(v.z() + steps * factor);
			}
			break;
		case Axis::Z:
			if (isHorizontal)
			{
				float factor = std::abs((v.x() - horStart) / (horEnd - horStart));
				v.setX(v.x() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.y() - verStart) / (verEnd - verStart));
				v.setY(v.y() + steps * factor);
			}
		}
	}

	switch (axis)
	{
	case Axis::X:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startZ += steps;
			else
				m_boundingBox.endZ += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startY += steps;
			else
				m_boundingBox.endY += steps;
		}
		break;
	case Axis::Y:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startX += steps;
			else
				m_boundingBox.endX += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startZ += steps;
			else
				m_boundingBox.endZ += steps;
		}
		break;
	case Axis::Z:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startX += steps;
			else
				m_boundingBox.endX += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startY += steps;
			else
				m_boundingBox.endY += steps;
		}
	}

	createLinesVertices();
	m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
}

QList<QVector3D>* ConstructionBlock::getResizePointsTranslationVectors(Axis axis, float zoomFactor, QVector3D zoomVec)
{
	auto getPositionByAxis = [&axis](float hor, float ver) {
		switch (axis)
		{
		case Axis::X:
			return QVector3D(0.0f, ver, hor);
		case Axis::Y:
			return QVector3D(hor, 0.0f, ver);
		case Axis::Z:
			return QVector3D(hor, ver, 0.0f);
		}
	};

	QList<QVector3D>* vectors = new QList<QVector3D>;
	QVector3D origin;
	float hor, ver;
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;

	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);

	/* LEFT TOP */
	hor = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* CENTER TOP */
	hor = (horBoundStart + horBoundEnd) / 2;
	ver = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* RIGHT TOP */
	hor = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* RIGHT CENTER */
	hor = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = (verBoundStart + verBoundEnd) / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* RIGHT BOTTOM */
	hor = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* CENTER BOTTOM */
	hor = (horBoundStart + horBoundEnd) / 2;
	ver = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* LEFT BOTTOM */
	hor = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	/* LEFT CENTER */
	hor = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor / 2;
	ver = (verBoundStart + verBoundEnd) / 2;
	origin = getPositionByAxis(hor, ver);
	vectors->push_back(QVector3D(origin.x() * zoomVec.x(), origin.y() * zoomVec.y(), origin.z() * zoomVec.z()));

	return vectors;
}

void ConstructionBlock::doMoveStep(Axis axis, QVector2D pos, float step)
{
	float stepsX = 0.0f, stepsY = 0.0f;
	m_moveDeltaX += pos.x() - m_lastPos.x();
	m_moveDeltaY += pos.y() - m_lastPos.y();

	if (std::abs(m_moveDeltaX) >= step)
	{
		stepsX = Helpers::trunc(m_moveDeltaX / step) * step;
		m_moveDeltaX -= stepsX;
	}

	if (std::abs(m_moveDeltaY) >= step)
	{
		stepsY = Helpers::trunc(m_moveDeltaY / step) * step;
		m_moveDeltaY -= stepsY;
	}

	if (std::abs(stepsX) > 0.0f || std::abs(stepsY) > 0.0f)
	{
		for (auto& v : m_vertices)
		{
			switch (axis)
			{
			case Axis::X:
				v.setZ(v.z() + stepsX);
				v.setY(v.y() + stepsY);
				break;
			case Axis::Y:
				v.setX(v.x() + stepsX);
				v.setZ(v.z() + stepsY);
				break;
			case Axis::Z:
				v.setX(v.x() + stepsX);
				v.setY(v.y() + stepsY);
			}
		}
		
		switch (axis)
		{
		case Axis::X:
			m_boundingBox.startZ += stepsX;
			m_boundingBox.endZ += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
			break;
		case Axis::Y:
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startZ += stepsY;
			m_boundingBox.endZ += stepsY;
			break;
		case Axis::Z:
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
		}

		createLinesVertices();
		m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
	}
	
	m_lastPos = pos;
}

void ConstructionBlock::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(zoomVec);
	
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("proj", proj));
	GLCall(m_program->setUniformValue("view", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("model", model));
	GLCall(m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f));

	vao->bind();
	GLCall($->glDrawArrays(GL_LINES, 0, verticesCount()));

	if (m_isEditingMode)
	{
		Point point(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f);
		
		QList<QVector3D>* translationVectors = getResizePointsTranslationVectors(axis, factor, zoomVec);

		for (auto& vec : *translationVectors)
		{
			context->makeCurrent(context->surface());

			model.setToIdentity();
			model.translate(vec);

			point.m_program->bind();
			point.m_program->setUniformValue("proj", proj);
			point.m_program->setUniformValue("view", camera.getViewMatrix());
			point.m_program->setUniformValue("model", model);
			point.m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f);

			point.bindVAO(context);
			$->glDrawArrays(GL_TRIANGLES, 0, point.verticesCount());
		}

		delete translationVectors;
	}
}
