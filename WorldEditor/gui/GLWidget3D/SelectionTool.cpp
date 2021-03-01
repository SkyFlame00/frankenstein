#include "GLWidget3D.h"
#include "../../common/GlobalData.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Line_2 Line_2;
typedef K::Intersect_2 Intersect_2;
typedef K::Point_3 Point_3;
typedef K::Triangle_3 Triangle_3;
typedef K::Ray_3 Ray_3;
typedef K::Direction_3 Direction_3;

void GLWidget3D::processSelectionTool()
{
	auto global = GlobalData::getInstance();
	auto& data = global->m_selectionToolData;
	bool isWidgetActive = m_inputData.isMouseOver;
	int mouseX = m_inputData.mouseX;
	int mouseY = m_inputData.mouseY;

	if (isWidgetActive && m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
	{
		QVector3D pointingRay =
			m_camera->getPickingRay(mouseX, mouseY, m_screenWidth, m_screenHeight, m_renderer->getNearPlane(), m_renderer->getFarPlane(), m_renderer->getProjMatrix());
		QVector3D intersectionPoint;
		Brush* intersectionRenderable;
		Types::Polygon* intersectionPolygon;
		Types::Triangle* intersectionTriangle;
		bool intersected = false;

		for (auto& renderable : m_scene->getObjects())
		{
			for (auto& polygon : renderable->getPolygons())
			{
				for (auto& triangle : polygon->triangles)
				{
					if (hasIntersection(m_camera->getPosition(), pointingRay, triangle, renderable->m_origin, intersectionPoint))
					{
						intersectionRenderable = renderable;
						intersectionPolygon = polygon;
						intersectionTriangle = &triangle;
						qInfo() << "Succeed";
						intersected = true;
						goto after_loop;
					}
				}
			}
		}

	after_loop:
		if (intersected)
		{
			if (data.renderable)
			{
				data.renderable->m_selected = false;
				data.renderable = nullptr;
			}

			data.renderable = intersectionRenderable;
			intersectionRenderable->m_selected = true;
		}
		else
		{
			if (data.renderable)
			{
				data.renderable->m_selected = false;
				data.renderable = nullptr;
			}
		}
	}
}

bool GLWidget3D::hasIntersection(QVector3D position, QVector3D direction, Types::Triangle triangle, QVector3D origin, QVector3D& output)
{
	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(origin);
	auto v0 = model * QVector4D(*triangle.v0, 1.0f);
	auto v1 = model * QVector4D(*triangle.v1, 1.0f);
	auto v2 = model * QVector4D(*triangle.v2, 1.0f);

	auto cgal_triangle_v0 = Point_3(v0.x(), v0.y(), v0.z());
	auto cgal_triangle_v1 = Point_3(v1.x(), v1.y(), v1.z());
	auto cgal_triangle_v2 = Point_3(v2.x(), v2.y(), v2.z());
	auto cgal_position = Point_3(position.x(), position.y(), position.z());
	auto cgal_direction = Direction_3(direction.x(), direction.y(), direction.z());
	Triangle_3 cgal_triangle(cgal_triangle_v0, cgal_triangle_v1, cgal_triangle_v2);
	Ray_3 cgal_ray(cgal_position, cgal_direction);

	auto res = intersection(cgal_triangle, cgal_ray);

	if (res)
	{
		const Point_3* p = boost::get<Point_3>(&*res);
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
