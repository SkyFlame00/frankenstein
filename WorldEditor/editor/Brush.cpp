#include "Brush.h"
#include "ResourceManager.h"
#include "../common/GlobalData.h"
#include "ResourceManager.h"
#include <limits>
#include "Grid2D.h"
#include "../common/helpers.h"
#include <QtMath>
#include "../common/constants.h"

Brush::Brush(QList<QVector3D>& cubeVertices, Texture& texture, bool isUsingColor)
	: m_selectionColor(1.0f, 0.0f, 0.0f),
	m_resizePoint(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f),
	m_isUsingColor(isUsingColor), m_defaultTexture(texture)
{
	QVector3D color(Helpers::getRandom(), Helpers::getRandom(), Helpers::getRandom());
	m_uniformColor = color;
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

	/* Unique edges */
	m_uniqueEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	m_uniqueEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	m_uniqueEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	m_uniqueEdges.push_back({ m_uniqueVertices[3], m_uniqueVertices[0] });

	m_uniqueEdges.push_back({ m_uniqueVertices[6], m_uniqueVertices[7] });
	m_uniqueEdges.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	m_uniqueEdges.push_back({ m_uniqueVertices[4], m_uniqueVertices[5] });
	m_uniqueEdges.push_back({ m_uniqueVertices[5], m_uniqueVertices[6] });

	m_uniqueEdges.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	m_uniqueEdges.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	m_uniqueEdges.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	m_uniqueEdges.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });

	float cos, acos;

	/* Polygon 1 */
	Types::Polygon* poly1 = new Types::Polygon;
	poly1->vertices.push_back(m_uniqueVertices[0]);
	poly1->vertices.push_back(m_uniqueVertices[1]);
	poly1->vertices.push_back(m_uniqueVertices[2]);
	poly1->vertices.push_back(m_uniqueVertices[3]);
	poly1->edges.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	poly1->edges.push_back({ m_uniqueVertices[0], m_uniqueVertices[3] });
	poly1->edges.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly1->edges.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly1->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[1], m_uniqueVertices[2] });
	poly1->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[2], m_uniqueVertices[3] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[0] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[2] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[3] });
	poly1->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly1->isUsingColor = isUsingColor;
	poly1->color = color;
	poly1->textureId = texture.id;
	poly1->textureWidth = texture.width;
	poly1->textureHeight = texture.height;
	poly1->scale = QVector2D(1.0f, 1.0f);
	poly1->shift = QVector2D(0.0f, 0.0f);
	poly1->verticesMap[m_uniqueVertices[0]] = QVector2D(0, 0);
	poly1->verticesMap[m_uniqueVertices[1]] = QVector2D(0, 1);
	poly1->verticesMap[m_uniqueVertices[2]] = QVector2D(1, 1);
	poly1->verticesMap[m_uniqueVertices[3]] = QVector2D(1, 0);
	calcNorm(poly1);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[0] - *m_uniqueVertices[1], *m_uniqueVertices[0] - *m_uniqueVertices[2]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[0]);

		poly1->norm = norm;
		poly1->a = a;
		poly1->b = b;
		poly1->c = c;
		poly1->d = d;
	}
	m_polygons.push_back(poly1);

	/* Polygon 2 */
	Types::Polygon* poly2 = new Types::Polygon;
	poly2->vertices.push_back(m_uniqueVertices[0]);
	poly2->vertices.push_back(m_uniqueVertices[6]);
	poly2->vertices.push_back(m_uniqueVertices[7]);
	poly2->vertices.push_back(m_uniqueVertices[1]);
	poly2->edges.push_back({ m_uniqueVertices[0], m_uniqueVertices[1] });
	poly2->edges.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	poly2->edges.push_back({ m_uniqueVertices[6], m_uniqueVertices[7] });
	poly2->edges.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	poly2->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[6], m_uniqueVertices[7] });
	poly2->triangles.push_back({ m_uniqueVertices[0], m_uniqueVertices[7], m_uniqueVertices[1] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[6], m_uniqueVertices[7] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[7] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[0], m_uniqueVertices[7] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[1] });
	poly2->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[0] });
	poly2->isUsingColor = isUsingColor;
	poly2->color = color;
	poly2->textureId = texture.id;
	poly2->textureWidth = texture.width;
	poly2->textureHeight = texture.height;
	poly2->scale = QVector2D(1.0f, 1.0f);
	poly2->shift = QVector2D(0.0f, 0.0f);
	poly2->verticesMap[m_uniqueVertices[0]] = QVector2D(0, 0);
	poly2->verticesMap[m_uniqueVertices[6]] = QVector2D(0, 1);
	poly2->verticesMap[m_uniqueVertices[7]] = QVector2D(1, 1);
	poly2->verticesMap[m_uniqueVertices[1]] = QVector2D(1, 0);
	calcNorm(poly2);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[0] - *m_uniqueVertices[6], *m_uniqueVertices[0] - *m_uniqueVertices[7]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[0]);

		poly2->norm = norm;
		poly2->a = a;
		poly2->b = b;
		poly2->c = c;
		poly2->d = d;
	}
	m_polygons.push_back(poly2);

	/* Polygon 3 */
	Types::Polygon* poly3 = new Types::Polygon;
	poly3->vertices.push_back(m_uniqueVertices[1]);
	poly3->vertices.push_back(m_uniqueVertices[7]);
	poly3->vertices.push_back(m_uniqueVertices[4]);
	poly3->vertices.push_back(m_uniqueVertices[2]);
	poly3->edges.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	poly3->edges.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly3->edges.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly3->edges.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	poly3->triangles.push_back({ m_uniqueVertices[1], m_uniqueVertices[7], m_uniqueVertices[4] });
	poly3->triangles.push_back({ m_uniqueVertices[1], m_uniqueVertices[4], m_uniqueVertices[2] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[7] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[4] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[4] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[4], m_uniqueVertices[2] });
	poly3->trianglesLines.push_back({ m_uniqueVertices[1], m_uniqueVertices[2] });
	poly3->isUsingColor = isUsingColor;
	poly3->color = color;
	poly3->textureId = texture.id;
	poly3->textureWidth = texture.width;
	poly3->textureHeight = texture.height;
	poly3->scale = QVector2D(1.0f, 1.0f);
	poly3->shift = QVector2D(0.0f, 0.0f);
	poly3->verticesMap[m_uniqueVertices[1]] = QVector2D(0, 0);
	poly3->verticesMap[m_uniqueVertices[7]] = QVector2D(0, 1);
	poly3->verticesMap[m_uniqueVertices[4]] = QVector2D(1, 1);
	poly3->verticesMap[m_uniqueVertices[2]] = QVector2D(1, 0);
	calcNorm(poly3);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[1] - *m_uniqueVertices[7], *m_uniqueVertices[1] - *m_uniqueVertices[4]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[1]);

		poly3->norm = norm;
		poly3->a = a;
		poly3->b = b;
		poly3->c = c;
		poly3->d = d;
	}
	m_polygons.push_back(poly3);

	/* Polygon 4 */
	Types::Polygon* poly4 = new Types::Polygon;
	poly4->vertices.push_back(m_uniqueVertices[2]);
	poly4->vertices.push_back(m_uniqueVertices[4]);
	poly4->vertices.push_back(m_uniqueVertices[5]);
	poly4->vertices.push_back(m_uniqueVertices[3]);
	poly4->edges.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	poly4->edges.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly4->edges.push_back({ m_uniqueVertices[4], m_uniqueVertices[5] });
	poly4->edges.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });
	poly4->triangles.push_back({ m_uniqueVertices[2], m_uniqueVertices[4], m_uniqueVertices[5] });
	poly4->triangles.push_back({ m_uniqueVertices[2], m_uniqueVertices[5], m_uniqueVertices[3] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[4] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[4], m_uniqueVertices[5] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[5] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[5] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[5], m_uniqueVertices[3] });
	poly4->trianglesLines.push_back({ m_uniqueVertices[2], m_uniqueVertices[3] });
	poly4->isUsingColor = isUsingColor;
	poly4->color = color;
	poly4->textureId = texture.id;
	poly4->textureWidth = texture.width;
	poly4->textureHeight = texture.height;
	poly4->scale = QVector2D(1.0f, 1.0f);
	poly4->shift = QVector2D(0.0f, 0.0f);
	poly4->verticesMap[m_uniqueVertices[2]] = QVector2D(0, 0);
	poly4->verticesMap[m_uniqueVertices[4]] = QVector2D(0, 1);
	poly4->verticesMap[m_uniqueVertices[5]] = QVector2D(1, 1);
	poly4->verticesMap[m_uniqueVertices[3]] = QVector2D(1, 0);
	calcNorm(poly4);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[2] - *m_uniqueVertices[4], *m_uniqueVertices[2] - *m_uniqueVertices[5]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[2]);

		poly4->norm = norm;
		poly4->a = a;
		poly4->b = b;
		poly4->c = c;
		poly4->d = d;
	}
	m_polygons.push_back(poly4);

	/* Polygon 5 */
	Types::Polygon* poly5 = new Types::Polygon;
	poly5->vertices.push_back(m_uniqueVertices[3]);
	poly5->vertices.push_back(m_uniqueVertices[5]);
	poly5->vertices.push_back(m_uniqueVertices[6]);
	poly5->vertices.push_back(m_uniqueVertices[0]);
	poly5->edges.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });
	poly5->edges.push_back({ m_uniqueVertices[3], m_uniqueVertices[0] });
	poly5->edges.push_back({ m_uniqueVertices[5], m_uniqueVertices[6] });
	poly5->edges.push_back({ m_uniqueVertices[0], m_uniqueVertices[6] });
	poly5->triangles.push_back({ m_uniqueVertices[3], m_uniqueVertices[5], m_uniqueVertices[6] });
	poly5->triangles.push_back({ m_uniqueVertices[3], m_uniqueVertices[6], m_uniqueVertices[0] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[3], m_uniqueVertices[5] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[5], m_uniqueVertices[6] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[3], m_uniqueVertices[6] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[3], m_uniqueVertices[6] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[6], m_uniqueVertices[0] });
	poly5->trianglesLines.push_back({ m_uniqueVertices[3], m_uniqueVertices[0] });
	poly5->isUsingColor = isUsingColor;
	poly5->color = color;
	poly5->textureId = texture.id;
	poly5->textureWidth = texture.width;
	poly5->textureHeight = texture.height;
	poly5->scale = QVector2D(1.0f, 1.0f);
	poly5->shift = QVector2D(0.0f, 0.0f);
	poly5->verticesMap[m_uniqueVertices[3]] = QVector2D(0, 0);
	poly5->verticesMap[m_uniqueVertices[5]] = QVector2D(0, 1);
	poly5->verticesMap[m_uniqueVertices[6]] = QVector2D(1, 1);
	poly5->verticesMap[m_uniqueVertices[0]] = QVector2D(1, 0);
	calcNorm(poly5);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[3] - *m_uniqueVertices[5], *m_uniqueVertices[3] - *m_uniqueVertices[6]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[3]);

		poly5->norm = norm;
		poly5->a = a;
		poly5->b = b;
		poly5->c = c;
		poly5->d = d;
	}
	m_polygons.push_back(poly5);

	/* Polygon 6 */
	Types::Polygon* poly6 = new Types::Polygon;
	poly6->vertices.push_back(m_uniqueVertices[7]);
	poly6->vertices.push_back(m_uniqueVertices[6]);
	poly6->vertices.push_back(m_uniqueVertices[5]);
	poly6->vertices.push_back(m_uniqueVertices[4]);
	poly6->edges.push_back({ m_uniqueVertices[7], m_uniqueVertices[6] });
	poly6->edges.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly6->edges.push_back({ m_uniqueVertices[6], m_uniqueVertices[5] });
	poly6->edges.push_back({ m_uniqueVertices[5], m_uniqueVertices[4] });
	poly6->triangles.push_back({ m_uniqueVertices[7], m_uniqueVertices[6], m_uniqueVertices[5] });
	poly6->triangles.push_back({ m_uniqueVertices[7], m_uniqueVertices[5], m_uniqueVertices[4] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[6] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[6], m_uniqueVertices[5] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[5] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[5] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[5], m_uniqueVertices[4] });
	poly6->trianglesLines.push_back({ m_uniqueVertices[7], m_uniqueVertices[4] });
	poly6->isUsingColor = isUsingColor;
	poly6->color = color;
	poly6->textureId = texture.id;
	poly6->textureWidth = texture.width;
	poly6->textureHeight = texture.height;
	poly6->scale = QVector2D(1.0f, 1.0f);
	poly6->shift = QVector2D(0.0f, 0.0f);
	poly6->verticesMap[m_uniqueVertices[7]] = QVector2D(0, 0);
	poly6->verticesMap[m_uniqueVertices[6]] = QVector2D(0, 1);
	poly6->verticesMap[m_uniqueVertices[5]] = QVector2D(1, 1);
	poly6->verticesMap[m_uniqueVertices[4]] = QVector2D(1, 0);
	calcNorm(poly6);
	{
		/* Calculate norm and plane coefficients */
		QVector3D norm = QVector3D::crossProduct(*m_uniqueVertices[7] - *m_uniqueVertices[6], *m_uniqueVertices[7] - *m_uniqueVertices[5]);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *m_uniqueVertices[7]);

		poly6->norm = norm;
		poly6->a = a;
		poly6->b = b;
		poly6->c = c;
		poly6->d = d;
	}
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

	for (auto* polygon : m_polygons)
	{
		QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
		polygon->minX = std::numeric_limits<float>::max();
		polygon->minY = std::numeric_limits<float>::max();

		for (auto v : polygon->vertices)
		{
			QVector3D pos = model * *v;

			if (pos.x() < polygon->minX)
				polygon->minX = pos.x();
			if (pos.y() < polygon->minY)
				polygon->minY = pos.y();
		}
	}

	for (auto* polygon : m_polygons)
		calcTexCoords(polygon);

	setupTextures();
	setup();
}

