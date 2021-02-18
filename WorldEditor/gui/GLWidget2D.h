#pragma once

#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QVector3D>
#include "../editor/Scene.h"
#include "../editor/Renderer2D.h"
#include "../editor/Camera.h"
#include "../common/types.h"
#include "../editor/Grid2D.h"
#include "../editor/grid2d/Point.h"

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
		ButtonDownState keyEscape = ButtonDownState::RELEASED_PROCESSED;
		ButtonState keyOpenBracket = ButtonState::NOT_ACTIVE;
		ButtonState keyCloseBracket = ButtonState::NOT_ACTIVE;
		ButtonState leftMouse = ButtonState::NOT_ACTIVE;
		ButtonDownState leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
		bool isMouseOver = false;
		MouseScroll mouseScroll = MouseScroll::NO_SCROLL;
		int mouseX;
		int mouseY;
	} m_inputData;
	struct {
		Point* startPoint = nullptr;
		Point* endPoint = nullptr;
	} m_dragData;

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
	void getMouseCoordinates(int screenX, int screenY, float* resX, float* resY);
	void getNearestPoint(float x, float y, float* nearestX, float* nearestY);
	void getNearestPointFromScreen(int screenX, int screenY, float* nearestX, float* nearestY);
	void placePoint(int screenX, int screenY);
	void processToolMode();
	float getZoomFactor();
	bool isCorrectPoints(QVector3D p1, QVector3D p2);

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

	void zoomIn();
	void zoomOut();
};


