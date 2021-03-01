#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include "GL.h"
#include "Camera.h"
#include <QOpenGLContext>
#include "VertexBufferObject.h"

class Renderable : public OpenGLFunctions
{
public:
	Renderable();
	~Renderable();

	virtual int verticesCount() = 0;
	bool inline shouldDraw() { return m_shouldDraw; }
	GLenum inline getDrawMode() { return m_drawMode; }
	bool inline shouldScale() { return m_shouldScale; }
	virtual void render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor);
	virtual void render3D(QOpenGLContext* context, QMatrix4x4& proj, Camera& camera);
	virtual void createVAO(VertexBufferObject& vbo);
	virtual void bindVAO(QOpenGLContext* context);
	virtual inline float getRenderId() { return m_renderId; }
	virtual inline void changeRenderId(float renderId) { m_renderId = renderId; }

	QOpenGLShaderProgram* m_program;
	QMatrix4x4 m_modelMatrix;
	QVector3D m_scaleVec;
	QVector3D m_origin;

protected:
	GLenum m_drawMode = GL_LINES;
	bool m_shouldDraw = true;
	bool m_shouldScale = false;
	float m_renderId; /* It's changed once per each render */
};
