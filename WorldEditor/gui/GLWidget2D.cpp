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
	m_grid = new Grid2D(Axis::Z);
	
	m_renderer->setup(-1.0f, m_grid->HALF_CUBE * 2);
	m_renderer->m_axis = Axis::Z;
}

void GLWidget2D::paintGL()
{
	processInputData();
	m_grid->updateView(m_camera->getPosition(), m_frustrumWidth, m_frustrumHeight);

	QList<Brush*> objects;
	QList<Renderable*> guiObjects;
	//QList<Renderable*> guiObjects;

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
	if (m_inputData.mouseScroll == MouseScroll::UP)
		m_grid->zoomIn();
	else if (m_inputData.mouseScroll == MouseScroll::DOWN)
		m_grid->zoomOut();

	bool isWidgetActive = m_inputData.isMouseOver;

	if (m_inputData.mouseScroll == MouseScroll::UP)
		m_grid->zoomIn();
	else if (m_inputData.mouseScroll == MouseScroll::DOWN)
		m_grid->zoomOut();

	float w = m_renderer->m_width;
	float h = m_renderer->m_height;
	float c = 5.0f;
	float horShift = 1.0f;
	float verShift = 1.0f;

	if (m_inputData.keyW == ButtonState::PRESSED && isWidgetActive)
	{
		//m_renderer->setFrustrum(w, h + c);
		m_camera->moveRelativeToAxis(m_axis, 0.0f, verShift);
	}
	if (m_inputData.keyA == ButtonState::PRESSED && isWidgetActive)
	{
		//m_renderer->setFrustrum(w - c, h);
		m_camera->moveRelativeToAxis(m_axis, -horShift, 0.0f);
	}
	if (m_inputData.keyS == ButtonState::PRESSED && isWidgetActive)
	{
		//m_renderer->setFrustrum(w, h - c);
		m_camera->moveRelativeToAxis(m_axis, 0.0f, -verShift);
	}
	if (m_inputData.keyD == ButtonState::PRESSED && isWidgetActive)
	{
		//m_renderer->setFrustrum(w + c, h);
		m_camera->moveRelativeToAxis(m_axis, horShift, 0.0f);
	}
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
