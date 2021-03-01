#include "Brush.h"
#include "ResourceManager.h"
#include "../common/GlobalData.h"
#include "ResourceManager.h"

Brush::Brush(QList<QVector3D>& cubeVertices, QVector3D color)
	: m_uniformColor(color), m_selectionColor(1.0f, 0.0f, 0.0f)
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

	m_uniqueVertices = cubeVertices;

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
	calcNorm(poly1);
	m_polygons.push_back(poly1);

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
	calcNorm(poly2);
	m_polygons.push_back(poly2);

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
	calcNorm(poly3);
	m_polygons.push_back(poly3);

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
	calcNorm(poly4);
	m_polygons.push_back(poly4);

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
	calcNorm(poly5);
	m_polygons.push_back(poly5);

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
	calcNorm(poly6);
	m_polygons.push_back(poly6);

	m_program2D = ResourceManager::getProgram("plain_brush_2d", "plain_brush_2d");
	m_program3D = ResourceManager::getProgram("plain_brush_3d", "plain_brush_3d");

	makeTrianglesBufferData();
	makeLinesBufferData();
}

Brush::~Brush()
{

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
	m_trianglesVbo.addAttribute<float>(3); // position
	m_trianglesVbo.addAttribute<float>(2); // tex coords
	m_trianglesVbo.addAttribute<float>(3); // color
	m_trianglesVbo.addAttribute<float>(3); // normal
	m_trianglesRenderable = new BrushRenderable(&m_trianglesVbo, m_trianglesVerticesCount);
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
	m_linesVbo.addAttribute<float>(3); // position
	m_linesVbo.addAttribute<float>(3); // color
	m_linesRenderable = new BrushRenderable(&m_linesVbo, m_linesVerticesCount);
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

void Brush::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
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
