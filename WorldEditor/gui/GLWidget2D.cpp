#include "GLWidget2D.h"

#include <QList>
#include <QPoint>
#include <math.h>
#include "../editor/Brush.h"
#include "../editor/Renderable.h"
#include "../editor/grid2d/Point.h"
#include "../common/helpers.h"
#include "../common/GlobalData.h"
#include "../editor/ConstructionBlock.h"

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
	m_renderer->m_axis = m_axis;
	m_renderer->setup(-1.0f, m_grid->HALF_LENGTH * 2);
	m_renderer->setZoom(m_zoom);
}

void GLWidget2D::paintGL()
{
	processInputData();
	m_grid->updateView(m_camera->getPosition(), m_frustrumWidth, m_frustrumHeight);

	QList<Brush*> objects;

	m_renderer->render(*m_grid, objects, m_scene->m_gui2DObjects, getZoomFactor());

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
	
	if (GlobalData::getInstance()->m_editorMode == EdtitorMode::BLOCK_MODE)
	{
		processToolMode();
	}

	//if (m_inputData.leftMouseDown == ButtonDownState::ACTIVE)
	//{
	//	//placePoint(m_inputData.mouseX, m_inputData.mouseY);
	//	m_inputData.leftMouseDown = ButtonDownState::PROCESSED;
	//}
	
	m_camera->updateCameraVectors();
}

void GLWidget2D::processToolMode()
{
	auto globalData = GlobalData::getInstance();
	auto blockToolData = &globalData->m_blockToolData;

	if (blockToolData->state == BlockToolState::CREATING)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			float nearestX, nearestY;
			getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);

			QVector3D pointPos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY);

			m_dragData.startPoint = new Point(1.0f, pointPos);
			m_scene->m_gui2DObjects.push_back(m_dragData.startPoint);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float nearestX, nearestY;
			getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);
			QVector3D endPointPos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY);
			QVector3D startPointPos = m_dragData.startPoint->m_origin;

			if (
				Helpers::trunc(endPointPos.x()) != Helpers::trunc(startPointPos.x()) ||
				Helpers::trunc(endPointPos.y()) != Helpers::trunc(startPointPos.y()) ||
				Helpers::trunc(endPointPos.z()) != Helpers::trunc(startPointPos.z()))
			{
				m_dragData.endPoint = new Point(1.0f, endPointPos);
				m_scene->m_gui2DObjects.removeOne(m_dragData.startPoint);

				if (blockToolData->blockInstance)
				{
					m_scene->m_gui2DObjects.removeOne(blockToolData->blockInstance);
					delete blockToolData->blockInstance;
				}
				
				blockToolData->blockInstance = new ConstructionBlock(startPointPos, endPointPos);
				m_scene->m_gui2DObjects.push_back(blockToolData->blockInstance);
			}
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			if (m_dragData.startPoint)
			{
				m_scene->m_gui2DObjects.removeOne(m_dragData.startPoint);
				delete m_dragData.startPoint;
				m_dragData.startPoint = nullptr;
			}
			if (m_dragData.endPoint)
			{
				m_scene->m_gui2DObjects.removeOne(m_dragData.endPoint);
				delete m_dragData.endPoint;
				m_dragData.endPoint = nullptr;
			}
			if (blockToolData->blockInstance)
			{
				blockToolData->state = BlockToolState::READY_TO_EDIT;
				blockToolData->blockInstance->m_isEditingMode = true;
			}
		}
	}
	else if (blockToolData->state == BlockToolState::READY_TO_EDIT)
	{
		float x, y;
		getMouseCoordinates(m_inputData.mouseX, m_inputData.mouseY, &x, &y);
		Qt::CursorShape cursor = blockToolData->blockInstance->checkHover(x, y, m_axis, getZoomFactor());
		setCursor(cursor);
		
		if (m_inputData.keyEscape == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			m_scene->m_gui2DObjects.removeOne(blockToolData->blockInstance);
			delete blockToolData->blockInstance;
			blockToolData->blockInstance = nullptr;
			blockToolData->state = BlockToolState::CREATING;
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			BlockToolState state = blockToolData->blockInstance->startDrag(m_axis, QVector2D(x, y), getZoomFactor());
			blockToolData->state = state;
		}
	}
	else if (blockToolData->state == BlockToolState::RESIZE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float x, y;
			float step = static_cast<float>(m_grid->getStep());
			getMouseCoordinates(m_inputData.mouseX, m_inputData.mouseY, &x, &y);
			blockToolData->blockInstance->doResizeStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			blockToolData->state = BlockToolState::READY_TO_EDIT;
		}
	}
	else if (blockToolData->state == BlockToolState::MOVE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			//blockToolData.blockInstance->doMoveStep();
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			blockToolData->state = BlockToolState::READY_TO_EDIT;
		}
	}
}

