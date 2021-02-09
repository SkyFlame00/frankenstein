#include "GLWidget3D.h"

#include "../editor/GL.h"
#include "Debug.h"

GLWidget3D::GLWidget3D(Camera* camera, Renderer3D* renderer, Scene* scene, QWidget* parent)
	: QOpenGLWidget(parent), m_camera(camera), m_renderer(renderer), m_scene(scene)
{
}

GLWidget3D::~GLWidget3D()
{
}

void GLWidget3D::initializeGL()
{
	m_scene->setup();
}

void GLWidget3D::paintGL()
{
	processInputData();
	updateCamera();
	m_renderer->render(m_scene->getObjects());
	clearInputData();
	update();
}

void GLWidget3D::processInputData()
{

}

void GLWidget3D::updateCamera()
{
	bool isCameraMode = m_isCameraToolPicked && m_inputData.leftMouse == ButtonState::PRESSED;
	bool isWidgetActive = isCameraMode || m_inputData.isMouseOver;
	int offsetX = 0;
	int offsetY = 0;

	//qInfo() << "iscameramode" << isCameraMode;

	if (isCameraMode)
	{
		if (m_isCameraModeStarted)
		{
			offsetX = m_inputData.mouseX - lastX;
			offsetY = m_inputData.mouseY - lastY;
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
		m_camera->processKeyboard(Camera::Direction::FORWARD, 0);

	m_camera->updateCameraVectors();
}

void GLWidget3D::clearInputData()
{

}

void GLWidget3D::enterEvent(QEvent* event)
{
	m_inputData.isMouseOver = true;
}
										  
void GLWidget3D::leaveEvent(QEvent* event)
{
	m_inputData.isMouseOver = false;
}

void GLWidget3D::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		m_inputData.leftMouse = ButtonState::PRESSED;

	QOpenGLWidget::mousePressEvent(event);
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		m_inputData.leftMouse = ButtonState::RELEASED;

	QOpenGLWidget::mouseReleaseEvent(event);
}

void GLWidget3D::keyPressEvent(QKeyEvent* event)
{
	qInfo() << "key pressed" << event->key();
	QOpenGLWidget::keyPressEvent(event);
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* event)
{
	QOpenGLWidget::keyReleaseEvent(event);
}