Brush::Brush(Polyhedron_3& polyhedron, Brush* parentBrush)
	: m_selectionColor(1.0f, 0.0f, 0.0f),
	m_resizePoint(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f)
{
	m_uniformColor = parentBrush->m_uniformColor;
	m_isUsingColor = parentBrush->m_isUsingColor;
	m_defaultTexture = parentBrush->m_defaultTexture;

	auto tryAddingUniqueVertex = [&](QVector3D& vertex) {
		auto res = std::find_if(m_uniqueVertices.begin(), m_uniqueVertices.end(), [&](QVector3D* v) {
			return vertex.x() == v->x() && vertex.y() == v->y() && vertex.z() == v->z();
			});

		if (res != m_uniqueVertices.end())
			return *res;

		QVector3D* uniqueVertex = new QVector3D(vertex);
		m_uniqueVertices.push_back(uniqueVertex);
		return uniqueVertex;
	};

	auto tryAddingPolygon = [&](float a, float b, float c, float d, QVector3D norm) {
		auto res = std::find_if(m_polygons.begin(), m_polygons.end(), [&](Types::Polygon* poly) {
			return Helpers::areEqual(poly->norm, norm);
			});

		if (res != m_polygons.end())
			return *res;

		Types::Polygon* poly = new Types::Polygon{ a, b, c, d };
		poly->norm = norm;
		m_polygons.push_back(poly);
		return poly;
	};

	auto tryAddingVertexToPolygon = [&](Types::Polygon* poly, QVector3D* vertex) {
		auto res = std::find_if(poly->vertices.begin(), poly->vertices.end(), [&](QVector3D* v) {
			return vertex == v;
			});

		if (res == poly->vertices.end())
			poly->vertices.push_back(vertex);
	};
	
	auto tryAddingUniqueEdge = [&](QVector3D* v0, QVector3D* v1) {
		auto res = std::find_if(m_uniqueEdges.begin(), m_uniqueEdges.end(), [&](Types::Edge edge) {
			return
				v0 == edge.v0 && v1 == edge.v1 ||
				v1 == edge.v0 && v0 == edge.v1;
			});

		if (res == m_uniqueEdges.end())
			m_uniqueEdges.push_back({ v0, v1 });
	};

	Polyhedron_3::Facet_iterator fit;

	/* Create polygons */
	for (fit = polyhedron.facets_begin(); fit != polyhedron.facets_end(); fit++)
	{
		Polyhedron_3::Facet facet = *fit;
		auto v1_raw = facet.halfedge()->vertex()->point();
		auto v2_raw = facet.halfedge()->next()->vertex()->point();
		auto v3_raw = facet.halfedge()->opposite()->vertex()->point();

		QVector3D* v1 = tryAddingUniqueVertex(QVector3D(v1_raw[0], v1_raw[1], v1_raw[2]));
		QVector3D* v2 = tryAddingUniqueVertex(QVector3D(v2_raw[0], v2_raw[1], v2_raw[2]));
		QVector3D* v3 = tryAddingUniqueVertex(QVector3D(v3_raw[0], v3_raw[1], v3_raw[2]));

		QVector3D norm = QVector3D::crossProduct(*v1 - *v2, *v1 - *v3);
		norm.normalize();
		float a = norm.x();
		float b = norm.y();
		float c = norm.z();
		float d = -QVector3D::dotProduct(norm, *v1);
		auto polygon = tryAddingPolygon(a, b, c, d, norm);

		Types::Triangle triangle{ v1, v2, v3 };
		polygon->triangles.push_back(triangle);
		tryAddingVertexToPolygon(polygon, v1);
		tryAddingVertexToPolygon(polygon, v2);
		tryAddingVertexToPolygon(polygon, v3);
		polygon->trianglesLines.push_back({ v1, v2 });
		polygon->trianglesLines.push_back({ v2, v3 });
		polygon->trianglesLines.push_back({ v1, v3 });
	}

	/* Sort polygons' vertices in the clockwise order */
	for (auto* polygon : m_polygons)
	{
		QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
		QVector2D centroid(0, 0);
		QVector2D total(0, 0);

		for (auto pt : polygon->vertices)
		{
			QVector4D v = model * QVector4D(*pt, 1.0f);
			total += QVector2D(v.x(), v.y());
		}

		centroid = total / polygon->vertices.size();
		centroid += QVector2D(0.01f, 0.01f);

		std::sort(polygon->vertices.begin(), polygon->vertices.end(), [&](QVector3D* v1, QVector3D* v2)
			{
				QVector4D _v1 = model * QVector4D(*v1, 1.0f);
				QVector4D _v2 = model * QVector4D(*v2, 1.0f);

				float a1 = qAtan2(_v1.x() - centroid.x(), _v1.y() - centroid.y());
				float a2 = qAtan2(_v2.x() - centroid.x(), _v2.y() - centroid.y());

				while (a1 >= 360.0f)
					a1 -= 360.0f;
				while (a2 >= 360.0f)
					a2 -= 360.0f;

				return a1 - a2 > 0;
			}
		);
	}

	/* Create unique edges */
	for (auto* polygon : m_polygons)
	{
		int size = polygon->vertices.size();
		for (int i = 0; i < size; i++)
		{
			int j = i == size - 1 ? 0 : i + 1;
			tryAddingUniqueEdge(polygon->vertices[i], polygon->vertices[j]);
			polygon->edges.push_back({ polygon->vertices[i], polygon->vertices[j] });
		}
	}

	/* Set polygons' texture coordinates */
	for (auto* polygon : m_polygons)
	{
		auto& oldPolygons = parentBrush->getPolygons();
		auto res = std::find_if(oldPolygons.begin(), oldPolygons.end(), [&](Types::Polygon* oldPolygon)
			{
				return Helpers::areEqual(polygon->a, oldPolygon->a) &&
					Helpers::areEqual(polygon->b, oldPolygon->b) &&
					Helpers::areEqual(polygon->c, oldPolygon->c) &&
					Helpers::areEqual(polygon->d, oldPolygon->d);
			});

		if (res != oldPolygons.end())
		{
			auto* oldPolygon = *res;
			polygon->isUsingColor = oldPolygon->isUsingColor;
			polygon->textureId = oldPolygon->textureId;
			polygon->textureWidth = oldPolygon->textureWidth;
			polygon->textureHeight = oldPolygon->textureHeight;
			polygon->scale = oldPolygon->scale;
			polygon->shift = oldPolygon->shift;
			polygon->rotationAngle = oldPolygon->rotationAngle;

			for (auto* v : polygon->vertices)
			{
				auto res = std::find_if(oldPolygon->vertices.begin(), oldPolygon->vertices.end(), [&](QVector3D* oldVertex)
					{
						return Helpers::areEqual(*v, *oldVertex, 0.000001f);
					});

				if (res != oldPolygon->vertices.end())
				{
					polygon->verticesMap[v] = oldPolygon->verticesMap[*res];
				}
				else
				{
					for (auto& edge : oldPolygon->edges)
					{
						QVector3D res = QVector3D::crossProduct(*edge.v0 - *v, *edge.v1 - *v);

						if (Helpers::areEqual(res, QVector3D(0.0f, 0.0f, 0.0f)))
						{
							QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
							QVector3D v0_pos = model * *edge.v0;
							QVector3D v1_pos = model * *edge.v1;
							QVector3D target_pos = model * *v;

							/* For x */
							float v0_texX = oldPolygon->verticesMap[edge.v0].x();
							float v1_texX = oldPolygon->verticesMap[edge.v1].x();
							float len_x = std::abs(v0_texX - v1_texX);
							float texX;

							if (Helpers::areEqual(len_x, 0.0f))
							{
								texX = v0_texX;
							}
							else
							{
								float x_d1 = std::abs(target_pos.x() - v0_pos.x());
								float x_d2 = std::abs(target_pos.x() - v1_pos.x());
								float x_ratio = x_d1 / x_d2;
								float x_d1_tex = len_x * x_ratio / (1 + x_ratio);

								if (v1_pos.x() - v0_pos.x() > 0)
									texX = v0_texX + x_d1_tex;
								else
									texX = v0_texX - x_d1_tex;
							}

							/* For y */
							float v0_texY = oldPolygon->verticesMap[edge.v0].y();
							float v1_texY = oldPolygon->verticesMap[edge.v1].y();
							float len_y = std::abs(v0_texY - v1_texY);
							float texY;

							if (Helpers::areEqual(len_y, 0.0f))
							{
								texY = v0_texY;
							}
							else
							{
								float y_d1 = std::abs(target_pos.y() - v0_pos.y());
								float y_d2 = std::abs(target_pos.y() - v1_pos.y());
								float y_ratio = y_d1 / y_d2;
								float y_d1_tex = len_y * y_ratio / (1 + y_ratio);

								if (v1_pos.y() - v0_pos.y() > 0)
									texY = v0_texY + y_d1_tex;
								else
									texY = v0_texY - y_d1_tex;
							}

							polygon->verticesMap[v] = QVector2D(texX, texY);
						}
					}
				}
			}
		}
		else
		{
			polygon->isUsingColor = m_isUsingColor;
			polygon->textureId = m_defaultTexture.id;
			polygon->textureWidth = m_defaultTexture.width;
			polygon->textureHeight = m_defaultTexture.height;
			polygon->scale = QVector2D(1.0f, 1.0f);
			polygon->shift = QVector2D(0.0f, 0.0f);
			polygon->rotationAngle = 0;

			QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
			polygon->minX = std::numeric_limits<float>::max();
			polygon->minY = std::numeric_limits<float>::max();

			for (auto v : polygon->vertices)
			{
				QVector3D pos = model * *v;

				if (pos.x() < polygon->minX)
					polygon->minX = pos.x();
				if (pos.y() < polygon->minY)
					polygon->minY = pos.y();
			}

			polygon->scale = QVector2D(1.0f, 1.0f);
			polygon->shift = QVector2D(0.0f, 0.0f);
			calcTexCoords(polygon);
		}
	}

	/* Calculate new origin */
	QVector3D total(0.0f, 0.0f, 0.0f);
	QVector3D shift;

	for (auto* v : m_uniqueVertices)
	{
		total += *v;
	}

	shift = total / m_uniqueVertices.size();
	m_origin = parentBrush->m_origin + shift;

	for (auto* v : m_uniqueVertices)
	{
		*v -= shift;
	}

	calcBoundingBox();
	recalcParams();
	setupTextures();
	setup();
}

