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
#include "../common/GlobalData.h"
#include "../common/helpers.h"

MainWindow::MainWindow()
{
	m_centralWidget = new QWidget;
	m_scene = new Scene;

	m_camera3D = new Camera;
	m_renderer3D = new Renderer3D(m_camera3D, 800, 600);

	m_camera2D_X = new Camera;
	m_renderer2D_X = new Renderer2D(m_camera2D_X, 800.0f, 600.0f);

	m_camera2D_Y = new Camera;
	m_renderer2D_Y = new Renderer2D(m_camera2D_Y, 800.0f, 600.0f);

	m_camera2D_Z = new Camera;
	m_renderer2D_Z = new Renderer2D(m_camera2D_Z, 800.0f, 600.0f);

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
	delete m_camera2D_X;
	delete m_renderer2D_X;
	delete m_camera2D_Y;
	delete m_renderer2D_Y;
	delete m_camera2D_Z;
	delete m_renderer2D_Z;
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
	QHBoxLayout* layout = new QHBoxLayout;
	m_glWidget3D = new GLWidget3D(m_camera3D, m_renderer3D, m_scene);
	m_glWidget2D_X = new GLWidget2D(Axis::X, m_camera2D_X, m_renderer2D_X, m_scene);
	m_glWidget2D_Y = new GLWidget2D(Axis::Y, m_camera2D_Y, m_renderer2D_Y, m_scene);
	m_glWidget2D_Z = new GLWidget2D(Axis::Z, m_camera2D_Z, m_renderer2D_Z, m_scene);
	m_glWidgetsContainer = new GLWidgetsContainer(m_glWidget3D, m_glWidget2D_X, m_glWidget2D_Y, m_glWidget2D_Z);

	layout->addWidget(m_glWidgetsContainer);
	m_centralWidget->setLayout(layout);
}

void MainWindow::enableMouseTracking()
{
	setMouseTracking(true);
	m_centralWidget->setMouseTracking(true);
	m_glWidgetsContainer->setMouseTracking(true);
	m_glWidget3D->setMouseTracking(true);
	m_glWidget2D_X->setMouseTracking(true);
	m_glWidget2D_Y->setMouseTracking(true);
	m_glWidget2D_Z->setMouseTracking(true);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	auto key = event->key();

	if (key == 'W')
	{
		m_glWidget3D->m_inputData.keyW = ButtonState::PRESSED;
		m_glWidget2D_X->m_inputData.keyW = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyW = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyW = ButtonState::PRESSED;
	}
	if (key == 'A')
	{
		m_glWidget3D->m_inputData.keyA = ButtonState::PRESSED;
		m_glWidget2D_X->m_inputData.keyA = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyA = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyA = ButtonState::PRESSED;
	}
	if (key == 'S')
	{
		m_glWidget3D->m_inputData.keyS = ButtonState::PRESSED;
		m_glWidget2D_X->m_inputData.keyS = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyS = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyS = ButtonState::PRESSED;
	}
	if (key == 'D')
	{
		m_glWidget3D->m_inputData.keyD = ButtonState::PRESSED;
		m_glWidget2D_X->m_inputData.keyD = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyD = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyD = ButtonState::PRESSED;
	}
	if (key == '[')
	{
		m_glWidget2D_X->m_inputData.keyOpenBracket = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyOpenBracket = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyOpenBracket = ButtonState::PRESSED;
	}
	if (key == ']')
	{
		m_glWidget2D_X->m_inputData.keyCloseBracket = ButtonState::PRESSED;
		m_glWidget2D_Y->m_inputData.keyCloseBracket = ButtonState::PRESSED;
		m_glWidget2D_Z->m_inputData.keyCloseBracket = ButtonState::PRESSED;
	}
	if (key == Qt::Key_Escape)
	{
		if (m_glWidget2D_X->m_inputData.keyEscape == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_X->m_inputData.keyEscape = ButtonDownState::DOWN_NOT_PROCESSED;
		}
		if (m_glWidget2D_Y->m_inputData.keyEscape == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_Y->m_inputData.keyEscape = ButtonDownState::DOWN_NOT_PROCESSED;
		}
		if (m_glWidget2D_Z->m_inputData.keyEscape == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_Z->m_inputData.keyEscape = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Return)
	{
		auto global = GlobalData::getInstance();
		auto& data = global->m_blockToolData;

		if (global->m_editorMode == EditorMode::BLOCK_MODE && data.state == BlockToolState::READY_TO_EDIT)
		{
			QVector3D color(Helpers::getRandom(), Helpers::getRandom(), Helpers::getRandom());

			auto block = data.blockInstance;
			auto brush = new Brush(*block->getVertices(), color);
			m_scene->addObject(brush);

			m_scene->m_gui2DObjects.removeOne(block);
			m_scene->m_gui3DObjects.removeOne(block);
			delete data.blockInstance;
			data.blockInstance = nullptr;
			data.state = BlockToolState::CREATING;
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
		m_glWidget2D_X->m_inputData.keyW = ButtonState::RELEASED;
		m_glWidget2D_Y->m_inputData.keyW = ButtonState::RELEASED;
		m_glWidget2D_Z->m_inputData.keyW = ButtonState::RELEASED;
	}
	if (key == 'A')
	{
		m_glWidget3D->m_inputData.keyA = ButtonState::RELEASED;
		m_glWidget2D_X->m_inputData.keyA = ButtonState::RELEASED;
		m_glWidget2D_Y->m_inputData.keyA = ButtonState::RELEASED;
		m_glWidget2D_Z->m_inputData.keyA = ButtonState::RELEASED;
	}
	if (key == 'S')
	{
		m_glWidget3D->m_inputData.keyS = ButtonState::RELEASED;
		m_glWidget2D_X->m_inputData.keyS = ButtonState::RELEASED;
		m_glWidget2D_Y->m_inputData.keyS = ButtonState::RELEASED;
		m_glWidget2D_Z->m_inputData.keyS = ButtonState::RELEASED;
	}
	if (key == 'D')
	{
		m_glWidget3D->m_inputData.keyD = ButtonState::RELEASED;
		m_glWidget2D_X->m_inputData.keyD = ButtonState::RELEASED;
		m_glWidget2D_Y->m_inputData.keyD = ButtonState::RELEASED;
		m_glWidget2D_Z->m_inputData.keyD = ButtonState::RELEASED;
	}
	if (key == Qt::Key_Escape)
	{
		if (m_glWidget2D_X->m_inputData.keyEscape == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_X->m_inputData.keyEscape = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
		if (m_glWidget2D_Y->m_inputData.keyEscape == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_Y->m_inputData.keyEscape = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
		if (m_glWidget2D_Z->m_inputData.keyEscape == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_Z->m_inputData.keyEscape = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}

	QMainWindow::keyReleaseEvent(event);
}
