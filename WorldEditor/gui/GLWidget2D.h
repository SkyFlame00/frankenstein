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

	enum class ButtonDownState
	{
		READY, ACTIVE, PROCESSED
	};

	struct {
		ButtonState keyW = ButtonState::NOT_ACTIVE;
		ButtonState keyA = ButtonState::NOT_ACTIVE;
		ButtonState keyS = ButtonState::NOT_ACTIVE;
		ButtonState keyD = ButtonState::NOT_ACTIVE;
		ButtonState keyOpenBracket = ButtonState::NOT_ACTIVE;
		ButtonState keyCloseBracket = ButtonState::NOT_ACTIVE;
		ButtonState leftMouse = ButtonState::NOT_ACTIVE;
		ButtonDownState leftMouseDown = ButtonDownState::READY;
		bool isMouseOver = false;
		MouseScroll mouseScroll = MouseScroll::NO_SCROLL;
		int mouseX;
		int mouseY;
	} m_inputData;

	GLWidget2D(Camera* camera, Renderer2D* renderer, Scene* scene, QWidget* parent = nullptr);
	~GLWidget2D();

	void initializeGL() override;
	void paintGL() override;
	void processInputData();
	void clearInputData();
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void placePoint(int screenX, int screenY);

private:
	const SceneZoom REFERENCE_ZOOM = SceneZoom::X1;

	Camera* m_camera;
	Scene* m_scene;
	Renderer2D* m_renderer;
	Grid2D* m_grid;
	Axis m_axis;
	float m_frustrumWidth;
	float m_frustrumHeight;
	SceneZoom m_zoom = SceneZoom::X32;
	QList<Renderable*> m_guiObjects;

	void zoomIn();
	void zoomOut();
};