Brush::Brush(Types::BrushJSON& brushData)
	: m_selectionColor(1.0f, 0.0f, 0.0f)
	, m_resizePoint(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f)
{
	m_uniformColor = QVector3D(brushData.color.r, brushData.color.g, brushData.color.b);
	m_origin = QVector3D(brushData.origin.x, brushData.origin.y, brushData.origin.z);
	m_isUsingColor = false;

	Texture defaultTexture;
	if (brushData.is_default_texture_missing)
		defaultTexture = ResourceManager::getMissingTexture();
	else
		defaultTexture = ResourceManager::getTexture(brushData.default_texture_path.c_str());
	m_defaultTexture = defaultTexture;

	std::unordered_map<int, QVector3D*> indexMap;
	int i = 0;

	for (auto& vData : brushData.unique_vertices)
	{
		QVector3D* v = new QVector3D(vData.x, vData.y, vData.z);
		indexMap[i++] = v;
		m_uniqueVertices.append(v);
	}

	for (auto& eData : brushData.unique_edges)
	{
		Types::Edge edge{ indexMap[eData.v0], indexMap[eData.v1] };
		m_uniqueEdges.append(edge);
	}

	for (auto& polygonData : brushData.polygons)
	{
		Types::Polygon* polygon = new Types::Polygon;

		for (auto& index : polygonData.vertices)
		{
			polygon->vertices.append(indexMap[index]);
		}

		for (auto& edge : polygonData.edges)
		{
			Types::Edge e{ indexMap[edge.v0], indexMap[edge.v1] };
			polygon->edges.append(e);
		}

		for (auto& edge : polygonData.trianglesLines)
		{
			Types::Edge e{ indexMap[edge.v0], indexMap[edge.v1] };
			polygon->trianglesLines.append(e);
		}

		for (auto& triangle : polygonData.triangles)
		{
			Types::Triangle t{ indexMap[triangle.v0], indexMap[triangle.v1], indexMap[triangle.v2] };
			polygon->triangles.append(t);
		}

		for (auto& tc : polygonData.tex_coords)
		{
			polygon->verticesMap[indexMap[tc.vertex_index]] = QVector2D(tc.u, tc.v);
		}

		Texture texture;
		if (polygonData.is_texture_missing)
			texture = ResourceManager::getMissingTexture();
		else
			texture = ResourceManager::getTexture(polygonData.texture_path.c_str());

		polygon->isUsingColor = false;
		polygon->color = QVector3D(brushData.color.r, brushData.color.g, brushData.color.b);
		polygon->textureId = texture.id;
		polygon->textureWidth = texture.width;
		polygon->textureHeight = texture.height;
		polygon->scale = QVector2D(polygonData.scale.x, polygonData.scale.y);
		polygon->shift = QVector2D(polygonData.shift.x, polygonData.shift.y);

		calcNorm(polygon);
		{
			/* Calculate norm and plane coefficients */
			QVector3D norm(polygonData.norm.x, polygonData.norm.y, polygonData.norm.z);
			float a = norm.x();
			float b = norm.y();
			float c = norm.z();
			float d = -QVector3D::dotProduct(norm, *polygon->vertices[0]);

			polygon->norm = norm;
			polygon->a = a;
			polygon->b = b;
			polygon->c = c;
			polygon->d = d;
		}

		m_polygons.push_back(polygon);
	}

	calcBoundingBox();
	setupTextures();
	setup();
}

