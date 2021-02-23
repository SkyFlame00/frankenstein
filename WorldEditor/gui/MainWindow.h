#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "GLWidget3D.h"
#include "GLWidget2D.h"
#include "../editor/Scene.h"
#include "../editor/Renderer3D.h"
#include "../editor/Camera.h"
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	QMenuBar* menuBar;
	QMenu* fileMenu;

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void setupMenu();
	void setupToolbar();
	void setupDocks();
	void setupEditor();
	void enableMouseTracking();

	QWidget* m_centralWidget;
	GLWidget3D* m_glWidget3D;
	GLWidget2D* m_glWidget2D_X, *m_glWidget2D_Y, *m_glWidget2D_Z;
	Camera* m_camera3D;
	Camera* m_camera2D_X, *m_camera2D_Y, *m_camera2D_Z;
	Scene* m_scene;
	Renderer3D* m_renderer3D;
	Renderer2D* m_renderer2D_X, *m_renderer2D_Y, *m_renderer2D_Z;

private slots:
};
