#include "GLWidget2D.h"

#include <QList>
#include <QPoint>
#include <math.h>
#include "../editor/Brush.h"
#include "../editor/Renderable.h"
#include "../editor/grid2d/Point.h"

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
	m_grid = new Grid2D(m_axis, m_zoom);
	m_guiObjects.push_back(m_grid);
	m_renderer->m_axis = m_axis;
	m_renderer->setup(-1.0f, m_grid->HALF_CUBE * 2);
}

void GLWidget2D::paintGL()
{
	processInputData();
	m_grid->updateView(m_camera->getPosition(), m_frustrumWidth, m_frustrumHeight);

	QList<Brush*> objects;

	m_renderer->render(objects, m_guiObjects);

	clearInputData();
	update();
}

void GLWidget2D::resizeGL(int width, int height)
{
	m_frustrumWidth = width;
	m_frustrumHeight = height;
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
	float velocity = 10.0f;
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
	if (m_inputData.keyOpenBracket == ButtonState::PRESSED && isWidgetActive)
	{
		m_grid->increaseScale();
	}
	if (m_inputData.keyCloseBracket == ButtonState::PRESSED && isWidgetActive)
	{
		m_grid->decreaseScale();
	}
	if (m_inputData.leftMouseDown == ButtonDownState::ACTIVE)
	{
		qInfo() << "leftMouseDown: x = " << m_inputData.mouseX << ", " << m_inputData.mouseY;
		placePoint(m_inputData.mouseX, m_inputData.mouseY);
		m_inputData.leftMouseDown = ButtonDownState::PROCESSED;
	}

	m_camera->updateCameraVectors();
}

void GLWidget2D::clearInputData()
{
	m_inputData.mouseScroll = MouseScroll::NO_SCROLL;
	m_inputData.keyOpenBracket = ButtonState::NOT_ACTIVE;
	m_inputData.keyCloseBracket = ButtonState::NOT_ACTIVE;
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

void GLWidget2D::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_inputData.leftMouse = ButtonState::PRESSED;

		if (m_inputData.leftMouseDown == ButtonDownState::READY)
		{
			m_inputData.leftMouseDown = ButtonDownState::ACTIVE;
		}
	}

	QOpenGLWidget::mousePressEvent(event);
}

void GLWidget2D::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_inputData.leftMouse = ButtonState::RELEASED;
		m_inputData.leftMouseDown = ButtonDownState::READY;
	}

	QOpenGLWidget::mouseReleaseEvent(event);
}

void GLWidget2D::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	m_inputData.mouseX = pos.x();
	m_inputData.mouseY = pos.y();

	QOpenGLWidget::mouseMoveEvent(event);
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
		m_zoom = SceneZoom::X16;
		break;
	case SceneZoom::X16:
		m_zoom = SceneZoom::X32;
		break;
	case SceneZoom::X32:
		m_zoom = SceneZoom::X64;
		break;
	case SceneZoom::X64:
		m_zoom = SceneZoom::X128;
		break;
	case SceneZoom::X128:
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
	case SceneZoom::X16:
		m_zoom = SceneZoom::X8;
		break;
	case SceneZoom::X32:
		m_zoom = SceneZoom::X16;
		break;
	case SceneZoom::X64:
		m_zoom = SceneZoom::X32;
		break;
	case SceneZoom::X128:
		m_zoom = SceneZoom::X64;
		break;
	}
}

void GLWidget2D::placePoint(int screenX, int screenY)
{
	float camX, camY;
	QVector3D camPos = m_camera->getPosition();

	switch (m_axis)
	{
	case Axis::X:
		camX = camPos.z();
		camY = camPos.y();
		break;
	case Axis::Y:
		camX = camPos.x();
		camY = camPos.z();
		break;
	case Axis::Z:
		camX = camPos.x();
		camY = camPos.y();
	}

	float currentfactor = SCENE_ZOOM_FACTORS.find(m_zoom)->second;
	float referenceFactor = SCENE_ZOOM_FACTORS.find(REFERENCE_ZOOM)->second;
	float factor = referenceFactor / currentfactor;
	float x0 = m_frustrumWidth / 2.0f;
	float y0 = m_frustrumHeight / 2.0f;
	float shiftX = (screenX - x0) * factor;
	float shiftY = (y0 - screenY) * factor;
	float pointX = camX * factor + shiftX;
	float pointY = camY * factor + shiftY;

	float nearestCellX, nearestCellY;
	float step = static_cast<float>(m_grid->getStep());

	if (pointX < 0.0f)
	{
		int times = pointX / step;
		float right = step * times;
		float left = right - step;
		if (std::abs(pointX - left) < std::abs(pointX - right))
		{
			nearestCellX = left;
		}
		else
		{
			nearestCellX = right;
		}
	}
	else
	{
		int times = pointX / step;
		float left = step * times;
		float right = left + step;
		if (std::abs(pointX - left) < std::abs(pointX - right))
		{
			nearestCellX = left;
		}
		else
		{
			nearestCellX = right;
		}
	}

	if (pointY < 0.0f)
	{
		int times = pointY / step;
		float top = step * times;
		float bottom = top - step;
		if (std::abs(pointY - top) < std::abs(pointY - bottom))
		{
			nearestCellY = top;
		}
		else
		{
			nearestCellY = bottom;
		}
	}
	else
	{
		int times = pointY / step;
		float bottom = step * times;
		float top = bottom + step;
		if (std::abs(pointY - top) < std::abs(pointY - bottom))
		{
			nearestCellY = top;
		}
		else
		{
			nearestCellY = bottom;
		}
	}

	Point* point;

	switch (m_axis)
	{
	case Axis::X:
		point = new Point(0.0f, nearestCellY, nearestCellX);
		break;
	case Axis::Y:
		point = new Point(nearestCellX, 0.0f, nearestCellY);
		break;
	case Axis::Z:
		point = new Point(nearestCellX, nearestCellY, 0.0f);
	}

	m_guiObjects.push_back(point);
}