Brush::~Brush()
{
	delete m_linesVerticesX_renderable;
	delete m_linesVerticesY_renderable;
	delete m_linesVerticesZ_renderable;
	delete m_linesRenderable;
	delete m_trianglesRenderable;
	delete m_trianglesLinesRenderable;

	if (m_clippedBrush)
		delete m_clippedBrush;
	if (m_remainingBrush)
		delete m_remainingBrush;

	for (auto* v : m_uniqueVertices)
		delete v;

	for (auto* p : m_polygons)
		delete p;
}

void Brush::calcBoundingBox()
{
	/* Bounding box struct */
	m_boundingBox.startX = std::numeric_limits<float>::max();
	m_boundingBox.endX = -std::numeric_limits<float>::max();
	m_boundingBox.startY = std::numeric_limits<float>::max();
	m_boundingBox.endY = -std::numeric_limits<float>::max();
	m_boundingBox.startZ = std::numeric_limits<float>::max();
	m_boundingBox.endZ = -std::numeric_limits<float>::max();

	for (auto& v : m_uniqueVertices)
	{
		if (v->x() < m_boundingBox.startX)
			m_boundingBox.startX = v->x();
		if (m_boundingBox.endX < v->x())
			m_boundingBox.endX = v->x();
		if (v->y() < m_boundingBox.startY)
			m_boundingBox.startY = v->y();
		if (m_boundingBox.endY < v->y())
			m_boundingBox.endY = v->y();
		if (v->z() < m_boundingBox.startZ)
			m_boundingBox.startZ = v->z();
		if (m_boundingBox.endZ < v->z())
			m_boundingBox.endZ = v->z();
	}

	m_boundingBox.startX += m_origin.x();
	m_boundingBox.endX += m_origin.x();
	m_boundingBox.startY += m_origin.y();
	m_boundingBox.endY += m_origin.y();
	m_boundingBox.startZ += m_origin.z();
	m_boundingBox.endZ += m_origin.z();
}

