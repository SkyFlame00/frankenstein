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

	if (globalData->m_editorMode == EditorMode::SELECTING_MODE)
	{
		processSelectionTool();
	}
}

void GLWidget3D::updateCamera()
{
	bool isCameraMode = m_isCameraToolPicked && m_inputData.leftMouse == ButtonState::PRESSED;
	bool isWidgetActive = isCameraMode || m_inputData.isMouseOver;
	float deltaSec = m_timeDelta / 1000.0f;
	int offsetX = 0;
	int offsetY = 0;

	if (isCameraMode)
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

	if (m_inputData.leftMouse == ButtonState::PRESSED && isCameraMode && m_isCameraModeStarted)
		m_camera->processMouseMovement(offsetX, offsetY);
	if (m_inputData.keyW == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::FORWARD, deltaSec);
	if (m_inputData.keyA == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::LEFT, deltaSec);
	if (m_inputData.keyS == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::BACKWARD, deltaSec);
	if (m_inputData.keyD == ButtonState::PRESSED && isWidgetActive)
		m_camera->processKeyboard(Camera::Direction::RIGHT, deltaSec);

	m_camera->updateCameraVectors();
}

void GLWidget3D::clearInputData()
{
	if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::DOWN_PROCESSED;
	if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		m_inputData.leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
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
