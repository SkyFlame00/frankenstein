#include "GLWidget2D.h"

#include <QList>
#include <QPoint>
#include "../editor/Brush.h"
#include "../editor/Renderable.h"

GLWidget2D::GLWidget2D(Camera* camera, Renderer2D* renderer, Scene* scene, QWidget* parent)
	: QOpenGLWidget(parent), m_camera(camera), m_renderer(renderer), m_scene(scene)
{
}

GLWidget2D::~GLWidget2D()
{
}

void GLWidget2D::initializeGL()
{
	m_axis = Axis::Z;
	m_grid = new Grid2D(m_axis);
	m_renderer->m_axis = m_axis;
	m_renderer->setup(-1.0f, m_grid->HALF_CUBE * 2);
}

void GLWidget2D::paintGL()
{
	processInputData();
	m_grid->updateView(m_camera->getPosition(), m_frustrumWidth, m_frustrumHeight);

	QList<Brush*> objects;
	QList<Renderable*> guiObjects;

	if (m_grid->ShouldDraw())
	{
		guiObjects.push_back(m_grid);
	}

	m_renderer->render(objects, guiObjects);

	clearInputData();
	update();
}

void GLWidget2D::resizeGL(int width, int height)
{
	m_frustrumWidth = width / FRUSTRUM_SIZE_FACTOR;
	m_frustrumHeight = height / FRUSTRUM_SIZE_FACTOR;
	m_renderer->setFrustrum(m_frustrumWidth, m_frustrumHeight);
}

void GLWidget2D::processInputData()
{
	float prevFactor = SCENE_ZOOM_FACTORS.find(m_zoom)->second;

	if (m_inputData.mouseScroll == MouseScroll::UP)
	{
		zoomIn();
		m_grid->m_zoom = m_zoom;
		m_renderer->setZoom(m_zoom);
	}
	else if (m_inputData.mouseScroll == MouseScroll::DOWN)
	{
		zoomOut();
		m_grid->m_zoom = m_zoom;
		m_renderer->setZoom(m_zoom);
	}

	float newFactor = SCENE_ZOOM_FACTORS.find(m_zoom)->second;
	float factor = newFactor / prevFactor;
	m_camera->setPosition(m_camera->getPosition() * factor);

	bool isWidgetActive = m_inputData.isMouseOver;
	float velocity = 0.8f;
	float horShift = velocity;
	float verShift = velocity;

	if (m_inputData.keyW == ButtonState::PRESSED && isWidgetActive)
	{
		m_camera->moveRelativelyToAxis(m_axis, 0.0f, verShift);
	}
	if (m_inputData.keyA == ButtonState::PRESSED && isWidgetActive)
	{
		m_camera->moveRelativelyToAxis(m_axis, -horShift, 0.0f);
	}
	if (m_inputData.keyS == ButtonState::PRESSED && isWidgetActive)
	{
		m_camera->moveRelativelyToAxis(m_axis, 0.0f, -verShift);
	}
	if (m_inputData.keyD == ButtonState::PRESSED && isWidgetActive)
	{
		m_camera->moveRelativelyToAxis(m_axis, horShift, 0.0f);
	}

	m_camera->updateCameraVectors();
}

void GLWidget2D::clearInputData()
{
	m_inputData.mouseScroll = MouseScroll::NO_SCROLL;
}

void GLWidget2D::wheelEvent(QWheelEvent* event)
{
	QPoint point = event->angleDelta();

	if (point.y() > 0)
		m_inputData.mouseScroll = MouseScroll::UP;
	else if (point.y() < 0)
		m_inputData.mouseScroll = MouseScroll::DOWN;
}

void GLWidget2D::enterEvent(QEvent* event)
{
	m_inputData.isMouseOver = true;
	QOpenGLWidget::enterEvent(event);
}

void GLWidget2D::leaveEvent(QEvent* event)
{
	m_inputData.isMouseOver = false;
	QOpenGLWidget::leaveEvent(event);
}

void GLWidget2D::zoomIn()
{
	switch (m_zoom)
	{
	case SceneZoom::X001:
		m_zoom = SceneZoom::X005;
		break;
	case SceneZoom::X005:
		m_zoom = SceneZoom::X01;
		break;
	case SceneZoom::X01:
		m_zoom = SceneZoom::X02;
		break;
	case SceneZoom::X02:
		m_zoom = SceneZoom::X04;
		break;
	case SceneZoom::X04:
		m_zoom = SceneZoom::X06;
		break;
	case SceneZoom::X06:
		m_zoom = SceneZoom::X08;
		break;
	case SceneZoom::X08:
		m_zoom = SceneZoom::X1;
		break;
	case SceneZoom::X1:
		m_zoom = SceneZoom::X2;
		break;
	case SceneZoom::X2:
		m_zoom = SceneZoom::X4;
		break;
	case SceneZoom::X4:
		m_zoom = SceneZoom::X8;
		break;
	case SceneZoom::X8:
		/* do nothing*/
		break;
	}
}

void GLWidget2D::zoomOut()
{
	switch (m_zoom)
	{
	case SceneZoom::X001:
		/* do nothing*/
		break;
	case SceneZoom::X005:
		m_zoom = SceneZoom::X001;
		break;
	case SceneZoom::X01:
		m_zoom = SceneZoom::X005;
		break;
	case SceneZoom::X02:
		m_zoom = SceneZoom::X01;
		break;
	case SceneZoom::X04:
		m_zoom = SceneZoom::X02;
		break;
	case SceneZoom::X06:
		m_zoom = SceneZoom::X04;
		break;
	case SceneZoom::X08:
		m_zoom = SceneZoom::X06;
		break;
	case SceneZoom::X1:
		m_zoom = SceneZoom::X08;
		break;
	case SceneZoom::X2:
		m_zoom = SceneZoom::X1;
		break;
	case SceneZoom::X4:
		m_zoom = SceneZoom::X2;
		break;
	case SceneZoom::X8:
		m_zoom = SceneZoom::X4;
		break;
	}
}