void Brush::setup()
{
	float c = Grid2D::HALF_LENGTH - 1.0f;

	float* linesVerticesX = new float[6 * 8]{
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
	delete[] linesVerticesX;

	m_linesVerticesY_vbo.allocate(linesVerticesY, 6 * 8 * sizeof(float));
	m_linesVerticesY_vbo.addAttribute<float>(3); // position
	m_linesVerticesY_vbo.addAttribute<float>(3); // color
	m_linesVerticesY_renderable = new BrushRenderable(&m_linesVerticesY_vbo, 8);
	delete[] linesVerticesY;

	m_linesVerticesZ_vbo.allocate(linesVerticesZ, 6 * 8 * sizeof(float));
	m_linesVerticesZ_vbo.addAttribute<float>(3); // position
	m_linesVerticesZ_vbo.addAttribute<float>(3); // color
	m_linesVerticesZ_renderable = new BrushRenderable(&m_linesVerticesZ_vbo, 8);
	delete[] linesVerticesZ;

	m_bboxLinesVerticesCount = 8;

	/* Shaders */
	m_program2D = ResourceManager::getProgram("plain_brush_2d", "plain_brush_2d");
	m_program3D = ResourceManager::getProgram("plain_brush_3d", "plain_brush_3d");
	m_programSelection = ResourceManager::getProgram("brush_selection", "brush_selection");

	m_trianglesVbo.addAttribute<float>(3); // position
	m_trianglesVbo.addAttribute<float>(2); // tex coords
	m_trianglesVbo.addAttribute<float>(3); // color
	m_trianglesVbo.addAttribute<float>(3); // normal
	m_trianglesVbo.addAttribute<float>(1); // is polygon selected
	m_trianglesVbo.addAttribute<float>(1); // is using color
	m_trianglesVbo.addAttribute<float>(1); // texture id
	m_trianglesRenderable = new BrushRenderable(&m_trianglesVbo, 0);
	makeTrianglesBufferData();

	m_linesVbo.addAttribute<float>(3); // position
	m_linesVbo.addAttribute<float>(3); // color
	m_linesRenderable = new BrushRenderable(&m_linesVbo, 0);
	makeLinesBufferData();

	m_trianglesLinesVbo.addAttribute<float>(3); // position
	m_trianglesLinesVbo.addAttribute<float>(3); // color
	m_trianglesLinesRenderable = new BrushRenderable(&m_trianglesLinesVbo, 0);
	makeTrianglesLinesBufferData();
}

void Brush::makeTrianglesBufferData()
{
	int size = 0;
	m_trianglesVerticesCount = 0;

	for (auto& poly : m_polygons)
	{
		m_trianglesVerticesCount += poly->triangles.size() * 3;
		size += poly->triangles.size() * 3 * m_trianglesVbo.componentsCount();
	}

	float *vertices = new float[size];
	int i = 0;

	for (auto* polygon : m_polygons)
	{
		polygon->begin = i * sizeof(float);
		makePolygonVertices(polygon, i, vertices);
	}

	m_trianglesVbo.allocate(vertices, size * sizeof(float));
	m_trianglesRenderable->setVerticesCount(m_trianglesVerticesCount);
	delete[] vertices;
}

void Brush::makeLinesBufferData()
{
	int size = 0;
	m_linesVerticesCount = 0;

	m_linesVerticesCount += m_uniqueEdges.size() * 2;
	size += m_uniqueEdges.size() * 2 * 6;

	float* vertices = new float[size];
	int i = 0;

	auto vertexBufferData = [&](QVector3D* v) {
		vertices[i++] = v->x();
		vertices[i++] = v->y();
		vertices[i++] = v->z();
		vertices[i++] = m_uniformColor.x();
		vertices[i++] = m_uniformColor.y();
		vertices[i++] = m_uniformColor.z();
	};

	for (auto& [v0, v1] : m_uniqueEdges)
	{
		vertexBufferData(v0);
		vertexBufferData(v1);
	}

	m_linesVbo.allocate(vertices, size * sizeof(float));
	m_linesRenderable->setVerticesCount(m_linesVerticesCount);
	delete[] vertices;
}

void Brush::makeTrianglesLinesBufferData()
{
	int size = 0;
	m_trianglesLinesVerticesCount = 0;

	for (auto& poly : m_polygons)
	{
		m_trianglesLinesVerticesCount += poly->trianglesLines.size() * 2;
		size += poly->trianglesLines.size() * 2 * 6;
	}

	float* vertices = new float[size];
	int i = 0;

	auto vertexBufferData = [&](QVector3D* v) {
		vertices[i++] = v->x();
		vertices[i++] = v->y();
		vertices[i++] = v->z();
		vertices[i++] = m_uniformColor.x();
		vertices[i++] = m_uniformColor.y();
		vertices[i++] = m_uniformColor.z();
	};

	for (auto& poly : m_polygons)
	{
		for (auto& [v0, v1] : poly->trianglesLines)
		{
			vertexBufferData(v0);
			vertexBufferData(v1);
		}
	}

	m_trianglesLinesVbo.allocate(vertices, size * sizeof(float));
	m_trianglesLinesRenderable->setVerticesCount(m_trianglesLinesVerticesCount);
	delete[] vertices;
}

void Brush::render3D(QOpenGLContext* context, QMatrix4x4& proj, const QVector3D& scaleVec, Camera& camera)
{
	if (m_beingClipped)
	{
		if (m_clippedBrush)
			m_clippedBrush->render3D(context, proj, scaleVec, camera);
		if (m_remainingBrush)
			m_remainingBrush->render3D(context, proj, scaleVec, camera);
		return;
	}

	auto global = GlobalData::getInstance();
	auto trianglesVao = GlobalData::getRenderableVAO(*context, *m_trianglesRenderable);
	auto linesVao = GlobalData::getRenderableVAO(*context, *m_linesRenderable);
	auto trianglesLinesVao = GlobalData::getRenderableVAO(*context, *m_trianglesLinesRenderable);
	float ambient = 0.4f;
	float diffuse = 0.7f;
	float specular = 1.0f;

	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(scaleVec);
	model.translate(m_origin);

	auto setUniforms = [&]()
	{
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
		GLCall(m_program3D->setUniformValue("u_Selected", m_selected || m_beingCut));
		GLCall(m_program3D->setUniformValue("u_SelectionColor", m_selectionColor));
		GLCall(m_program3D->setUniformValue("u_UniformColor", m_uniformColor));
	};

	if (!m_isUsingColor && !global->m_isDrawingLines && !global->m_isWireframeMode)
	{
		for (auto& renderCall : m_renderCalls)
		{
			useContext(context);
			setUniforms();

			for (int i = 0; i < renderCall.textureMap.size(); i += 2)
			{
				std::string idx1 = std::to_string(i);
				std::string idx2 = std::to_string(i + 1);
				std::string textureCount = std::to_string(i / 2);
				GLuint textureLoc = m_program3D->uniformLocation(("u_Textures[" + textureCount + "]").c_str());

				GLCall(m_program3D->setUniformValue(("u_TextureMap[" + idx1 + "]").c_str(), renderCall.textureMap[i]));
				GLCall(m_program3D->setUniformValue(("u_TextureMap[" + idx2 + "]").c_str(), renderCall.textureMap[i + 1]));

				if (renderCall.textureMap[i] == -1)
					break;

				GLCall($->glActiveTexture(GL_TEXTURE0 + renderCall.textureMap[i + 1]));
				GLCall($->glBindTexture(GL_TEXTURE_2D, renderCall.textureMap[i]));
				GLCall(m_program3D->setUniformValue(textureLoc, renderCall.textureMap[i + 1]));
			}

			GLCall(trianglesVao->bind());
			GLCall(m_program3D->setUniformValue("u_IsUsingColor", false));
			GLCall($->glDrawArrays(GL_TRIANGLES, renderCall.begin, renderCall.verticesCount));
		}
		
		return;
	}

	useContext(context);
	setUniforms();

	if (global->m_isDrawingLines)
	{
		GLCall(linesVao->bind());
		GLCall(m_program3D->setUniformValue("u_IsUsingColor", true));
		GLCall($->glDrawArrays(GL_LINES, 0, m_linesVerticesCount));
	}
	else
	{
		if (global->m_isWireframeMode)
		{
			GLCall(trianglesLinesVao->bind());
			GLCall(m_program3D->setUniformValue("u_IsUsingColor", true));
			GLCall($->glDrawArrays(GL_LINES, 0, m_trianglesLinesVerticesCount));
		}
		else
		{
			GLCall(trianglesVao->bind());
			GLCall($->glDrawArrays(GL_TRIANGLES, 0, m_trianglesVerticesCount));
		}
	}
}

void Brush::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	if (m_beingClipped)
	{
		if (m_clippedBrush)
			m_clippedBrush->render2D(context, proj, zoomVec, camera, axis, factor);
		if (m_remainingBrush)
			m_remainingBrush->render2D(context, proj, zoomVec, camera, axis, factor);
		return;
	}

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
	GLCall(m_program2D->setUniformValue("u_BeingCut", m_beingCut || m_isInClippingMode));

	GLCall(vao->bind());
	GLCall($->glDrawArrays(GL_LINES, 0, m_linesVerticesCount));

	if (m_selected && !m_isInClippingMode)
	{
		auto* bbox = &m_boundingBox;
		BrushRenderable* renderable;
		QVector3D scale;
		float* vertices;

		auto calcBboxOrigin = [&]() {
			QList<QVector3D> vertices;
			vertices.push_back({ bbox->startX, bbox->startY, bbox->endZ });
			vertices.push_back({ bbox->endX, bbox->startY, bbox->endZ });
			vertices.push_back({ bbox->endX, bbox->startY, bbox->startZ });
			vertices.push_back({ bbox->startX, bbox->startY, bbox->startZ });
			vertices.push_back({ bbox->startX, bbox->endY, bbox->endZ });
			vertices.push_back({ bbox->endX, bbox->endY, bbox->endZ });
			vertices.push_back({ bbox->endX, bbox->endY, bbox->startZ });
			vertices.push_back({ bbox->startX, bbox->endY, bbox->startZ });

			QVector3D total(0.0f, 0.0f, 0.0f);

			for (auto& v : vertices)
				total += v;

			return total / vertices.size();
		};

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
		model.scale(scale * zoomVec);
		model.translate(calcBboxOrigin() * QVector3D(1 / scale.x(), 1 / scale.y(), 1 / scale.z()));

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

void Brush::writeSelectionBuffer(QOpenGLContext* context, float renderId, QMatrix4x4& proj, const QVector3D& scaleVec, Camera& camera)
{
	auto vao = GlobalData::getRenderableVAO(*context, *m_trianglesRenderable);
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(scaleVec);
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
	std::unordered_map<QVector3D*, QVector3D> newVertices;

	for (auto& v : m_uniqueVertices)
	{
		QMatrix4x4 model;
		model.setToIdentity();
		model.translate(m_origin);
		QVector3D vShifted = model * *v;
		QVector3D vertex = *v;

		switch (axis)
		{
		case Axis::X:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.z() - horStart) / (horEnd - horStart));
				vertex.setZ(v->z() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.y() - verStart) / (verEnd - verStart));
				vertex.setY(v->y() + steps * factor);
			}
			break;
		case Axis::Y:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.x() - horStart) / (horEnd - horStart));
				vertex.setX(v->x() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.z() - verStart) / (verEnd - verStart));
				vertex.setZ(v->z() + steps * factor);
			}
			break;
		case Axis::Z:
			if (isHorizontal)
			{
				float factor = std::abs((vShifted.x() - horStart) / (horEnd - horStart));
				vertex.setX(v->x() + steps * factor);
			}
			else
			{
				float factor = std::abs((vShifted.y() - verStart) / (verEnd - verStart));
				vertex.setY(v->y() + steps * factor);
			}
		}

		total += vertex;
		newVertices[v] = vertex;
	}

	for (auto* polygon : m_polygons)
	{
		QMatrix4x4 model = get2DTransformMatrix(polygon->norm);

		for (auto* v : polygon->vertices)
		{
			QVector3D newVertex = newVertices[v];
			QVector3D oldPos = model * *v;
			QVector3D newPos = model * newVertex;
			QVector3D shift = newPos - oldPos;

			auto& texCoord = polygon->verticesMap[v];
			texCoord.setX(texCoord.x() + shift.x() / polygon->textureWidth * Constants::TEXTURE_MAGNIFYING_FACTOR);
			texCoord.setY(texCoord.y() + shift.y() / polygon->textureHeight * Constants::TEXTURE_MAGNIFYING_FACTOR);
		}
	}

	QVector3D shift = total / m_uniqueVertices.size();
	m_origin += shift;

	for (auto& [_, v] : newVertices)
	{
		v -= shift;
	}

	for (auto* v : m_uniqueVertices)
	{
		*v = newVertices[v];
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

	recalcParams();
	makeTrianglesBufferData();
	makeLinesBufferData();
	makeTrianglesLinesBufferData();
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
	}

	m_lastPos = pos;
}

