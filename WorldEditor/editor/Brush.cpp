#include "Brush.h"
#include "ResourceManager.h"
#include "../common/GlobalData.h"
#include "ResourceManager.h"
#include <limits>
#include "Grid2D.h"
#include "../common/helpers.h"

Brush::Brush(QList<QVector3D>& cubeVertices, QVector3D color)
	: m_uniformColor(color), m_selectionColor(1.0f, 0.0f, 0.0f),
	m_resizePoint(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f)
{
	QVector3D total(0.0f, 0.0f, 0.0f);

	for (auto& v : cubeVertices)
	{
		total += v;
	}

	m_origin = total / cubeVertices.size();

	for (auto& v : cubeVertices)
	{
		m_uniqueVertices.push_back(new QVector3D(v - m_origin));
	}

	/* Polygon 1 */
	Types::Polygon* poly1 = new Types::Polygon;
	poly1->verticesMap[m_uniqueVertices[0]] = QVector2D(0, 0);
	poly1->verticesMap[m_uniqueVertices[1]] = QVector2D(0, 1);
	poly1->verticesMap[m_uniqueVertices[2]] = QVector2D(1, 1);
	poly1->verticesMap[m_uniqueVertices[3]] = QVector2D(1, 0);
	poly1->borderEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	poly1->borderEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[3] });
	poly1->borderEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly1->borderEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly1->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[1], m_uniqueVertices[2] });
	poly1->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[2], m_uniqueVertices[3] });
	calcNorm(poly1);
	m_polygons.push_back(poly1);

	/* Polygon 2 */
	Types::Polygon* poly2 = new Types::Polygon;
	poly2->verticesMap[m_uniqueVertices[0]] = QVector2D(0, 0);
	poly2->verticesMap[m_uniqueVertices[6]] = QVector2D(0, 1);
	poly2->verticesMap[m_uniqueVertices[7]] = QVector2D(1, 1);
	poly2->verticesMap[m_uniqueVertices[1]] = QVector2D(1, 0);
	poly2->borderEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	poly2->borderEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	poly2->borderEdges.push_back({ m_uniqueVertices[6], m_uniqueVertices[7] });
	poly2->borderEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	poly2->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[6], m_uniqueVertices[7] });
	poly2->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[7], m_uniqueVertices[1] });
	calcNorm(poly2);
	m_polygons.push_back(poly2);

	/* Polygon 3 */
	Types::Polygon* poly3 = new Types::Polygon;
	poly3->verticesMap[m_uniqueVertices[1]] = QVector2D(0, 0);
	poly3->verticesMap[m_uniqueVertices[7]] = QVector2D(0, 1);
	poly3->verticesMap[m_uniqueVertices[4]] = QVector2D(1, 1);
	poly3->verticesMap[m_uniqueVertices[2]] = QVector2D(1, 0);
	poly3->borderEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	poly3->borderEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly3->borderEdges.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly3->borderEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	poly3->triangles.push_back({ m_uniqueVertices[1], m_uniqueVertices[7], m_uniqueVertices[4] });
	poly3->triangles.push_back({ m_uniqueVertices[1], m_uniqueVertices[4], m_uniqueVertices[2] });
	calcNorm(poly3);
	m_polygons.push_back(poly3);

	/* Polygon 4 */
	Types::Polygon* poly4 = new Types::Polygon;
	poly4->verticesMap[m_uniqueVertices[2]] = QVector2D(0, 0);
	poly4->verticesMap[m_uniqueVertices[4]] = QVector2D(0, 1);
	poly4->verticesMap[m_uniqueVertices[5]] = QVector2D(1, 1);
	poly4->verticesMap[m_uniqueVertices[3]] = QVector2D(1, 0);
	poly4->borderEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	poly4->borderEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly4->borderEdges.push_back({ m_uniqueVertices[4], m_uniqueVertices[5] });
	poly4->borderEdges.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });
	poly4->triangles.push_back({ m_uniqueVertices[2], m_uniqueVertices[4], m_uniqueVertices[5] });
	poly4->triangles.push_back({ m_uniqueVertices[2], m_uniqueVertices[5], m_uniqueVertices[3] });
	calcNorm(poly4);
	m_polygons.push_back(poly4);

	/* Polygon 5 */
	Types::Polygon* poly5 = new Types::Polygon;
	poly5->verticesMap[m_uniqueVertices[3]] = QVector2D(0, 0);
	poly5->verticesMap[m_uniqueVertices[5]] = QVector2D(0, 1);
	poly5->verticesMap[m_uniqueVertices[6]] = QVector2D(1, 1);
	poly5->verticesMap[m_uniqueVertices[0]] = QVector2D(1, 0);
	poly5->borderEdges.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });
	poly5->borderEdges.push_back({ m_uniqueVertices[3], m_uniqueVertices[0] });
	poly5->borderEdges.push_back({ m_uniqueVertices[5], m_uniqueVertices[6] });
	poly5->borderEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	poly5->triangles.push_back({ m_uniqueVertices[3], m_uniqueVertices[5], m_uniqueVertices[6] });
	poly5->triangles.push_back({ m_uniqueVertices[3], m_uniqueVertices[6], m_uniqueVertices[0] });
	calcNorm(poly5);
	m_polygons.push_back(poly5);

	/* Polygon 6 */
	Types::Polygon* poly6 = new Types::Polygon;
	poly6->verticesMap[m_uniqueVertices[7]] = QVector2D(0, 0);
	poly6->verticesMap[m_uniqueVertices[6]] = QVector2D(0, 1);
	poly6->verticesMap[m_uniqueVertices[5]] = QVector2D(1, 1);
	poly6->verticesMap[m_uniqueVertices[4]] = QVector2D(1, 0);
	poly6->borderEdges.push_back({ m_uniqueVertices[7], m_uniqueVertices[6] });
	poly6->borderEdges.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly6->borderEdges.push_back({ m_uniqueVertices[6], m_uniqueVertices[5] });
	poly6->borderEdges.push_back({ m_uniqueVertices[5], m_uniqueVertices[4] });
	poly6->triangles.push_back({ m_uniqueVertices[7], m_uniqueVertices[6], m_uniqueVertices[5] });
	poly6->triangles.push_back({ m_uniqueVertices[7], m_uniqueVertices[5], m_uniqueVertices[4] });
	calcNorm(poly6);
	m_polygons.push_back(poly6);

	/* Bounding box struct */
	m_boundingBox.startX = std::numeric_limits<float>::max();
	m_boundingBox.endX = -std::numeric_limits<float>::max();
	m_boundingBox.startY = std::numeric_limits<float>::max();
	m_boundingBox.endY = -std::numeric_limits<float>::max();
	m_boundingBox.startZ = std::numeric_limits<float>::max();
	m_boundingBox.endZ = -std::numeric_limits<float>::max();

	for (auto& v : cubeVertices)
	{
		if (v.x() < m_boundingBox.startX)
			m_boundingBox.startX = v.x();
		if (m_boundingBox.endX < v.x())
			m_boundingBox.endX = v.x();
		if (v.y() < m_boundingBox.startY)
			m_boundingBox.startY = v.y();
		if (m_boundingBox.endY < v.y())
			m_boundingBox.endY = v.y();
		if (v.z() < m_boundingBox.startZ)
			m_boundingBox.startZ = v.z();
		if (m_boundingBox.endZ < v.z())
			m_boundingBox.endZ = v.z();
	}

	float c = Grid2D::HALF_LENGTH - 1.0f;

	float *linesVerticesX = new float[6 * 8]{
		-c, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		-c,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

		-c,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		-c,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

		-c,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		-c, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

		-c, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		-c, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f
	};
	m_linesVerticesX = linesVerticesX;

	float* linesVerticesY = new float[6 * 8]{
		-0.5f, -c, -0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, -c,  0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -c,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -c,  0.5f, 1.0f, 0.0f, 0.0f,

		 0.5f, -c,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -c, -0.5f, 1.0f, 0.0f, 0.0f,

		 0.5f, -c, -0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, -c, -0.5f, 1.0f, 0.0f, 0.0f
	};
	m_linesVerticesY = linesVerticesY;

	float* linesVerticesZ = new float[6 * 8]{
		-0.5f, -0.5f, c, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, c, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, c, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, c, 1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f, c, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, c, 1.0f, 0.0f, 0.0f,

		 0.5f, -0.5f, c, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, c, 1.0f, 0.0f, 0.0f
	};
	m_linesVerticesZ = linesVerticesZ;

	m_linesVerticesX_vbo.allocate(linesVerticesX, 6 * 8 * sizeof(float));
	m_linesVerticesX_vbo.addAttribute<float>(3); // position
	m_linesVerticesX_vbo.addAttribute<float>(3); // color
	m_linesVerticesX_renderable = new BrushRenderable(&m_linesVerticesX_vbo, 8);
	//delete[] linesVerticesX;

	m_linesVerticesY_vbo.allocate(linesVerticesY, 6 * 8 * sizeof(float));
	m_linesVerticesY_vbo.addAttribute<float>(3); // position
	m_linesVerticesY_vbo.addAttribute<float>(3); // color
	m_linesVerticesY_renderable = new BrushRenderable(&m_linesVerticesY_vbo, 8);
	//delete[] linesVerticesY;

	m_linesVerticesZ_vbo.allocate(linesVerticesZ, 6 * 8 * sizeof(float));
	m_linesVerticesZ_vbo.addAttribute<float>(3); // position
	m_linesVerticesZ_vbo.addAttribute<float>(3); // color
	m_linesVerticesZ_renderable = new BrushRenderable(&m_linesVerticesZ_vbo, 8);
	//delete[] linesVerticesZ;

	m_bboxLinesVerticesCount = 8;

	/* Shaders */
	m_program2D = ResourceManager::getProgram("plain_brush_2d", "plain_brush_2d");
	m_program3D = ResourceManager::getProgram("plain_brush_3d", "plain_brush_3d");
	m_programSelection = ResourceManager::getProgram("brush_selection", "brush_selection");

	m_trianglesVbo.addAttribute<float>(3); // position
	m_trianglesVbo.addAttribute<float>(2); // tex coords
	m_trianglesVbo.addAttribute<float>(3); // color
	m_trianglesVbo.addAttribute<float>(3); // normal
	m_trianglesRenderable = new BrushRenderable(&m_trianglesVbo, 0);
	makeTrianglesBufferData();

	m_linesVbo.addAttribute<float>(3); // position
	m_linesVbo.addAttribute<float>(3); // color
	m_linesRenderable = new BrushRenderable(&m_linesVbo, 0);
	makeLinesBufferData();
}

