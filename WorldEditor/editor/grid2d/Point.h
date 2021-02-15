#pragma once

#include "../Renderable.h"

class Point : public Renderable
{
public:
	Point(float x, float y, float z);

	inline int verticesCount() { return m_verticesCount; };

private:
	int m_verticesCount = 0;
};
