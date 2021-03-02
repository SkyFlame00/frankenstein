#include "GLWidget2D.h"
#include "../../common/GlobalData.h"
#include "../../common/helpers.h"
#include <algorithm>

void GLWidget2D::processClippingTool()
{
	auto globalData = GlobalData::getInstance();
	auto data = &globalData->m_clippingToolData;

	if (data->axis != Axis::NONE && m_axis != data->axis)
		return;

	float x, y;
	float nearestX, nearestY;
	bool isWidgetActive = m_inputData.isMouseOver;
	auto lmDown = m_inputData.leftMouseDown;
	getMouseCoordinates(m_inputData.mouseX, m_inputData.mouseY, &x, &y);
	getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);

	auto getAxisVector = [&]() -> QVector3D {
		switch (m_axis)
		{
		case Axis::X:
			return QVector3D(1.0f, 0.0, 0.0f);
		case Axis::Y:
			return QVector3D(0.0f, 1.0, 0.0f);
		case Axis::Z:
			return QVector3D(0.0f, 0.0, -1.0f);
		}
	};

	auto getLineCoords = [&](float hor, QVector2D cornerCase) -> QVector2D {
		switch (m_axis)
		{
		case Axis::X:
			if (data->plane.b == 0.0f)
				return cornerCase;
			else
				return QVector2D(hor, (-data->plane.d - data->plane.c * hor) / data->plane.b);
		case Axis::Y:
			if (data->plane.c == 0.0f)
				return cornerCase;
			else
				return QVector2D(hor, (-data->plane.d - data->plane.a * hor) / data->plane.c);
		case Axis::Z:
			if (data->plane.b == 0.0f)
				return cornerCase;
			else
				return QVector2D(hor, (-data->plane.d - data->plane.a * hor) / data->plane.b);
		}
	};

	if (data->state == Types::ClippingToolState::READY)
	{
		if (lmDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			QVector3D pointPos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY, 1.0f);
			data->point1->m_origin = pointPos;
			m_guiObjects.push_back(data->point1);
			data->state = Types::ClippingToolState::POINT_PLACED;
			data->pt1_placed = true;
			data->axis = m_axis;
		}
	}
	else if (data->state == Types::ClippingToolState::POINT_PLACED)
	{
		if (lmDown == ButtonDownState::DOWN_PROCESSED)
		{
			QVector3D point1Pos = data->point1->m_origin;
			QVector3D point2Pos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY, 1.0f);
			bool isSamePoint = this->isSamePoint(point1Pos, point2Pos);
			auto pt1 = Helpers::get2DPointFrom3D(m_axis, point1Pos);
			auto pt2 = QVector2D(nearestX, nearestY);
			
			if (!isSamePoint)
			{
				data->point2->m_origin = point2Pos;
				data->pt2_placed = true;

				bool containsPt2 =
					std::find(m_guiObjects.begin(), m_guiObjects.end(), data->point2) != m_guiObjects.end();

				if (!containsPt2)
					m_guiObjects.push_back(data->point2);

				auto vec1 = point2Pos - point1Pos;
				auto vec2 = getAxisVector();
				auto coeffs = QVector3D::crossProduct(vec1, vec2);
				auto a = coeffs.x();
				auto b = coeffs.y();
				auto c = coeffs.z();
				data->plane.a = a;
				data->plane.b = b;
				data->plane.c = c;
				data->plane.d = (-a) * point1Pos.x() + (-b) * point1Pos.y() + (-c) * point1Pos.z();

				float xx1 = 10000.0f;
				float xx2 = -10000.0f;

				auto v1 = getLineCoords(xx1, QVector2D(pt1.x(), xx1));
				auto v2 = getLineCoords(xx2, QVector2D(pt1.x(), xx2));

				data->line->m_pt1Origin = Helpers::getTop3DPointFrom2D(m_axis, v1.x(), v1.y());
				data->line->m_pt2Origin = Helpers::getTop3DPointFrom2D(m_axis, v2.x(), v2.y());

				bool containsLine =
					std::find(m_guiObjects.begin(), m_guiObjects.end(), data->line) != m_guiObjects.end();

				if (!containsLine)
					m_guiObjects.push_back(data->line);
			}
		}
		else if (lmDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			if (!data->pt2_placed)
			{
				m_guiObjects.removeOne(data->point1);
				data->pt1_placed = false;
				data->axis = Axis::NONE;
			}
			else
			{
				data->state = Types::ClippingToolState::INTERSECTION;
			}
		}
	}
	else if (data->state == Types::ClippingToolState::INTERSECTION)
	{
		if (m_inputData.keyEscape == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			m_guiObjects.removeOne(data->point1);
			m_guiObjects.removeOne(data->point2);
			m_guiObjects.removeOne(data->line);
			data->pt1_placed = false;
			data->pt2_placed = false;
			data->state = Types::ClippingToolState::READY;
			data->axis = Axis::NONE;
		}
		else if (lmDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			bool pt1DragStarted = data->point1->hasHover(m_axis, x, y, getZoomFactor());
			bool pt2DragStarted = data->point2->hasHover(m_axis, x, y, getZoomFactor());

			if (pt1DragStarted)
			{
				data->d_pt = data->point1;
				data->nd_pt = data->point2;
			}

			if (pt2DragStarted)
			{
				data->d_pt = data->point2;
				data->nd_pt = data->point1;
			}

			data->state = Types::ClippingToolState::POINT_DISPLACEMENT;
		}
		else
		{
			bool pt1HasHover = data->point1->hasHover(m_axis, x, y, getZoomFactor());
			bool pt2HasHover = data->point2->hasHover(m_axis, x, y, getZoomFactor());

			setCursor((pt1HasHover || pt2HasHover) ? Qt::OpenHandCursor : Qt::ArrowCursor);


		}
	}
	else if (data->state == Types::ClippingToolState::POINT_DISPLACEMENT)
	{
		if (lmDown == ButtonDownState::DOWN_PROCESSED)
		{
			QVector3D point1Pos = data->nd_pt->m_origin;
			QVector3D point2Pos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY, 1.0f);
			bool isSamePoint = this->isSamePoint(point1Pos, point2Pos);
			auto pt1 = Helpers::get2DPointFrom3D(m_axis, point1Pos);
			auto pt2 = QVector2D(nearestX, nearestY);

			if (!isSamePoint)
			{
				data->d_pt->m_origin = point2Pos;

				auto vec1 = point2Pos - point1Pos;
				auto vec2 = getAxisVector();
				auto coeffs = QVector3D::crossProduct(vec1, vec2);
				auto a = coeffs.x();
				auto b = coeffs.y();
				auto c = coeffs.z();
				data->plane.a = a;
				data->plane.b = b;
				data->plane.c = c;
				data->plane.d = (-a) * point1Pos.x() + (-b) * point1Pos.y() + (-c) * point1Pos.z();

				float xx1 = 10000.0f;
				float xx2 = -10000.0f;

				auto v1 = getLineCoords(xx1, QVector2D(pt1.x(), xx1));
				auto v2 = getLineCoords(xx2, QVector2D(pt1.x(), xx2));

				data->line->m_pt1Origin = Helpers::getTop3DPointFrom2D(m_axis, v1.x(), v1.y());
				data->line->m_pt2Origin = Helpers::getTop3DPointFrom2D(m_axis, v2.x(), v2.y());
			}
		}
		else if (lmDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			data->state = Types::ClippingToolState::INTERSECTION;
		}
	}
}
