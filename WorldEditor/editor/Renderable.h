#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include "GL.h"

class Renderable : public OpenGLFunctions
{
public:
	Renderable() : m_modelMatrix(), m_scaleVec(1.0f, 1.0f, 1.0f), m_origin(0.0f, 0.0f, 0.0f) {}

	virtual int verticesCount() = 0;
	bool inline shouldDraw() { return m_shouldDraw; }
	GLenum inline getDrawMode() { return m_drawMode; }
	bool inline shouldScale() { return m_shouldScale; }

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram* m_program;
	QMatrix4x4 m_modelMatrix;
	QVector3D m_scaleVec;
	QVector3D m_origin;

protected:
	GLenum m_drawMode = GL_LINES;
	bool m_shouldDraw = true;
	bool m_shouldScale = false;
};