Brush::~Brush()
{
	delete m_linesVerticesX_renderable;
	delete m_linesVerticesY_renderable;
	delete m_linesVerticesZ_renderable;
}

void Brush::makeTrianglesBufferData()
{
	int size = 0;
	m_trianglesVerticesCount = 0;

	for (auto& poly : m_polygons)
	{
		m_trianglesVerticesCount += poly->triangles.size() * 3;
		size += poly->triangles.size() * 3 * 11;
	}

	float *vertices = new float[size];
	int i = 0;

	auto vertexBufferData = [&](Types::Polygon* poly, QVector3D* v) {
		auto& texCoords = poly->verticesMap[v];
		vertices[i++] = v->x();
		vertices[i++] = v->y();
		vertices[i++] = v->z();
		vertices[i++] = texCoords.x();
		vertices[i++] = texCoords.y();
		vertices[i++] = m_uniformColor.x();
		vertices[i++] = m_uniformColor.y();
		vertices[i++] = m_uniformColor.z();
		vertices[i++] = poly->norm.x();
		vertices[i++] = poly->norm.y();
		vertices[i++] = poly->norm.z();
	};

	for (auto& poly : m_polygons)
	{
		for (auto& [v0, v1, v2] : poly->triangles)
		{
			vertexBufferData(poly, v0);
			vertexBufferData(poly, v1);
			vertexBufferData(poly, v2);
		}
	}

	m_trianglesVbo.allocate(vertices, size * sizeof(float));
	m_trianglesRenderable->setVerticesCount(m_trianglesVerticesCount);
	delete[] vertices;
}

