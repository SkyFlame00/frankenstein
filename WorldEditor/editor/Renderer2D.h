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

	void setup(float _near, float _far);
	void render(QList<Brush*>& objects, QList<Renderable*>& guiObjects);

	void zoomIn();
	void zoomOut();
	void setFrustrum(float width, float height);

	Axis m_axis;
	float m_near = 0;
	float m_far = 0;

	float m_width = 0;
	float m_height = 0;

private:
	
	
	Grid2D* m_grid;
	Camera* m_camera;
	QMatrix4x4 m_projMatrix;

	QVector3D m_zoom;
	
};
