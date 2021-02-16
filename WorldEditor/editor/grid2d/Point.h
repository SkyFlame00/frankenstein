#pragma once

#include <QVector3D>
#include "../Renderable.h"

class Point : public Renderable
{
public:
	Point(float x, float y, float z);
	Point(QVector3D position);

	inline int verticesCount() { return m_verticesCount; };

private:
	int m_verticesCount = 0;
};
