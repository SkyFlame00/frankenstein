#include "GLWidget3D.h"
#include "../../common/GlobalData.h"
#include "../../common/cgal_bindings.h"
#include "../../common/constants.h"

void GLWidget3D::processSelectionTool()
{
	auto global = GlobalData::getInstance();
	auto& data = global->m_selectionToolData;

	if (isWidgetActive() && m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
	{
		Brush* brush;
		Types::Polygon* polygon;
		Types::Triangle* triangle;
		QVector3D point;

		bool isBrushPicked = pickBrush(&brush, &polygon, &triangle, &point);

		if (!isBrushPicked)
		{
			GlobalData::hideBrushMetrics();
			return;
		}

		if (data.renderable)
		{
			data.renderable->m_selected = false;
			data.renderable = nullptr;
		}

		data.renderable = brush;
		brush->m_selected = true;
		GlobalData::showBrushMetrics();
		GlobalData::updateBrushMetrics(brush->getWidth(), brush->getHeight(), brush->getLength());
	}
}

bool GLWidget3D::hasIntersection(QVector3D position, QVector3D direction, Types::Triangle triangle, QVector3D origin, QVector3D& output)
{
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(Constants::SCALE_VECTOR);
	model.translate(origin);
	auto v0 = model * QVector4D(*triangle.v0, 1.0f);
	auto v1 = model * QVector4D(*triangle.v1, 1.0f);
	auto v2 = model * QVector4D(*triangle.v2, 1.0f);

	auto cgal_triangle_v0 = EXACT_K::Point_3(v0.x(), v0.y(), v0.z());
	auto cgal_triangle_v1 = EXACT_K::Point_3(v1.x(), v1.y(), v1.z());
	auto cgal_triangle_v2 = EXACT_K::Point_3(v2.x(), v2.y(), v2.z());
	auto cgal_position = EXACT_K::Point_3(position.x(), position.y(), position.z());
	auto cgal_direction = EXACT_K::Direction_3(direction.x(), direction.y(), direction.z());
	EXACT_K::Triangle_3 cgal_triangle(cgal_triangle_v0, cgal_triangle_v1, cgal_triangle_v2);
	EXACT_K::Ray_3 cgal_ray(cgal_position, cgal_direction);

	auto res = intersection(cgal_triangle, cgal_ray);

	if (res)
	{
		const EXACT_K::Point_3* p = boost::get<EXACT_K::Point_3>(&*res);
		auto x = p->x().exact().to_double();
		auto y = p->y().exact().to_double();
		auto z = p->z().exact().to_double();
		output.setX(x);
		output.setY(y);
		output.setZ(z);

		return true;
	}

	return false;
}
