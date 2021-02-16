#include "Grid2D.h"
#include "ResourceManager.h"
#include "../common/helpers.h"

Grid2D::Grid2D(Axis axis, SceneZoom zoom)
	: m_axis(axis), m_zoom(zoom)
{
	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();

	$->glEnableVertexAttribArray(0);
	$->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	m_vbo.release();
	m_vao.release();

	m_program = ResourceManager::getProgram("test", "test");
	m_program->bind();
}

Grid2D::~Grid2D()
{
	delete m_program;
}

void Grid2D::increaseScale()
{
	switch (m_step)
	{
	case UnitSize::X1:
		m_step = UnitSize::X2;
		break;
	case UnitSize::X2:
		m_step = UnitSize::X4;
		break;
	case UnitSize::X4:
		m_step = UnitSize::X8;
		break;
	case UnitSize::X8:
		m_step = UnitSize::X16;
		break;
	case UnitSize::X16:
		m_step = UnitSize::X32;
		break;
	case UnitSize::X32:
		/* Do nothing */
		break;
	}
}

void Grid2D::decreaseScale()
{
	switch (m_step)
	{
	case UnitSize::X1:
		/* Do nothing */
		break;
	case UnitSize::X2:
		m_step = UnitSize::X1;
		break;
	case UnitSize::X4:
		m_step = UnitSize::X2;
		break;
	case UnitSize::X8:
		m_step = UnitSize::X4;
		break;
	case UnitSize::X16:
		m_step = UnitSize::X8;
		break;
	case UnitSize::X32:
		m_step = UnitSize::X16;
		break;
	}
}

void Grid2D::updateView(QVector3D position, float width, float height)
{
	float c1, c2;

	switch (m_axis)
	{
	case Axis::X:
		c1 = position.z();
		c2 = position.y();
		break;
	case Axis::Y:
		c1 = position.x();
		c2 = position.z();
		break;
	case Axis::Z:
		c1 = position.x();
		c2 = position.y();
		break;
	}

	float factor = SCENE_ZOOM_FACTORS.find(m_zoom)->second;
	float halfWidth = width /  2;
	float halfHeight = height /  2;
	float frustrumLeft = c1 - halfWidth;
	float frustrumRight = c1 + halfWidth;
	float frustrumTop = c2 + halfHeight;
	float frustrumBottom = c2 - halfHeight;
	float gridLeft = m_gridLeft * factor;
	float gridRight = m_gridRight * factor;
	float gridTop = m_gridTop * factor;
	float gridBottom = m_gridBottom * factor;
	QList<QVector2D> verticalLines;
	QList<QVector2D> horizontalLines;
	float hor;
	float horEnd;
	float ver;
	float verEnd;
	bool nothing = false;
	float step = static_cast<float>(SIZES.find(m_step)->second.find(m_zoom)->second) * factor;

	if (gridLeft <= frustrumLeft && frustrumRight <= gridRight)
	{
		int remainder = frustrumLeft / step;
		hor = step * remainder - step;
		horEnd = frustrumRight;
	}
	else if (frustrumLeft < gridLeft && gridLeft <= frustrumRight && frustrumRight <= gridRight)
	{
		hor = gridLeft;
		horEnd = frustrumRight;
	}
	else if (gridLeft <= frustrumLeft && frustrumLeft <= gridRight && gridRight < frustrumRight)
	{
		int remainder = frustrumLeft / step;
		hor = step * remainder - step;
		horEnd = gridRight;
	}
	else if (frustrumRight < gridLeft)
	{
		nothing = true;
	}
	else if (gridRight < frustrumLeft)
	{
		nothing = true;
	}
	else if (frustrumLeft < gridLeft && gridRight < frustrumRight)
	{
		hor = gridLeft;
		horEnd = gridRight;
	}

	if (gridBottom <= frustrumBottom && frustrumTop <= gridTop)
	{
		int remainder = frustrumTop / step;
		ver = remainder * step + step;
		verEnd = frustrumBottom;
	}
	else if (gridBottom <= frustrumBottom && gridTop < frustrumTop)
	{
		ver = gridTop;
		verEnd = frustrumBottom;
	}
	else if (frustrumBottom < gridBottom && gridBottom <= frustrumTop && frustrumTop <= gridTop)
	{
		int remainder = frustrumTop / step;
		ver = remainder * step + step;
		verEnd = gridBottom;
	}
	else if (gridTop < frustrumBottom)
	{
		nothing = true;
	}
	else if (frustrumTop < gridBottom)
	{
		nothing = true;
	}
	else if (frustrumBottom < gridBottom && gridTop < frustrumTop)
	{
		ver = gridTop;
		verEnd = gridBottom;
	}

	if (nothing)
	{
		m_shouldDraw = false;
		return;
	}

	for (float horPos = hor; Helpers::trunc(horPos) <= Helpers::trunc(horEnd, 3); horPos += step)
	{
		verticalLines.push_back({ horPos, ver });
		verticalLines.push_back({ horPos, verEnd });
	}

	for (float verPos = ver; Helpers::trunc(verPos) >= Helpers::trunc(verEnd, 3); verPos -= step)
	{
		horizontalLines.push_back({ hor, verPos });
		horizontalLines.push_back({ horEnd, verPos });
	}

	m_verticesCount = (verticalLines.size() + horizontalLines.size()) * 2;
	int size = m_verticesCount * 3;
	float* vertices = new float[size];
	int n = 0;

	for (auto& horLine : horizontalLines)
	{
		addVertex(vertices, n, horLine[0], horLine[1]);
		n += 3;
	}

	for (auto& verLine : verticalLines)
	{
		addVertex(vertices, n, verLine[0], verLine[1]);
		n += 3;
	}

	m_vbo.bind();
	m_vbo.allocate(vertices, size * sizeof(float));
	delete[] vertices;
	m_shouldDraw = true;
}

void Grid2D::addVertex(float* vertices, int i, float horPos, float verPos)
{
	switch (m_axis)
	{
	case Axis::X:
		vertices[i] = -HALF_LENGTH;
		vertices[i + 1] = verPos;
		vertices[i + 2] = horPos;
		break;
	case Axis::Y:
		vertices[i] = horPos;
		vertices[i + 1] = -HALF_LENGTH;
		vertices[i + 2] = verPos;
		break;
	case Axis::Z:
		vertices[i] = horPos;
		vertices[i + 1] = verPos;
		vertices[i + 2] = -HALF_LENGTH;
		break;
	}
}