void Brush::makeLinesBufferData()
{
	int size = 0;
	m_linesVerticesCount = 0;

	for (auto& poly : m_polygons)
	{
		m_linesVerticesCount += poly->borderEdges.size() * 2;
		size += poly->borderEdges.size() * 2 * 6;
	}

	float* vertices = new float[size];
	int i = 0;

	auto vertexBufferData = [&](Types::Polygon* poly, QVector3D* v) {
		auto& texCoords = poly->verticesMap[v];
		vertices[i++] = v->x();
		vertices[i++] = v->y();
		vertices[i++] = v->z();
		vertices[i++] = m_uniformColor.x();
		vertices[i++] = m_uniformColor.y();
		vertices[i++] = m_uniformColor.z();
	};

	for (auto& poly : m_polygons)
	{
		for (auto& [v0, v1] : poly->borderEdges)
		{
			vertexBufferData(poly, v0);
			vertexBufferData(poly, v1);
		}
	}

	m_linesVbo.allocate(vertices, size * sizeof(float));
	m_linesRenderable->setVerticesCount(m_linesVerticesCount);
	delete[] vertices;
}

void Brush::render3D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_trianglesRenderable);
	float ambient = 0.4f;
	float diffuse = 0.7f;
	float specular = 1.0f;
	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(m_origin);

	useContext(context);
	GLCall(m_program3D->bind());
	GLCall(m_program3D->setUniformValue("u_Proj", proj));
	GLCall(m_program3D->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program3D->setUniformValue("u_Model", model));
	GLCall(m_program3D->setUniformValue("u_UsingColor", true));
	GLCall(m_program3D->setUniformValue("u_ViewPos", camera.getPosition()));
	GLCall(m_program3D->setUniformValue("u_DirLight.direction", -0.1f, -0.5f, -0.2f));
	GLCall(m_program3D->setUniformValue("u_DirLight.ambient", ambient, ambient, ambient));
	GLCall(m_program3D->setUniformValue("u_DirLight.diffuse", diffuse, diffuse, diffuse));
	//GLCall(m_program3D->setUniformValue("u_DirLight.specular", specular, specular, specular));
	GLCall(m_program3D->setUniformValue("u_Material.shininess", 64.0f));
	GLCall(m_program3D->setUniformValue("u_Selected", m_selected));
	GLCall(m_program3D->setUniformValue("u_SelectionColor", m_selectionColor));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_TRIANGLES, 0, m_trianglesVerticesCount));
}

