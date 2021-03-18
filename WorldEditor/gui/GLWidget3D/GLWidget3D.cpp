#include "GLWidget3D.h"

#include "../../editor/GL.h"
#include "../Debug.h"
#include "../../common/GlobalData.h"

GLWidget3D::GLWidget3D(Camera* camera, Renderer3D* renderer, Scene* scene, QWidget* parent)
	: QOpenGLWidget(parent), m_camera(camera), m_renderer(renderer), m_scene(scene)
{
}

GLWidget3D::~GLWidget3D()
{
}

void GLWidget3D::initializeGL()
{
	GlobalData::openglContexts[context()] = new GlobalData::ContextVAOMap;
	GlobalData::onContextReady();

	m_scene->setup();
	m_timer.start();
	m_renderer->setupSelectionBuffer(context());

	update();
}

void GLWidget3D::paintGL()
{
	m_timeDelta = m_timer.elapsed();

	processInputData();
	updateCamera();
	m_renderer->render(context(), m_scene->getObjects(), m_scene->m_gui3DObjects);
	clearInputData();

	m_timer.restart();

	update();
}

void GLWidget3D::processInputData()
{
	auto globalData = GlobalData::getInstance();

	if (globalData->m_editorMode == EditorMode::SELECTION_MODE)
	{
		processSelectionTool();
	}
	else if (globalData->m_editorMode == EditorMode::TEXTURE_TOOL)
	{
		processTextureTool();
	}
}

void GLWidget3D::updateCamera()
{
	auto global = GlobalData::getInstance();
	bool isWidgetActive = isCameraMode() || m_inputData.isMouseOver;
	float deltaSec = m_timeDelta / 1000.0f;
	auto& cdata = global->m_clippingToolData;
	auto& sdata = global->m_selectionToolData;
	
	processMouseMovement();

	if (m_inputData.keyW == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::FORWARD, deltaSec);
	if (m_inputData.keyA == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::LEFT, deltaSec);
	if (m_inputData.keyS == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::BACKWARD, deltaSec);
	if (m_inputData.keyD == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::RIGHT, deltaSec);

	if (m_inputData.keyV == ButtonDownState::DOWN_NOT_PROCESSED)
		global->m_isDrawingLines = !global->m_isDrawingLines;
	if (m_inputData.keyF == ButtonDownState::DOWN_NOT_PROCESSED)
		global->m_isWireframeMode = !global->m_isWireframeMode;
	if (m_inputData.keyR == ButtonDownState::DOWN_NOT_PROCESSED && global->m_editorMode == EditorMode::CLIPPING_MODE)
	{
		auto* brush = sdata.renderable;

		if (brush)
		{
			std::swap(brush->m_clippedBrush, brush->m_remainingBrush);
			if (brush->m_clippedBrush)
				brush->m_clippedBrush->m_beingCut = true;
			if (brush->m_remainingBrush)
				brush->m_remainingBrush->m_beingCut = false;
		}
	}

	m_camera->updateCameraVectors();
}

