#include "MainWindow.h"

#include <QToolBar>
#include <QDockWidget>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>
#include <QVector3D>

#include "../common/types.h"

MainWindow::MainWindow()
{
	m_centralWidget = new QWidget;
	m_scene = new Scene;
	m_camera3D = new Camera;
	m_renderer3D = new Renderer3D(m_camera3D, 800, 600);
	m_camera2D = new Camera;
	m_renderer2D = new Renderer2D(m_camera2D, 800.0f, 600.0f);
	
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

	m_glWidget2D = new GLWidget2D(m_camera2D, m_renderer2D, m_scene);
	m_glWidget2D->setMaximumWidth(800);
	m_glWidget2D->setMaximumHeight(600);
	m_glWidget2D->setMouseTracking(true);

	QVBoxLayout* layout = new QVBoxLayout;
	m_centralWidget->setLayout(layout);
	layout->addWidget(m_glWidget3D);
	layout->addWidget(m_glWidget2D);
}

void MainWindow::enableMouseTracking()
{
	setMouseTracking(true);
	m_centralWidget->setMouseTracking(true);
	m_glWidget3D->setMouseTracking(true);
	m_glWidget2D->setMouseTracking(true);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	auto key = event->key();

	if (key == 'W')
	{
		m_glWidget3D->m_inputData.keyW = ButtonState::PRESSED;
		m_glWidget2D->m_inputData.keyW = ButtonState::PRESSED;
	}
		
	if (key == 'A')
	{
		m_glWidget3D->m_inputData.keyA = ButtonState::PRESSED;
		m_glWidget2D->m_inputData.keyA = ButtonState::PRESSED;
	}
	if (key == 'S')
	{
		m_glWidget3D->m_inputData.keyS = ButtonState::PRESSED;
		m_glWidget2D->m_inputData.keyS = ButtonState::PRESSED;
	}
	if (key == 'D')
	{
		m_glWidget3D->m_inputData.keyD = ButtonState::PRESSED;
		m_glWidget2D->m_inputData.keyD = ButtonState::PRESSED;
	}
	if (key == '[')
	{
		m_glWidget2D->m_inputData.keyOpenBracket = ButtonState::PRESSED;
	}
	if (key == ']')
	{
		m_glWidget2D->m_inputData.keyCloseBracket = ButtonState::PRESSED;
	}
	if (key == Qt::Key_Escape)
	{
		if (m_glWidget2D->m_inputData.keyEscape == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D->m_inputData.keyEscape = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}

	QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	auto key = event->key();

	if (key == 'W')
	{
		m_glWidget3D->m_inputData.keyW = ButtonState::RELEASED;
		m_glWidget2D->m_inputData.keyW = ButtonState::RELEASED;
	}
	if (key == 'A')
	{
		m_glWidget3D->m_inputData.keyA = ButtonState::RELEASED;
		m_glWidget2D->m_inputData.keyA = ButtonState::RELEASED;
	}
	if (key == 'S')
	{
		m_glWidget3D->m_inputData.keyS = ButtonState::RELEASED;
		m_glWidget2D->m_inputData.keyS = ButtonState::RELEASED;
	}
	if (key == 'D')
	{
		m_glWidget3D->m_inputData.keyD = ButtonState::RELEASED;
		m_glWidget2D->m_inputData.keyD = ButtonState::RELEASED;
	}
	if (key == Qt::Key_Escape)
	{
		if (m_glWidget2D->m_inputData.keyEscape == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D->m_inputData.keyEscape = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}

	QMainWindow::keyReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	auto pos = event->pos();
	m_glWidget3D->m_inputData.mouseX = pos.x();
	m_glWidget3D->m_inputData.mouseY = pos.y();

	QMainWindow::mouseMoveEvent(event);
}
