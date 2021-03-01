#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "GLWidget3D/GLWidget3D.h"
#include "GLWidget2D/GLWidget2D.h"
#include "../editor/Scene.h"
#include "../editor/Renderer3D.h"
#include "../editor/Camera.h"
#include <QKeyEvent>
#include "GLWidgetsContainer.h"

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

private:
	void setupMenu();
	void setupTopToolbar();
	void setupLeftToolbar();
	void setupDocks();
	void setupEditor();
	void enableMouseTracking();
	void handleToolChange(QAction* action);

	QWidget* m_centralWidget;
	GLWidget3D* m_glWidget3D;
	GLWidget2D* m_glWidget2D_X, *m_glWidget2D_Y, *m_glWidget2D_Z;
	Camera* m_camera3D;
	Camera* m_camera2D_X, *m_camera2D_Y, *m_camera2D_Z;
	Scene* m_scene;
	Renderer3D* m_renderer3D;
	Renderer2D* m_renderer2D_X, *m_renderer2D_Y, *m_renderer2D_Z;
	GLWidgetsContainer* m_glWidgetsContainer;

	/* Left toolbar */
	QToolBar* m_leftToolbar;
	QActionGroup* m_leftToolbarGroup;
	QAction* m_selectionToolButton;
	QAction* m_blockToolButton;
};