void Brush::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_linesRenderable);
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(zoomVec);
	model.translate(m_origin);

	useContext(context);
	GLCall(m_program2D->bind());
	GLCall(m_program2D->setUniformValue("u_Proj", proj));
	GLCall(m_program2D->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program2D->setUniformValue("u_Model", model));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_LINES, 0, m_linesVerticesCount));

	if (m_selected)
	{
		auto* bbox = &m_boundingBox;
		BrushRenderable* renderable;
		QVector3D scale;
		float* vertices;

		switch (axis)
		{
		case Axis::X:
			renderable = m_linesVerticesX_renderable;
			scale = QVector3D(1.0f, bbox->endY - bbox->startY, bbox->endZ - bbox->startZ);
			vertices = m_linesVerticesX;
			break;
		case Axis::Y:
			renderable = m_linesVerticesY_renderable;
			scale = QVector3D(bbox->endX - bbox->startX, 1.0f, bbox->endZ - bbox->startZ);
			vertices = m_linesVerticesY;
			break;
		case Axis::Z:
			renderable = m_linesVerticesZ_renderable;
			scale = QVector3D(bbox->endX - bbox->startX, bbox->endY - bbox->startY, 1.0f);
			vertices = m_linesVerticesZ;
			break;
		}

		auto vao = GlobalData::getRenderableVAO(*context, *renderable);
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(m_origin * zoomVec);
		model.scale(scale * zoomVec);

		useContext(context);
		GLCall(m_program2D->bind());
		GLCall(m_program2D->setUniformValue("u_Proj", proj));
		GLCall(m_program2D->setUniformValue("u_View", camera.getViewMatrix()));
		GLCall(m_program2D->setUniformValue("u_Model", model));

		GLCall(vao->bind());
		GLCall($->glDrawArrays(GL_LINES, 0, m_bboxLinesVerticesCount));

		/* Draw resizing points */
		QList<QVector3D>* translationVectors = getResizePointsTranslationVectors(axis, factor, zoomVec);

		for (auto& vec : *translationVectors)
		{
			useContext(context);

			model.setToIdentity();
			model.translate(vec);

			m_resizePoint.m_program->bind();
			m_resizePoint.m_program->setUniformValue("proj", proj);
			m_resizePoint.m_program->setUniformValue("view", camera.getViewMatrix());
			m_resizePoint.m_program->setUniformValue("model", model);
			m_resizePoint.m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f);

			m_resizePoint.bindVAO(context);
			$->glDrawArrays(GL_TRIANGLES, 0, m_resizePoint.verticesCount());
		}

		delete translationVectors;
	}
}

