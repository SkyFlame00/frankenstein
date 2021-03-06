#include "GLWidget3D.h"
#include "../../common/GlobalData.h"
#include "../../common/cgal_bindings.h"

void GLWidget3D::processSelectionTool()
{
	auto global = GlobalData::getInstance();
	auto& data = global->m_selectionToolData;
	bool isWidgetActive = m_inputData.isMouseOver;
	int mouseX = m_inputData.mouseX;
	int mouseY = m_inputData.mouseY;

	if (isWidgetActive && m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
	{
		float renderId = m_renderer->getSelectionValue(mouseX, m_screenHeight - mouseY);
		auto renderable = m_renderer->getBrushByRenderId(renderId);

		if (renderId == 0.0f || !renderable)
		{
			if (data.renderable)
			{
				data.renderable->m_selected = false;
				data.renderable = nullptr;
			}
			goto end;
		}

		QVector3D pointingRay =
			m_camera->getPickingRay(mouseX, mouseY, m_screenWidth, m_screenHeight, m_renderer->getNearPlane(), m_renderer->getFarPlane(), m_renderer->getProjMatrix());
		QVector3D intersectionPoint;
		Types::Polygon* intersectionPolygon = nullptr;
		Types::Triangle* intersectionTriangle = nullptr;

		for (auto& polygon : renderable->getPolygons())
		{
			for (auto& triangle : polygon->triangles)
			{
				auto cameraPos = m_camera->getPosition();
				QVector3D pt;
				if (hasIntersection(cameraPos, pointingRay, triangle, renderable->m_origin, pt))
				{
					if (!intersectionPolygon)
					{
						intersectionPolygon = polygon;
						intersectionTriangle = &triangle;
						intersectionPoint = pt;
						continue;
					}

					float d1 = std::abs(cameraPos.distanceToPoint(intersectionPoint));
					float d2 = std::abs(cameraPos.distanceToPoint(pt));

					if (d2 < d1)
					{
						intersectionPolygon = polygon;
						intersectionTriangle = &triangle;
						intersectionPoint = pt;
					}
				}
			}
		}

		if (data.renderable)
		{
			data.renderable->m_selected = false;
			data.renderable = nullptr;
		}

		data.renderable = renderable;
		renderable->m_selected = true;
	}
end:;
}

bool GLWidget3D::hasIntersection(QVector3D position, QVector3D direction, Types::Triangle triangle, QVector3D origin, QVector3D& output)
{
	QMatrix4x4 model;
	model.setToIdentity();
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
