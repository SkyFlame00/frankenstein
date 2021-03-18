#pragma once

#include "GL.h"
#include <QList>

class VertexBufferObject : public OpenGLFunctions
{
public:
	struct VertexBufferElement
	{
		GLuint type;
		GLuint count;
		const void* pointer;
	};

	VertexBufferObject();
	~VertexBufferObject();

	void allocate(void* data, GLuint size);
	void apply();
	inline int componentsCount() { return m_componentsCount; }
	void subdata(GLuint begin, GLuint size, void* data);

	template<typename T>
	void addAttribute(GLuint count) { static_assert(false); }

	template<>
	void addAttribute<float>(GLuint count)
	{
		m_elements.push_back({ GL_FLOAT, count, (const void*)m_stride });
		m_stride += count * sizeof(float);
		m_componentsCount += count;
	}

private:
	GLuint m_vbo;
	GLenum m_target;
	GLenum m_usage;
	GLuint m_stride;
	QList<VertexBufferElement> m_elements;
	int m_componentsCount = 0;
};