void Brush::setupTextures()
{
	auto* global = GlobalData::getInstance();
	int begin = 0;
	int textureCount = 0;
	int verticesCount = 0;
	std::unordered_map<GLuint, GLuint> map;

	m_renderCalls.clear();

	for (auto* polygon : m_polygons)
	{
		if (map.find(polygon->textureId) == map.end())
		{
			map[polygon->textureId] = textureCount++;
		}
		verticesCount += polygon->triangles.size() * 3;

		if (map.size() >= global->m_maxTextureUnits)
		{
			QList<int> textureMap;

			for (auto [textureId, activeTextureId] : map)
			{
				textureMap.append(textureId);
				textureMap.append(activeTextureId);
			}

			m_renderCalls.push_back({ begin, verticesCount, textureMap });
			begin += verticesCount;
			map.clear();
			verticesCount = 0;
			textureCount = 0;
		}
	}

	if (map.size() > 0)
	{
		QList<int> textureMap;
		int i = 0;

		for (auto [textureId, activeTextureId] : map)
		{
			textureMap.append(textureId);
			textureMap.append(activeTextureId);
			i += 2;
		}

		while (i < global->m_maxTextureUnits)
		{
			textureMap.append(-1);
			textureMap.append(-1);
			i += 2;
		}

		m_renderCalls.push_back({ begin, verticesCount, textureMap });
	}
}

