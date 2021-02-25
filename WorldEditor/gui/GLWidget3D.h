#pragma once

#include <QOpenGLWidget>
#include <QElapsedTimer>
#include "../editor/Scene.h"
#include "../editor/Renderer3D.h"
#include "../editor/Camera.h"
#include "../common/types.h"

class GLWidget3D : public QOpenGLWidget
{
	Q_OBJECT

public:
	GLWidget3D(Camera* camera, Renderer3D* renderer, Scene* scene, QWidget *parent = nullptr);
	~GLWidget3D();

	void initializeGL() override;
	void paintGL() override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	struct {
		ButtonState keyW = ButtonState::NOT_ACTIVE;
		ButtonState keyA = ButtonState::NOT_ACTIVE;
		ButtonState keyS = ButtonState::NOT_ACTIVE;
		ButtonState keyD = ButtonState::NOT_ACTIVE;
		ButtonState leftMouse = ButtonState::NOT_ACTIVE;
		ButtonState rightMouse = ButtonState::NOT_ACTIVE;
		float mouseX;
		float mouseY;
		bool isMouseOver = false;
		bool isInitialized = false;
	} m_inputData;

private:
	Camera* m_camera;
	Scene* m_scene;
	Renderer3D* m_renderer;
	bool m_isCameraToolPicked = true;
	bool m_isCameraModeStarted = false;
	int lastX;
	int lastY;
	QElapsedTimer m_timer;
	float m_timeDelta = 0.0f;
	float m_frustrumWidth;
	float m_frustrumHeight;

	void processInputData();
	void updateCamera();
	void clearInputData();
	void resizeGL(int width, int height) override;
};
