#include "Brush.h"

#include "ResourceManager.h"

Brush::Brush()
{
	m_vertices = new float[] {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	m_verticesCount = 3;

	GLCall(m_vao.create());
	GLCall(m_vao.bind());
	GLCall(m_vbo.create());
	GLCall(m_vbo.bind());
	GLCall(m_vbo.allocate(m_vertices, 9 * sizeof(float)));
	GLCall($->glEnableVertexAttribArray(0));
	GLCall($->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
	GLCall(m_vbo.release());
	GLCall(m_vao.release());

	/* Shader */
	m_program = ResourceManager::getProgram("test", "test");
}

Brush::~Brush()
{

}
