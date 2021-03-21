#include "GLWidget2D.h"
#include "../../common/GlobalData.h"
#include "../../common/helpers.h"
#include <algorithm>
#include "../../common/cgal_bindings.h"

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
			bool isSamePoint1 = this->isSamePoint(point1Pos, point2Pos);
			bool isSamePoint2 = false;
			auto pt1 = Helpers::get2DPointFrom3D(m_axis, point1Pos);
			auto pt2 = QVector2D(nearestX, nearestY);
			
			if (data->pt2_placed)
			{
				isSamePoint2 = this->isSamePoint(data->point2->m_origin, point2Pos);
			}

			if (!isSamePoint1 && !isSamePoint2)
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
				data->plane.norm = coeffs;
				data->plane.norm.normalize();
				data->plane.p0 = point1Pos;

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

				clipBrush();
				data->glWidget = this;
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
			discardClipping();
		}
		else if (lmDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			bool pt1DragStarted = data->point1->hasHover(m_axis, x, y, getZoomFactor());
			bool pt2DragStarted = data->point2->hasHover(m_axis, x, y, getZoomFactor());

			if (pt1DragStarted)
			{
				data->d_pt = data->point1;
				data->nd_pt = data->point2;
				data->state = Types::ClippingToolState::POINT_DISPLACEMENT;
			}

			if (pt2DragStarted)
			{
				data->d_pt = data->point2;
				data->nd_pt = data->point1;
				data->state = Types::ClippingToolState::POINT_DISPLACEMENT;
			}
		}
		else if (m_inputData.keyReturn == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			applyClipping();
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
			bool isSamePoint1 = this->isSamePoint(point1Pos, point2Pos);
			bool isSamePoint2 = this->isSamePoint(data->d_pt->m_origin, point2Pos);
			auto pt1 = Helpers::get2DPointFrom3D(m_axis, point1Pos);
			auto pt2 = QVector2D(nearestX, nearestY);

			if (!isSamePoint1 && !isSamePoint2)
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
				data->plane.norm = coeffs;
				data->plane.norm.normalize();
				data->plane.p0 = point1Pos;

				float xx1 = 10000.0f;
				float xx2 = -10000.0f;

				auto v1 = getLineCoords(xx1, QVector2D(pt1.x(), xx1));
				auto v2 = getLineCoords(xx2, QVector2D(pt1.x(), xx2));

				data->line->m_pt1Origin = Helpers::getTop3DPointFrom2D(m_axis, v1.x(), v1.y());
				data->line->m_pt2Origin = Helpers::getTop3DPointFrom2D(m_axis, v2.x(), v2.y());

				clipBrush();
			}
		}
		else if (lmDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			data->state = Types::ClippingToolState::INTERSECTION;
		}
	}
}