void Brush::makePolygonVertices(Types::Polygon* polygon, int& i, float* output)
{
	auto vertexBufferData = [&](QVector3D* v) {
		auto& texCoords = polygon->verticesMap[v];
		QMatrix4x4 model;
		model.setToIdentity();
		model.rotate(polygon->rotationAngle, 0.0f, 0.0f, 1.0f);
		auto rotatedCoords = model * QVector4D(texCoords, 0.0f, 1.0f);
		auto x = rotatedCoords.x();
		auto y = rotatedCoords.y();
		auto scaleX = 1.0f / polygon->scale.x();
		auto scaleY = 1.0f / polygon->scale.y();
		auto shiftX = (polygon->shift.x() / polygon->textureWidth);
		auto shiftY = (polygon->shift.y() / polygon->textureHeight);
		output[i++] = v->x();
		output[i++] = v->y();
		output[i++] = v->z();
		output[i++] = x * scaleX + shiftX * scaleX;
		output[i++] = y * scaleY - shiftY * scaleY;
		output[i++] = m_uniformColor.x();
		output[i++] = m_uniformColor.y();
		output[i++] = m_uniformColor.z();
		output[i++] = polygon->norm.x();
		output[i++] = polygon->norm.y();
		output[i++] = polygon->norm.z();
		output[i++] = polygon->isSelected ? 1.0f : 0.0f;
		output[i++] = static_cast<float>(polygon->isUsingColor);
		output[i++] = static_cast<float>(polygon->textureId);
	};

	for (auto& triangle : polygon->triangles)
	{
		vertexBufferData(triangle.v0);
		vertexBufferData(triangle.v1);
		vertexBufferData(triangle.v2);
	}
}