void GLWidget3D::clearInputData()
{
	if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
	if (m_inputData.keyV == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.keyV = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.keyV == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyV = ButtonDownState::RELEASED_PROCESSED;
	if (m_inputData.keyF == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.keyF = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.keyF == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyF = ButtonDownState::RELEASED_PROCESSED;
	if (m_inputData.keyR == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.keyR = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.keyR == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyR = ButtonDownState::RELEASED_PROCESSED;

	if (m_inputData.keyCtrl == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyCtrl = ButtonDownState::RELEASED_PROCESSED;
	if (m_inputData.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.keyCtrl = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.keyCtrl == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.keyCtrl = ButtonDownState::RELEASED_PROCESSED;
}

void GLWidget3D::enterEvent(QEvent* event)
{
	m_inputData.isMouseOver = true;
	QOpenGLWidget::enterEvent(event);
}
										  
void GLWidget3D::leaveEvent(QEvent* event)
{
	m_inputData.isMouseOver = false;
	QOpenGLWidget::leaveEvent(event);
}

void GLWidget3D::mouseMoveEvent(QMouseEvent* event)
{
	auto pos = event->pos();
	m_inputData.mouseX = pos.x();
	m_inputData.mouseY = pos.y();

	QOpenGLWidget::mouseMoveEvent(event);
}

void GLWidget3D::mousePressEvent(QMouseEvent* event)
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

void GLWidget3D::mouseReleaseEvent(QMouseEvent* event)
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

void GLWidget3D::resizeGL(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_renderer->setFrustrum(width, height);
}

bool GLWidget3D::pickBrush(Brush** brush, Types::Polygon** polygon, Types::Triangle** triangle, QVector3D* point)
{
	auto global = GlobalData::getInstance();
	auto& data = global->m_selectionToolData;
	int mouseX = m_inputData.mouseX;
	int mouseY = m_inputData.mouseY;
	float renderId = m_renderer->getSelectionValue(mouseX, m_screenHeight - mouseY);
	auto renderable = m_renderer->getBrushByRenderId(renderId);

	if (renderId == 0.0f || !renderable)
	{
		if (data.renderable)
		{
			data.renderable->m_selected = false;
			data.renderable = nullptr;
		}
		return false;
	}

	QVector3D pointingRay =
		m_camera->getPickingRay(mouseX, mouseY, m_screenWidth, m_screenHeight, m_renderer->getNearPlane(), m_renderer->getFarPlane(), m_renderer->getProjMatrix());
	QVector3D intersectionPoint;
	Types::Polygon* intersectionPolygon = nullptr;
	Types::Triangle* intersectionTriangle = nullptr;

	for (auto& polygon : renderable->getPolygons())
	{
		for (auto& triangle : polygon->triangles)
		{
			auto cameraPos = m_camera->getPosition();
			QVector3D pt;
			if (hasIntersection(cameraPos, pointingRay, triangle, renderable->m_origin, pt))
			{
				if (!intersectionPolygon)
				{
					intersectionPolygon = polygon;
					intersectionTriangle = &triangle;
					intersectionPoint = pt;
					continue;
				}

				float d1 = std::abs(cameraPos.distanceToPoint(intersectionPoint));
				float d2 = std::abs(cameraPos.distanceToPoint(pt));

				if (d2 < d1)
				{
					intersectionPolygon = polygon;
					intersectionTriangle = &triangle;
					intersectionPoint = pt;
				}
			}
		}
	}

	if (brush)
		*brush = renderable;
	if (polygon)
		*polygon = intersectionPolygon;
	if (triangle)
		*triangle = intersectionTriangle;
	if (point)
		*point = intersectionPoint;

	return true;
}

bool GLWidget3D::isWidgetActive()
{
	return m_inputData.isMouseOver;
}

bool GLWidget3D::isCameraMode()
{
	auto* global = GlobalData::getInstance();

	return (global->m_editorMode == EditorMode::CAMERA_MODE || global->isCameraForTextureToolActivated) &&
		m_inputData.leftMouse == ButtonState::PRESSED;
}

void GLWidget3D::processMouseMovement()
{
	int offsetX = 0;
	int offsetY = 0;

	if (isCameraMode())
	{
		if (m_isCameraModeStarted)
		{
			offsetX = m_inputData.mouseX - lastX;
			offsetY = lastY - m_inputData.mouseY;
			lastX = m_inputData.mouseX;
			lastY = m_inputData.mouseY;
		}
		else
		{
			lastX = m_inputData.mouseX;
			lastY = m_inputData.mouseY;
			m_isCameraModeStarted = true;
		}
	}
	else
	{
		m_isCameraModeStarted = false;
	}

	if (m_inputData.leftMouse == ButtonState::PRESSED && isCameraMode() && m_isCameraModeStarted)
		m_camera->processMouseMovement(offsetX, offsetY);
}