Brush::BrushRenderable::BrushRenderable(VertexBufferObject* vbo, int verticesCount)
	: m_vbo(vbo), m_verticesCount(verticesCount)
{
	createVAO(*vbo);
}

void Brush::calcNorm(Types::Polygon* polygon)
{
	auto& tri = polygon->triangles[0];
	polygon->norm = QVector3D::normal(*tri.v0, *tri.v1, *tri.v2);
}

void Brush::writeSelectionBuffer(QOpenGLContext* context, float renderId, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_trianglesRenderable);
	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(m_origin);

	useContext(context);
	GLCall(m_programSelection->bind());
	GLCall(m_programSelection->setUniformValue("u_Proj", proj));
	GLCall(m_programSelection->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_programSelection->setUniformValue("u_Model", model));
	GLCall(m_programSelection->setUniformValue("u_RenderId", renderId));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_TRIANGLES, 0, m_trianglesVerticesCount));
}

QVector2D Brush::get2DOrigin(Axis axis)
{
	switch (axis)
	{
	case Axis::X:
		return QVector2D(m_origin.z(), m_origin.y());
	case Axis::Y:
		return QVector2D(m_origin.x(), m_origin.z());
	case Axis::Z:
		return QVector2D(m_origin.x(), m_origin.y());
	}
}

void Brush::calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps)
{
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	float horStart, horEnd, verStart, verEnd;
	QVector2D origin = get2DOrigin(axis);
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

	QVector3D total(0.0f, 0.0f, 0.0f);

	for (auto& v : m_uniqueVertices)
	{
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(m_origin);
		QVector3D vShifted = model * *v;

		switch (axis)
		{
		case Axis::X:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.z() - horStart) / (horEnd - horStart));
				v->setZ(v->z() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.y() - verStart) / (verEnd - verStart));
				v->setY(v->y() + steps * factor);
			}
			break;
		case Axis::Y:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.x() - horStart) / (horEnd - horStart));
				v->setX(v->x() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.z() - verStart) / (verEnd - verStart));
				v->setZ(v->z() + steps * factor);
			}
			break;
		case Axis::Z:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.x() - horStart) / (horEnd - horStart));
				v->setX(v->x() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.y() - verStart) / (verEnd - verStart));
				v->setY(v->y() + steps * factor);
			}
		}

		total += *v;
	}

	QVector3D shift = total / m_uniqueVertices.size();
	m_origin += shift;

	for (auto& v : m_uniqueVertices)
	{
		*v -= shift;
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

	makeTrianglesBufferData();
	makeLinesBufferData();
}

void Brush::doMoveStep(Axis axis, QVector2D pos, float step)
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
		switch (axis)
		{
		case Axis::X:
			m_origin.setZ(m_origin.z() + stepsX);
			m_origin.setY(m_origin.y() + stepsY);
			m_boundingBox.startZ += stepsX;
			m_boundingBox.endZ += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
			break;
		case Axis::Y:
			m_origin.setX(m_origin.x() + stepsX);
			m_origin.setZ(m_origin.z() + stepsY);
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startZ += stepsY;
			m_boundingBox.endZ += stepsY;
			break;
		case Axis::Z:
			m_origin.setX(m_origin.x() + stepsX);
			m_origin.setY(m_origin.y() + stepsY);
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
		}

		makeTrianglesBufferData();
		makeLinesBufferData();
	}

	m_lastPos = pos;
}