void Brush::calcTexCoords(Types::Polygon* polygon)
{
	for (auto* v : polygon->vertices)
	{
		QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
		QVector3D position = model * *v;
		float x = (position.x() - polygon->minX) / polygon->textureWidth * Constants::TEXTURE_MAGNIFYING_FACTOR;
		float y = (position.y() - polygon->minY) / polygon->textureHeight * Constants::TEXTURE_MAGNIFYING_FACTOR;
		polygon->verticesMap[v] = QVector2D(x, y);
	}
}

bool Brush::shouldRotate(QVector3D norm)
{
	auto round = [&](float n) {
		return Helpers::round(n, 5);
	};

	return !(round(m_targetAxis.x()) == round(norm.x()) &&
		round(m_targetAxis.y()) == round(norm.y()) &&
		round(m_targetAxis.z()) == round(norm.z())
		) && !(
		round(-m_targetAxis.x()) == round(norm.x()) &&
		round(-m_targetAxis.y()) == round(norm.y()) &&
		round(-m_targetAxis.z()) == round(norm.z())
		);
}

QMatrix4x4 Brush::get2DTransformMatrix(QVector3D norm)
{
	QMatrix4x4 transform;
	QMatrix4x4 transform2;
	QVector3D projectionVec(norm.x(), 0.0f, norm.z());

	transform.setToIdentity();
	transform2.setToIdentity();
	projectionVec.normalize();

	if (Helpers::areEqual(projectionVec, QVector3D(0.0f, 0.0f, 0.0f)))
	{
		projectionVec = m_targetAxis;
	}

	if (!Helpers::areEqual(norm.y(), 0.0f))
	{
		QVector3D rotationAxis = QVector3D::crossProduct(projectionVec, norm);
		float cos = QVector3D::dotProduct(norm, projectionVec) / (norm.length() * projectionVec.length());
		float acos = -qRadiansToDegrees(qAcos(cos));
		transform.rotate(acos, rotationAxis);
	}

	if (shouldRotate(projectionVec))
	{
		QVector3D rotationAxis = QVector3D::crossProduct(projectionVec, m_targetAxis);
		float cos = QVector3D::dotProduct(m_targetAxis, projectionVec) / (m_targetAxis.length() * projectionVec.length());
		float acos = -qRadiansToDegrees(qAcos(cos));
		transform2.rotate(acos, -rotationAxis);
	}

	return transform2 * transform;
}

void Brush::recalcParams()
{
	for (auto* polygon : m_polygons)
	{
		auto& triangle = polygon->triangles[0];
		auto v0 = *triangle.v0;
		auto v1 = *triangle.v1;
		auto v2 = *triangle.v2;
		QVector3D norm = QVector3D::crossProduct(v0 - v1, v0 - v2);

		norm.normalize();
		polygon->norm = norm;
		polygon->a = norm.x();
		polygon->b = norm.y();
		polygon->c = norm.z();
		polygon->d = -QVector3D::dotProduct(norm, v0);
	}
}

void Brush::selectPolygon(Types::Polygon* polygon)
{
	polygon->isSelected = true;
	sendPolygonDataToGPU(polygon);
}

void Brush::unselectPolygon(Types::Polygon* polygon)
{
	polygon->isSelected = false;
	sendPolygonDataToGPU(polygon);
}

void Brush::sendPolygonDataToGPU(Types::Polygon* polygon)
{
	int size = polygon->triangles.size() * 3 * 14;
	float* data = new float[size];
	int i = 0;
	makePolygonVertices(polygon, i, data);
	m_trianglesVbo.subdata(polygon->begin, size * sizeof(float), data);
	delete[] data;
}

void Brush::updatePolygonTexture(Types::Polygon* polygon, Texture& texture)
{
	polygon->textureId = texture.id;
	polygon->textureWidth = texture.width;
	polygon->textureHeight = texture.height;

	QMatrix4x4 model = get2DTransformMatrix(polygon->norm);
	polygon->minX = std::numeric_limits<float>::max();
	polygon->minY = std::numeric_limits<float>::max();

	for (auto v : polygon->vertices)
	{
		QVector3D pos = model * *v;

		if (pos.x() < polygon->minX)
			polygon->minX = pos.x();
		if (pos.y() < polygon->minY)
			polygon->minY = pos.y();
	}

	calcTexCoords(polygon);
	sendPolygonDataToGPU(polygon);
	setupTextures();
}
