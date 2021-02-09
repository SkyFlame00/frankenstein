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

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(m_vertices, 9 * sizeof(float));
	$->glEnableVertexAttribArray(0);
	$->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	m_vbo.release();
	m_vao.release();

	/* Shader */
	m_program = ResourceManager::getProgram("test", "test");
}

Brush::~Brush()
{

}
