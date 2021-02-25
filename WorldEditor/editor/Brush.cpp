#include "Brush.h"
#include "ResourceManager.h"
#include "../common/GlobalData.h"
#include "ResourceManager.h"

Brush::Brush(QList<QVector3D>& cubeVertices, QVector3D color)
	: m_uniformColor(color)
{
	QVector3D total(0.0f, 0.0f, 0.0f);

	for (auto& v : cubeVertices)
	{
		total += v;
	}

	m_origin = total / cubeVertices.size();

	QList<QVector3D*> shiftedVertices;

	for (auto& v : cubeVertices)
	{
		shiftedVertices.push_back(new QVector3D(v - m_origin));
	}

	/* Polygon 1 */
	Types::Polygon* poly1 = new Types::Polygon;
	poly1->verticesMap[shiftedVertices[0]] = QVector2D(0, 0);
	poly1->verticesMap[shiftedVertices[1]] = QVector2D(0, 1);
	poly1->verticesMap[shiftedVertices[2]] = QVector2D(1, 1);
	poly1->verticesMap[shiftedVertices[3]] = QVector2D(1, 0);
	poly1->borderEdges.push_back({ shiftedVertices[0], shiftedVertices[1] });
	poly1->borderEdges.push_back({ shiftedVertices[0], shiftedVertices[3] });
	poly1->borderEdges.push_back({ shiftedVertices[1], shiftedVertices[2] });
	poly1->borderEdges.push_back({ shiftedVertices[2], shiftedVertices[3] });
	poly1->triangles.push_back({ shiftedVertices[0], shiftedVertices[1], shiftedVertices[2] });
	poly1->triangles.push_back({ shiftedVertices[0], shiftedVertices[2], shiftedVertices[3] });
	polygons.push_back(poly1);

	/* Polygon 2 */
	Types::Polygon* poly2 = new Types::Polygon;
	poly2->verticesMap[shiftedVertices[0]] = QVector2D(0, 0);
	poly2->verticesMap[shiftedVertices[6]] = QVector2D(0, 1);
	poly2->verticesMap[shiftedVertices[7]] = QVector2D(1, 1);
	poly2->verticesMap[shiftedVertices[1]] = QVector2D(1, 0);
	poly2->borderEdges.push_back({ shiftedVertices[0], shiftedVertices[1] });
	poly2->borderEdges.push_back({ shiftedVertices[0], shiftedVertices[6] });
	poly2->borderEdges.push_back({ shiftedVertices[6], shiftedVertices[7] });
	poly2->borderEdges.push_back({ shiftedVertices[1], shiftedVertices[7] });
	poly2->triangles.push_back({ shiftedVertices[0], shiftedVertices[6], shiftedVertices[7] });
	poly2->triangles.push_back({ shiftedVertices[0], shiftedVertices[7], shiftedVertices[1] });
	polygons.push_back(poly2);

	/* Polygon 3 */
	Types::Polygon* poly3 = new Types::Polygon;
	poly3->verticesMap[shiftedVertices[1]] = QVector2D(0, 0);
	poly3->verticesMap[shiftedVertices[7]] = QVector2D(0, 1);
	poly3->verticesMap[shiftedVertices[4]] = QVector2D(1, 1);
	poly3->verticesMap[shiftedVertices[2]] = QVector2D(1, 0);
	poly3->borderEdges.push_back({ shiftedVertices[1], shiftedVertices[7] });
	poly3->borderEdges.push_back({ shiftedVertices[1], shiftedVertices[2] });
	poly3->borderEdges.push_back({ shiftedVertices[7], shiftedVertices[4] });
	poly3->borderEdges.push_back({ shiftedVertices[2], shiftedVertices[4] });
	poly3->triangles.push_back({ shiftedVertices[1], shiftedVertices[7], shiftedVertices[4] });
	poly3->triangles.push_back({ shiftedVertices[1], shiftedVertices[4], shiftedVertices[2] });
	polygons.push_back(poly3);

	/* Polygon 4 */
	Types::Polygon* poly4 = new Types::Polygon;
	poly4->verticesMap[shiftedVertices[2]] = QVector2D(0, 0);
	poly4->verticesMap[shiftedVertices[4]] = QVector2D(0, 1);
	poly4->verticesMap[shiftedVertices[5]] = QVector2D(1, 1);
	poly4->verticesMap[shiftedVertices[3]] = QVector2D(1, 0);
	poly4->borderEdges.push_back({ shiftedVertices[2], shiftedVertices[4] });
	poly4->borderEdges.push_back({ shiftedVertices[2], shiftedVertices[3] });
	poly4->borderEdges.push_back({ shiftedVertices[4], shiftedVertices[5] });
	poly4->borderEdges.push_back({ shiftedVertices[3], shiftedVertices[5] });
	poly4->triangles.push_back({ shiftedVertices[2], shiftedVertices[4], shiftedVertices[5] });
	poly4->triangles.push_back({ shiftedVertices[2], shiftedVertices[5], shiftedVertices[3] });
	polygons.push_back(poly4);

	/* Polygon 5 */
	Types::Polygon* poly5 = new Types::Polygon;
	poly5->verticesMap[shiftedVertices[3]] = QVector2D(0, 0);
	poly5->verticesMap[shiftedVertices[5]] = QVector2D(0, 1);
	poly5->verticesMap[shiftedVertices[6]] = QVector2D(1, 1);
	poly5->verticesMap[shiftedVertices[0]] = QVector2D(1, 0);
	poly5->borderEdges.push_back({ shiftedVertices[3], shiftedVertices[5] });
	poly5->borderEdges.push_back({ shiftedVertices[3], shiftedVertices[0] });
	poly5->borderEdges.push_back({ shiftedVertices[5], shiftedVertices[6] });
	poly5->borderEdges.push_back({ shiftedVertices[0], shiftedVertices[6] });
	poly5->triangles.push_back({ shiftedVertices[3], shiftedVertices[5], shiftedVertices[6] });
	poly5->triangles.push_back({ shiftedVertices[3], shiftedVertices[6], shiftedVertices[0] });
	polygons.push_back(poly5);

	/* Polygon 6 */
	Types::Polygon* poly6 = new Types::Polygon;
	poly6->verticesMap[shiftedVertices[7]] = QVector2D(0, 0);
	poly6->verticesMap[shiftedVertices[6]] = QVector2D(0, 1);
	poly6->verticesMap[shiftedVertices[5]] = QVector2D(1, 1);
	poly6->verticesMap[shiftedVertices[4]] = QVector2D(1, 0);
	poly6->borderEdges.push_back({ shiftedVertices[7], shiftedVertices[6] });
	poly6->borderEdges.push_back({ shiftedVertices[7], shiftedVertices[4] });
	poly6->borderEdges.push_back({ shiftedVertices[6], shiftedVertices[5] });
	poly6->borderEdges.push_back({ shiftedVertices[5], shiftedVertices[4] });
	poly6->triangles.push_back({ shiftedVertices[7], shiftedVertices[6], shiftedVertices[5] });
	poly6->triangles.push_back({ shiftedVertices[7], shiftedVertices[5], shiftedVertices[4] });
	polygons.push_back(poly6);

	m_program = ResourceManager::getProgram("plain_brush", "plain_brush");

	makeTrianglesData();
	makeLinesData();
}

