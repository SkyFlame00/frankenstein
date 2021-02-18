#pragma once

#include <QMatrix4x4>
#include <QList>
#include <QVector3D>
#include "Grid2D.h"
#include "GL.h"
#include "Camera.h"
#include "Brush.h"
#include "Renderable.h"
#include "../common/types.h"

class Renderer2D : OpenGLFunctions
{
public:
	Renderer2D(Camera* camera, float width, float height);
	~Renderer2D();

	void setup(float nearPlane, float farPlane);
	void render(QOpenGLContext* context, Grid2D& grid2D, QList<Brush*>& objects, QList<Renderable*>& guiObjects, float zoomFactor);
	void setFrustrum(float width, float height);
	void setZoom(SceneZoom zoom);

	Axis m_axis;
	float m_width = 0;
	float m_height = 0;

private:
	Grid2D* m_grid;
	Camera* m_camera;
	QMatrix4x4 m_projMatrix;
	float m_near = 0.0f;
	float m_far = 0.0f;
	SceneZoom m_zoom = SceneZoom::X1;
	QVector3D m_zoomVec;
};