void GLWidget2D::clipBrush()
{
	auto globalData = GlobalData::getInstance();
	auto sdata = &globalData->m_selectionToolData;
	auto cdata = &globalData->m_clippingToolData;
	auto& brush = sdata->renderable;

	/* Brush has not been selected, nothing to do */
	if (!brush)
		return;

	if (brush->m_clippedBrush)
	{
		delete brush->m_clippedBrush;
		brush->m_clippedBrush = nullptr;
	}

	if (brush->m_remainingBrush)
	{
		delete brush->m_remainingBrush;
		brush->m_remainingBrush = nullptr;
	}

	std::vector<INEXACT_K::Point_3> intersectionPts;

	auto isIntersectionPt = [&](QVector3D pt) {
		auto res = std::find_if(intersectionPts.begin(), intersectionPts.end(), [&](INEXACT_K::Point_3 _pt)
			{
				return Helpers::areEqual(pt.x(), _pt.x()) &&
					Helpers::areEqual(pt.y(), _pt.y()) &&
					Helpers::areEqual(pt.z(), _pt.z());
			});

		return res != intersectionPts.end();
	};

	for (auto& seg : brush->getUniqueEdges())
	{
		QVector3D pt;
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(brush->m_origin);

		QVector3D v0 = model * *seg.v0;
		QVector3D v1 = model * *seg.v1;

		if (Helpers::lineSegmentPlaneIntersection(v0, v1, cdata->plane.p0, cdata->plane.norm, &pt))
		{
			auto x = pt.x() - brush->m_origin.x();
			auto y = pt.y() - brush->m_origin.y();
			auto z = pt.z() - brush->m_origin.z();

			if (!isIntersectionPt({ x, y, z }))
				intersectionPts.push_back({ x, y, z });
		}
	}

	std::vector<INEXACT_K::Point_3> clippedBrushVertices{ intersectionPts };
	std::vector<INEXACT_K::Point_3> remainingBrushVertices{ intersectionPts };

	QVector3D norm = cdata->plane.norm;
	QVector3D p0 = cdata->plane.p0;
	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(brush->m_origin);

	if (cdata->plane.normReversed)
		norm = -norm;

	for (auto& vertex : brush->getUniqueVertices())
	{
		QVector3D v(model * QVector4D(*vertex, 1.0f));
		float sign = QVector3D::dotProduct(norm, v - p0);

		if (sign < 0)
			clippedBrushVertices.push_back({ vertex->x(), vertex->y(), vertex->z() });
			
		else if (sign > 0)
			remainingBrushVertices.push_back({ vertex->x(), vertex->y(), vertex->z() });
	}

	Brush* clippedBrush = nullptr;
	Brush* remainingBrush = nullptr;

	/* If size is equal then no vertices were added after the clipping */
	if (clippedBrushVertices.size() > intersectionPts.size())
	{
		Polyhedron_3 poly;
		CGAL::convex_hull_3(clippedBrushVertices.begin(), clippedBrushVertices.end(), poly);
		clippedBrush = new Brush(poly, brush);
	}

	if (remainingBrushVertices.size() > intersectionPts.size())
	{
		Polyhedron_3 poly;
		CGAL::convex_hull_3(remainingBrushVertices.begin(), remainingBrushVertices.end(), poly);
		remainingBrush = new Brush(poly, brush);
	}

	brush->m_beingClipped = true;

	if (clippedBrush)
	{
		clippedBrush->m_beingCut = true;
		brush->m_clippedBrush = clippedBrush;
	}

	if (remainingBrush)
	{
		brush->m_remainingBrush = remainingBrush;
	}
}

void GLWidget2D::applyClipping()
{
	auto globalData = GlobalData::getInstance();
	auto sdata = &globalData->m_selectionToolData;
	auto cdata = &globalData->m_clippingToolData;
	auto& brush = sdata->renderable;

	if (brush->m_remainingBrush)
	{
		m_scene->addObject(brush->m_remainingBrush);
	}

	if (brush->m_clippedBrush)
	{
		delete brush->m_clippedBrush;
	}

	brush->m_remainingBrush = nullptr;
	brush->m_clippedBrush = nullptr;
	m_scene->removeObject(brush);
	delete brush;

	m_guiObjects.removeOne(cdata->point1);
	m_guiObjects.removeOne(cdata->point2);
	m_guiObjects.removeOne(cdata->line);
	cdata->pt1_placed = false;
	cdata->pt2_placed = false;
	cdata->state = Types::ClippingToolState::READY;
	cdata->axis = Axis::NONE;
	cdata->glWidget = nullptr;

	sdata->renderable = nullptr;
}

void GLWidget2D::discardClipping()
{
	auto globalData = GlobalData::getInstance();
	auto data = &globalData->m_clippingToolData;
	auto sdata = &globalData->m_selectionToolData;
	auto& brush = sdata->renderable;

	if (data->state == Types::ClippingToolState::READY)
	{
		return;
	}

	if (data->pt1_placed && data->pt2_placed)
	{
		m_guiObjects.removeOne(data->line);
	}

	if (data->pt1_placed)
	{
		m_guiObjects.removeOne(data->point1);
		data->pt1_placed = false;
	}

	if (data->pt2_placed)
	{
		m_guiObjects.removeOne(data->point2);
		data->pt2_placed = false;
	}

	data->state = Types::ClippingToolState::READY;
	data->axis = Axis::NONE;
	data->glWidget = nullptr;

	brush->m_beingClipped = false;

	if (brush->m_clippedBrush)
	{
		delete brush->m_clippedBrush;
		brush->m_clippedBrush = nullptr;
	}

	if (brush->m_remainingBrush)
	{
		delete brush->m_remainingBrush;
		brush->m_remainingBrush = nullptr;
	}
}