void GLWidget2D::clearInputData()
{
	m_inputData.mouseScroll = MouseScroll::NO_SCROLL;
	m_inputData.keyOpenBracket = ButtonState::NOT_ACTIVE;
	m_inputData.keyCloseBracket = ButtonState::NOT_ACTIVE;

	if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
	if (m_inputData.keyEscape == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.keyEscape = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.keyEscape == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyEscape = ButtonDownState::RELEASED_PROCESSED;
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

		if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_PROCESSED)
		{
			m_inputData.leftMouseDown = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}

	QOpenGLWidget::mousePressEvent(event);
}

void GLWidget2D::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_inputData.leftMouse = ButtonState::RELEASED;
		
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			m_inputData.leftMouseDown = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
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
	case SceneZoom::X005:
		m_zoom = SceneZoom::X01;
		break;
	case SceneZoom::X01:
		m_zoom = SceneZoom::X025;
		break;
	case SceneZoom::X025:
		m_zoom = SceneZoom::X05;
		break;
	case SceneZoom::X05:
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
	case SceneZoom::X005:
		/* do nothing*/
		break;
	case SceneZoom::X01:
		m_zoom = SceneZoom::X005;
		break;
	case SceneZoom::X025:
		m_zoom = SceneZoom::X01;
		break;
	case SceneZoom::X05:
		m_zoom = SceneZoom::X025;
		break;
	case SceneZoom::X1:
		m_zoom = SceneZoom::X05;
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

void GLWidget2D::getMouseCoordinates(int screenX, int screenY, float* resX, float* resY)
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

	float factor = getZoomFactor();
	float x0 = m_frustrumWidth / 2.0f;
	float y0 = m_frustrumHeight / 2.0f;
	float shiftX = (screenX - x0) * factor;
	float shiftY = (y0 - screenY) * factor;
	*resX = camX * factor + shiftX;
	*resY = camY * factor + shiftY;
}

void GLWidget2D::getNearestPoint(float x, float y, float* nearestX, float* nearestY)
{
	float step = static_cast<float>(m_grid->getStep());

	if (x < 0.0f)
	{
		int times = x / step;
		float right = step * times;
		float left = right - step;
		if (std::abs(x - left) < std::abs(x - right))
		{
			*nearestX = left;
		}
		else
		{
			*nearestX = right;
		}
	}
	else
	{
		int times = x / step;
		float left = step * times;
		float right = left + step;
		if (std::abs(x - left) < std::abs(x - right))
		{
			*nearestX = left;
		}
		else
		{
			*nearestX = right;
		}
	}

	if (y < 0.0f)
	{
		int times = y / step;
		float top = step * times;
		float bottom = top - step;
		if (std::abs(y - top) < std::abs(y - bottom))
		{
			*nearestY = top;
		}
		else
		{
			*nearestY = bottom;
		}
	}
	else
	{
		int times = y / step;
		float bottom = step * times;
		float top = bottom + step;
		if (std::abs(y - top) < std::abs(y - bottom))
		{
			*nearestY = top;
		}
		else
		{
			*nearestY = bottom;
		}
	}
}

void GLWidget2D::getNearestPointFromScreen(int screenX, int screenY, float* nearestX, float* nearestY)
{
	float pointX, pointY;
	getMouseCoordinates(screenX, screenY, &pointX, &pointY);
	getNearestPoint(pointX, pointY, nearestX, nearestY);
}

void GLWidget2D::placePoint(int screenX, int screenY)
{
	float nearestX, nearestY;
	getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);

	Point* point;

	switch (m_axis)
	{
	case Axis::X:
		point = new Point(1.0f, 0.0f, nearestY, nearestX);
		break;
	case Axis::Y:
		point = new Point(1.0f, nearestX, 0.0f, nearestY);
		break;
	case Axis::Z:
		point = new Point(1.0f, nearestX, nearestY, 0.0f);
	}

	m_scene->m_gui2DObjects.push_back(point);
}

float GLWidget2D::getZoomFactor()
{
	float currentfactor = SCENE_ZOOM_FACTORS.find(m_zoom)->second;
	float referenceFactor = SCENE_ZOOM_FACTORS.find(REFERENCE_ZOOM)->second;
	float factor = referenceFactor / currentfactor;
	return factor;
}
