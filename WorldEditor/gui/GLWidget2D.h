#pragma once

#include <QOpenGLWidget>
#include <QWheelEvent>
#include "../editor/Scene.h"
#include "../editor/Renderer2D.h"
#include "../editor/Camera.h"
#include "../common/types.h"
#include "../editor/Grid2D.h"
#include "../common/types.h"

class GLWidget2D : public QOpenGLWidget
{
public:
	enum class MouseScroll
	{
		NO_SCROLL, UP, DOWN
	};

	struct {
		ButtonState keyW = ButtonState::NOT_ACTIVE;
		ButtonState keyA = ButtonState::NOT_ACTIVE;
		ButtonState keyS = ButtonState::NOT_ACTIVE;
		ButtonState keyD = ButtonState::NOT_ACTIVE;
		ButtonState keyOpenBracket = ButtonState::NOT_ACTIVE;
		ButtonState keyCloseBracket = ButtonState::NOT_ACTIVE;
		bool isMouseOver = false;
		MouseScroll mouseScroll = MouseScroll::NO_SCROLL;
	} m_inputData;

	GLWidget2D(Camera* camera, Renderer2D* renderer, Scene* scene, QWidget* parent = nullptr);
	~GLWidget2D();

	void initializeGL() override;
	void paintGL() override;
	void processInputData();
	void clearInputData();
	void GLWidget2D::enterEvent(QEvent* event) override;
	void GLWidget2D::leaveEvent(QEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void resizeGL(int width, int height) override;

private:
	const float FRUSTRUM_SIZE_FACTOR = 7.0f;

	Camera* m_camera;
	Scene* m_scene;
	Renderer2D* m_renderer;
	Grid2D* m_grid;
	Axis m_axis;
	float m_frustrumWidth;
	float m_frustrumHeight;
	SceneZoom m_zoom = SceneZoom::X1;

	void zoomIn();
	void zoomOut();
};


