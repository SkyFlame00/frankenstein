#pragma once

#include <QOpenGLWidget>
#include "../editor/Scene.h"
#include "../editor/Renderer3D.h"
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
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	struct {
		ButtonState keyW;
		ButtonState keyA;
		ButtonState keyS;
		ButtonState keyD;
		ButtonState leftMouse;
		ButtonState rightMouse;
		float mouseX;
		float mouseY;
		bool isMouseOver;
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

	void processInputData();
	void updateCamera();
	void clearInputData();
};
