#include "ChangeableRenderable.h"
#include "../common/helpers.h"

void ChangeableRenderable::doResizeStep(Axis axis, QVector2D pos, float step, float* outStepsX, float* outStepsY)
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
			{
				calcResize(axis, true, true, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
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
			{
				calcResize(axis, false, false, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, false, false, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, true, false, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
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
			{
				calcResize(axis, false, false, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, true, false, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
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
			{
				calcResize(axis, true, false, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
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
			{
				calcResize(axis, false, true, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, false, true, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, true, true, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
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
			{
				calcResize(axis, false, true, steps);
				if (outStepsY)
					*outStepsY = steps;
			}
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
			{
				calcResize(axis, true, true, steps);
				if (outStepsX)
					*outStepsX = steps;
			}
		}
	}

	m_lastPos = pos;
}

void ChangeableRenderable::get2DBounds(Axis axis, float* horStart, float* horEnd, float* verStart, float* verEnd)
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

QList<QVector3D>* ChangeableRenderable::getResizePointsTranslationVectors(Axis axis, float zoomFactor, QVector3D zoomVec)
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

ResizePointsBoundaries* ChangeableRenderable::getResizePointsBoundaries(Axis axis, float zoomFactor)
{
	ResizePointsBoundaries* b = new ResizePointsBoundaries;
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

Qt::CursorShape ChangeableRenderable::checkHover(float x, float y, Axis axis, float zoomFactor)
{
	ResizePointsBoundaries* b = getResizePointsBoundaries(axis, zoomFactor);
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

Types::BrushAction ChangeableRenderable::startDrag(Axis axis, QVector2D pos, float zoomFactor)
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
		return Types::BrushAction::MOVE;
	}

	ResizePointsBoundaries* b = getResizePointsBoundaries(axis, zoomFactor);

	if (b->leftTop.horStart <= hor && hor <= b->leftTop.horEnd &&
		b->leftTop.verStart <= ver && ver <= b->leftTop.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_TOP;
		return Types::BrushAction::RESIZE;
	}

	if (b->centerTop.horStart <= hor && hor <= b->centerTop.horEnd &&
		b->centerTop.verStart <= ver && ver <= b->centerTop.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_TOP;
		return Types::BrushAction::RESIZE;
	}

	if (b->rightTop.horStart <= hor && hor <= b->rightTop.horEnd &&
		b->rightTop.verStart <= ver && ver <= b->rightTop.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_TOP;
		return Types::BrushAction::RESIZE;
	}

	if (b->rightCenter.horStart <= hor && hor <= b->rightCenter.horEnd &&
		b->rightCenter.verStart <= ver && ver <= b->rightCenter.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_CENTER;
		return Types::BrushAction::RESIZE;
	}

	if (b->rightBottom.horStart <= hor && hor <= b->rightBottom.horEnd &&
		b->rightBottom.verStart <= ver && ver <= b->rightBottom.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_BOTTOM;
		return Types::BrushAction::RESIZE;
	}

	if (b->centerBottom.horStart <= hor && hor <= b->centerBottom.horEnd &&
		b->centerBottom.verStart <= ver && ver <= b->centerBottom.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_BOTTOM;
		return Types::BrushAction::RESIZE;
	}

	if (b->leftBottom.horStart <= hor && hor <= b->leftBottom.horEnd &&
		b->leftBottom.verStart <= ver && ver <= b->leftBottom.verEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_BOTTOM;
		return Types::BrushAction::RESIZE;
	}

	if (b->leftCenter.horStart <= hor && hor <= b->leftCenter.horEnd &&
		b->leftCenter.verStart <= ver && ver <= b->leftCenter.verEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_CENTER;
		return Types::BrushAction::RESIZE;
	}

	return Types::BrushAction::READY;
}
