#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
	: m_stride(0), m_target(GL_ARRAY_BUFFER), m_usage(GL_STATIC_DRAW)
{
	$->glGenBuffers(1, &m_vbo);
}

void VertexBufferObject::allocate(void* data, GLuint size)
{
	$->glBindBuffer(m_target, m_vbo);
	$->glBufferData(m_target, size, data, m_usage);
}

void VertexBufferObject::apply()
{
	$->glBindBuffer(m_target, m_vbo);

	int i = 0;
	for (auto& elem : m_elements)
	{
		$->glEnableVertexAttribArray(i);
		$->glVertexAttribPointer(i, elem.count, elem.type, GL_FALSE, m_stride, elem.pointer);
		i++;
	}
}
