#include "MainWindow.h"

#include <QToolBar>
#include <QDockWidget>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>

#include "../common/types.h"

MainWindow::MainWindow()
{
	m_centralWidget = new QWidget;
	m_camera3D = new Camera;
	m_renderer3D = new Renderer3D(m_camera3D, 800, 600);
	m_scene = new Scene;
	
	setCentralWidget(m_centralWidget);
	m_centralWidget->setMouseTracking(true);

	setupMenu();
	setupToolbar();
	setupDocks();
	setupEditor();
	enableMouseTracking();
}

MainWindow::~MainWindow()
{
	delete m_camera3D;
	delete m_renderer3D;
	delete m_scene;
}

void MainWindow::setupMenu()
{
	menuBar = new QMenuBar;
	fileMenu = menuBar->addMenu("File");
	QAction* closeLevel = new QAction(fileMenu);
	closeLevel->setText("Close");
	fileMenu->addAction(closeLevel);

	setAnimated(false);
	setMenuBar(menuBar);
}

void MainWindow::setupToolbar()
{
	QToolBar* toolbar = new QToolBar("resize");
	QAction* resize = new QAction(toolbar);
	resize->setText("Resize");
	toolbar->addAction(resize);
	toolbar->setMovable(false);

	addToolBar(Qt::ToolBarArea::TopToolBarArea, toolbar);
}

void MainWindow::setupDocks()
{
}

void MainWindow::setupEditor()
{
	m_glWidget3D = new GLWidget3D(m_camera3D, m_renderer3D, m_scene);
	m_glWidget3D->setMaximumWidth(800);
	m_glWidget3D->setMaximumHeight(600);
	m_glWidget3D->setMouseTracking(true);

	QVBoxLayout* layout = new QVBoxLayout;
	m_centralWidget->setLayout(layout);
	layout->addWidget(m_glWidget3D);
}

void MainWindow::enableMouseTracking()
{
	setMouseTracking(true);
	m_centralWidget->setMouseTracking(true);
	m_glWidget3D->setMouseTracking(true);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	qInfo() << "key pressed" << event->key();

	switch (event->key())
	{
	case 'W':
		m_glWidget3D->m_inputData.keyW = ButtonState::PRESSED;
	case 'A':
		m_glWidget3D->m_inputData.keyA = ButtonState::PRESSED;
	case 'S':
		m_glWidget3D->m_inputData.keyS = ButtonState::PRESSED;
	case 'D':
		m_glWidget3D->m_inputData.keyD = ButtonState::PRESSED;
	}

	QMainWindow::keyPressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	//qInfo() << "Mouse move event" << event->pos();
	auto pos = event->pos();
	m_glWidget3D->m_inputData.mouseX = pos.x();
	m_glWidget3D->m_inputData.mouseY = pos.y();
}
