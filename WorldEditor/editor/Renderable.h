#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "GL.h"

class Renderable : public OpenGLFunctions
{
public:
	virtual inline int verticesCount() = 0;

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram* m_program;
};