Brush::~Brush()
{

}

void Brush::makeBufferData()
{

}

void Brush::makeTrianglesData()
{
	int size = 0;
	m_trianglesVerticesCount = 0;

	for (auto& poly : polygons)
	{
		m_trianglesVerticesCount += poly->triangles.size() * 3;
		size += poly->triangles.size() * 3 * 8;
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
	};

	for (auto& poly : polygons)
	{
		for (auto& [v0, v1, v2] : poly->triangles)
		{
			vertexBufferData(poly, v0);
			vertexBufferData(poly, v1);
			vertexBufferData(poly, v2);
		}
	}

	m_trianglesVbo.allocate(vertices, size * sizeof(float));
	m_trianglesVbo.addAttribute<float>(3); // position
	m_trianglesVbo.addAttribute<float>(2); // tex coords
	m_trianglesVbo.addAttribute<float>(3); // color
	m_trianglesRenderable = new BrushRenderable(&m_trianglesVbo, m_trianglesVerticesCount);
	delete[] vertices;
}

void Brush::makeLinesData()
{
	int size = 0;
	m_linesVerticesCount = 0;

	for (auto& poly : polygons)
	{
		m_linesVerticesCount += poly->borderEdges.size() * 2;
		size += poly->borderEdges.size() * 2 * 8;
	}

	float* vertices = new float[size];
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
	};

	for (auto& poly : polygons)
	{
		for (auto& [v0, v1] : poly->borderEdges)
		{
			vertexBufferData(poly, v0);
			vertexBufferData(poly, v1);
		}
	}

	m_linesVbo.allocate(vertices, size * sizeof(float));
	m_linesVbo.addAttribute<float>(3); // position
	m_linesVbo.addAttribute<float>(2); // tex coords
	m_linesVbo.addAttribute<float>(3); // color
	m_linesRenderable = new BrushRenderable(&m_linesVbo, m_linesVerticesCount);
	delete[] vertices;
}

void Brush::render3D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_trianglesRenderable);
	QMatrix4x4 model;
	model.setToIdentity();
	model.translate(m_origin);

	useContext(context);
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("u_Proj", proj));
	GLCall(m_program->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("u_Model", model));
	GLCall(m_program->setUniformValue("u_UsingColor", true));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_TRIANGLES, 0, m_trianglesVerticesCount));
}

void Brush::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_linesRenderable);
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(zoomVec);
	model.translate(m_origin);
	

	useContext(context);
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("u_Proj", proj));
	GLCall(m_program->setUniformValue("u_View", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("u_Model", model));
	GLCall(m_program->setUniformValue("u_UsingColor", true));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_LINES, 0, m_linesVerticesCount));
}

Brush::BrushRenderable::BrushRenderable(VertexBufferObject* vbo, int verticesCount)
	: m_vbo(vbo), m_verticesCount(verticesCount)
{
	createVAO(*vbo);
}
