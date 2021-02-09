#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "GL.h"

class Brush : OpenGLFunctions
{
public:
	Brush();
	~Brush();

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram* m_program;
	
	inline int verticesCount() { return m_verticesCount; }

private:
	float* m_vertices;
	int m_verticesCount = 0;
};
