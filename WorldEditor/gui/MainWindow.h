#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "GLWidget3D.h"
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
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void setupMenu();
	void setupToolbar();
	void setupDocks();
	void setupEditor();
	void enableMouseTracking();

	QWidget* m_centralWidget;
	GLWidget3D* m_glWidget3D;
	Camera* m_camera3D;
	Scene* m_scene;
	Renderer3D* m_renderer3D;

private slots:
};
