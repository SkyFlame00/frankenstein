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
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <nlohmann/json.hpp>

#include "../common/types.h"
#include "../common/GlobalData.h"
#include "../common/helpers.h"
#include "../common/actions.h"
#include "../common/ActionHistoryTool.h"
#include "../editor/ResourceManager.h"

using nlohmann::json;

MainWindow* MainWindow::m_instance = nullptr;

MainWindow::MainWindow()
{
	m_centralWidget = new QWidget;
	m_scene = new Scene;
	GlobalData::getInstance()->m_scene = m_scene;

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
	setupTopToolbar();
	setupLeftToolbar();
	setupDocks();
	setupEditor();
	setupWindows();
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

void MainWindow::init()
{
	if (m_instance)
	{
		qInfo() << "MainWindow has already been initialized";
	}

	m_instance = new MainWindow;
	m_instance->handleToolChange(m_instance->getSelectionToolButton());
}

void MainWindow::cleanup()
{
}

void MainWindow::setupMenu()
{
	menuBar = new QMenuBar(this);
	fileMenu = menuBar->addMenu("File");

	/* New */
	m_newBtn = new QAction("New", fileMenu);
	fileMenu->addAction(m_newBtn);
	connect(m_newBtn, &QAction::triggered, this, &MainWindow::handleNewButtonClicked);

	/* Open file */
	m_openFileBtn = new QAction("Open", fileMenu);
	fileMenu->addAction(m_openFileBtn);
	connect(m_openFileBtn, &QAction::triggered, this, &MainWindow::handleOpenFileButtonClicked);

	/* Separator */
	fileMenu->addSeparator();

	/* Save */

	/* Save as */
	m_saveAsBtn = new QAction("Save as...", fileMenu);
	fileMenu->addAction(m_saveAsBtn);
	connect(m_saveAsBtn, &QAction::triggered, this, &MainWindow::handleSaveAsButtonClicked);

	/* Separator */
	fileMenu->addSeparator();

	/* Close */
	m_closeBtn = new QAction("Close", fileMenu);
	fileMenu->addAction(m_closeBtn);
	connect(m_closeBtn, &QAction::triggered, this, &MainWindow::handleCloseButtonClicked);

	/* Exit */
	m_exitBtn = new QAction("Exit", fileMenu);
	fileMenu->addAction(m_exitBtn);
	connect(m_exitBtn, &QAction::triggered, this, &MainWindow::handleExitButtonClicked);

	setAnimated(false);
	setMenuBar(menuBar);
}

void MainWindow::setupTopToolbar()
{
	/* Toolbar itself */
	m_topToolbar = new QToolBar;
	m_topToolbar->setMovable(false);

	/* Action history */
	m_historyGroup = new QActionGroup(m_topToolbar);

	m_historyBackButton = new QAction(m_historyGroup);
	m_historyBackButton->setIcon(QIcon("assets/icons/history_back.png"));
	m_historyBackButton->setDisabled(true);
	m_topToolbar->addAction(m_historyBackButton);
	m_historyGroup->addAction(m_historyBackButton);

	m_historyForthButton = new QAction(m_historyGroup);
	m_historyForthButton->setIcon(QIcon("assets/icons/history_forth.png"));
	m_historyForthButton->setDisabled(true);
	m_topToolbar->addAction(m_historyForthButton);
	m_historyGroup->addAction(m_historyForthButton);

	addToolBar(Qt::ToolBarArea::TopToolBarArea, m_topToolbar);
	connect(m_historyGroup, &QActionGroup::triggered, this, &MainWindow::handleHistoryChange);
}

void MainWindow::setupLeftToolbar()
{
	/* Toolbar itself */
	QToolBar* m_leftToolbar = new QToolBar;
	m_leftToolbar->setIconSize(QSize(36, 36));
	m_leftToolbar->setMovable(false);
	addToolBar(Qt::ToolBarArea::LeftToolBarArea, m_leftToolbar);

	/* Selection tool */
	m_selectionToolButton = new QAction(m_leftToolbar);
	m_selectionToolButton->setIcon(QIcon("assets/icons/selection_tool.png"));
	m_selectionToolButton->setCheckable(true);
	m_leftToolbar->addAction(m_selectionToolButton);

	/* Camera tool */
	m_cameraToolButton = new QAction(m_leftToolbar);
	m_cameraToolButton->setIcon(QIcon("assets/icons/camera_tool.png"));
	m_cameraToolButton->setCheckable(true);
	m_leftToolbar->addAction(m_cameraToolButton);

	/* Block tool */
	m_blockToolButton = new QAction(m_leftToolbar);
	m_blockToolButton->setIcon(QIcon("assets/icons/block_tool.png"));
	m_blockToolButton->setCheckable(true);
	m_leftToolbar->addAction(m_blockToolButton);

	/* Clipping tool */
	m_clippingToolButton = new QAction(m_leftToolbar);
	m_clippingToolButton->setIcon(QIcon("assets/icons/clipping_tool.png"));
	m_clippingToolButton->setCheckable(true);
	m_leftToolbar->addAction(m_clippingToolButton);

	/* Texture tool */
	m_textureToolButton = new QAction(m_leftToolbar);
	m_textureToolButton->setIcon(QIcon("assets/icons/texture_tool.png"));
	m_textureToolButton->setCheckable(true);
	m_leftToolbar->addAction(m_textureToolButton);

	/* Action group */
	m_leftToolbarGroup = new QActionGroup(m_leftToolbar);
	m_leftToolbarGroup->addAction(m_selectionToolButton);
	m_leftToolbarGroup->addAction(m_cameraToolButton);
	m_leftToolbarGroup->addAction(m_blockToolButton);
	m_leftToolbarGroup->addAction(m_clippingToolButton);
	m_leftToolbarGroup->addAction(m_textureToolButton);
	m_leftToolbarGroup->setExclusive(false);

	connect(m_leftToolbarGroup, &QActionGroup::triggered, this, &MainWindow::handleToolChange);
	m_selectionToolButton->setChecked(true);
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

	closeEditor();

	layout->addWidget(m_glWidgetsContainer);
	m_centralWidget->setLayout(layout);
}

void MainWindow::setupWindows()
{
	m_textureToolDialog = new TextureToolDialog(this, Qt::Tool);
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
	if (event->isAutoRepeat())
		return;

	auto key = event->key();
	auto global = GlobalData::getInstance();
	auto& bdata = global->m_blockToolData;
	auto& sdata = global->m_selectionToolData;
	auto& cdata = global->m_clippingToolData;

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
	if (key == Qt::Key_V)
	{
		if (m_glWidget3D->m_inputData.keyV == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyV = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_F)
	{
		if (m_glWidget3D->m_inputData.keyF == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyF = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_R)
	{
		if (m_glWidget3D->m_inputData.keyR == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyR = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Control)
	{
		if (m_inputData.keyCtrl == ButtonDownState::RELEASED_PROCESSED)
		{
			m_inputData.keyCtrl = ButtonDownState::DOWN_NOT_PROCESSED;
		}

		if (m_glWidget3D->m_inputData.keyCtrl == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyCtrl = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Z)
	{
		if (m_inputData.keyZ == ButtonDownState::RELEASED_PROCESSED)
		{
			m_inputData.keyZ = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Shift)
	{
		if (m_inputData.keyShift == ButtonDownState::RELEASED_PROCESSED)
		{
			m_inputData.keyShift = ButtonDownState::DOWN_NOT_PROCESSED;
		}
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
		if (global->m_editorMode == EditorMode::BLOCK_MODE && bdata.state == BlockToolState::READY_TO_EDIT)
		{
			Texture& texture = GlobalData::applyingTexture;

			auto block = bdata.blockInstance;
			auto brush = new Brush(*block->getVertices(), texture);
			m_scene->addObject(brush);

			m_scene->m_gui2DObjects.removeOne(block);
			m_scene->m_gui3DObjects.removeOne(block);
			delete bdata.blockInstance;
			bdata.blockInstance = nullptr;
			bdata.state = BlockToolState::CREATING;

			Actions::BrushCreatingData* data = new Actions::BrushCreatingData{ brush };
			ActionHistoryTool::addAction(Actions::brushcreating_undo, Actions::brushcreating_redo, Actions::brushcreating_cleanup, data);
		}

		if (m_glWidget2D_X->m_inputData.keyReturn == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_X->m_inputData.keyReturn = ButtonDownState::DOWN_NOT_PROCESSED;
		}
		if (m_glWidget2D_Y->m_inputData.keyReturn == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_Y->m_inputData.keyReturn = ButtonDownState::DOWN_NOT_PROCESSED;
		}
		if (m_glWidget2D_Z->m_inputData.keyReturn == ButtonDownState::RELEASED_PROCESSED)
		{
			m_glWidget2D_Z->m_inputData.keyReturn = ButtonDownState::DOWN_NOT_PROCESSED;
		}
	}

	processShortcuts();
	endInputProcessing(false);

	QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat())
		return;

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
	if (key == Qt::Key_V)
	{
		if (m_glWidget3D->m_inputData.keyV == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyV = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_F)
	{
		if (m_glWidget3D->m_inputData.keyF == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyF = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_R)
	{
		if (m_glWidget3D->m_inputData.keyR == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyR = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Control)
	{
		if (m_inputData.keyCtrl == ButtonDownState::DOWN_PROCESSED)
		{
			m_inputData.keyCtrl = ButtonDownState::RELEASED_NOT_PROCESSED;
		}

		if (m_glWidget3D->m_inputData.keyCtrl == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget3D->m_inputData.keyCtrl = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Z)
	{
		if (m_inputData.keyZ == ButtonDownState::DOWN_PROCESSED)
		{
			m_inputData.keyZ = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}
	if (key == Qt::Key_Shift)
	{
		if (m_inputData.keyShift == ButtonDownState::DOWN_PROCESSED)
		{
			m_inputData.keyShift = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
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
	if (key == Qt::Key_Return)
	{
		if (m_glWidget2D_X->m_inputData.keyReturn == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_X->m_inputData.keyReturn = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
		if (m_glWidget2D_Y->m_inputData.keyReturn == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_Y->m_inputData.keyReturn = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
		if (m_glWidget2D_Z->m_inputData.keyReturn == ButtonDownState::DOWN_PROCESSED)
		{
			m_glWidget2D_Z->m_inputData.keyReturn = ButtonDownState::RELEASED_NOT_PROCESSED;
		}
	}

	processShortcuts();
	endInputProcessing(true);

	QMainWindow::keyReleaseEvent(event);
}

void MainWindow::handleToolChange(QAction* action)
{
	if (action == m_selectionToolButton)
	{
		GlobalData::setMode(EditorMode::SELECTION_MODE);
	}
	else if (action == m_cameraToolButton)
	{
		GlobalData::setMode(EditorMode::CAMERA_MODE);
	}
	else if (action == m_blockToolButton)
	{
		GlobalData::setMode(EditorMode::BLOCK_MODE);
	}
	else if (action == m_clippingToolButton)
	{
		GlobalData::setMode(EditorMode::CLIPPING_MODE);
	}

	else if (action == m_textureToolButton)
	{
		GlobalData::setMode(EditorMode::TEXTURE_TOOL);
	}
}

void MainWindow::handleHistoryChange(QAction* action)
{
	if (action == m_historyBackButton)
	{
		ActionHistoryTool::undo();
	}
	else if (action == m_historyForthButton)
	{
		ActionHistoryTool::redo();
	}
}

void MainWindow::processShortcuts()
{
	auto& d = m_inputData;

	/* Ctrl + Shift + Z shortcut */
	if ((d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_PROCESSED && d.keyShift == ButtonDownState::DOWN_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED && d.keyShift == ButtonDownState::DOWN_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED && d.keyShift == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_PROCESSED && d.keyShift == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED && d.keyShift == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_PROCESSED && d.keyZ == ButtonDownState::DOWN_PROCESSED && d.keyShift == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED && d.keyShift == ButtonDownState::DOWN_PROCESSED))
	{
		ActionHistoryTool::redo();
	}
	/* Ctrl + Z shortcut */
	else if ((d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_PROCESSED && d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED) ||
		(d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED && d.keyZ == ButtonDownState::DOWN_PROCESSED))
	{
		ActionHistoryTool::undo();
	}
}

void MainWindow::endInputProcessing(bool isReleased)
{
	auto& d = m_inputData;

	if (isReleased)
	{
		if (d.keyCtrl == ButtonDownState::RELEASED_NOT_PROCESSED)
			d.keyCtrl = ButtonDownState::RELEASED_PROCESSED;

		if (d.keyZ == ButtonDownState::RELEASED_NOT_PROCESSED)
			d.keyZ = ButtonDownState::RELEASED_PROCESSED;

		if (d.keyShift == ButtonDownState::RELEASED_NOT_PROCESSED)
			d.keyShift = ButtonDownState::RELEASED_PROCESSED;
	}
	else
	{
		if (d.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED)
			d.keyCtrl = ButtonDownState::DOWN_PROCESSED;

		if (d.keyZ == ButtonDownState::DOWN_NOT_PROCESSED)
			d.keyZ = ButtonDownState::DOWN_PROCESSED;

		if (d.keyShift == ButtonDownState::DOWN_NOT_PROCESSED)
			d.keyShift = ButtonDownState::DOWN_PROCESSED;
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	bool agreed = true;

	if (GlobalData::isStateTouched)
	{
		auto btn = QMessageBox::question(this, "Close map", "All unsaved changes will be lost. Are you sure?");
		agreed = btn != QMessageBox::StandardButton::No;
	}

	if (!agreed)
	{
		event->ignore();
		return;
	}

	if (m_textureToolDialog)
	{
		m_textureToolDialog->close();
	}

	QMainWindow::closeEvent(event);
}

void MainWindow::handleSaveAsButtonClicked(bool checked)
{
	QString filename = QFileDialog::getSaveFileName(this,
		"Save map", QDir::currentPath(),
		"F3D map (*.map);");

	if (filename.isEmpty())
		return;

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, "Unable to open file", file.errorString());
		return;
	}

	QTextStream outStream(&file);
	json output;
	auto& brushes = GlobalData::getInstance()->m_scene->getObjects();

	for (auto* brush : brushes)
	{
		if (!brush->isOnScene)
			continue;

		QDir cwd = QDir::currentPath();
		auto& defaultTexture = brush->getDefaultTexture();
		QString defaultTexturePath = cwd.relativeFilePath(defaultTexture.path);

		json b = {
			{"type", "brush"},
			{"origin", {
				{"x", brush->m_origin.x()},
				{"y", brush->m_origin.y()},
				{"z", brush->m_origin.z()}
			}},
			{"color", {
				{"r", brush->getColor().x()},
				{"g", brush->getColor().y()},
				{"b", brush->getColor().z()}
			}},
			{"default_texture_path", defaultTexturePath.toStdString()},
			{"is_default_texture_missing", defaultTexture.isMissing}
		};

		json uniqueVertices;
		std::unordered_map<QVector3D*, int> indexMap;
		int i = 0;
		for (auto* v : brush->getUniqueVertices())
		{
			uniqueVertices.push_back({
				{"x", v->x()},
				{"y", v->y()},
				{"z", v->z()},
				});

			indexMap[v] = i++;
		}
		b["unique_vertices"] = uniqueVertices;

		json uniqueEdges;
		for (auto& edge : brush->getUniqueEdges())
		{
			uniqueEdges.push_back({
				{"v0", indexMap[edge.v0]},
				{"v1", indexMap[edge.v1]},
				});
		}
		b["unique_edges"] = uniqueEdges;

		json polygons;
		for (auto* polygon : brush->getPolygons())
		{
			auto* texture = ResourceManager::getTextureById(polygon->textureId);
			QString texturePath = cwd.relativeFilePath(texture->path);

			json p = {
				{"norm", {
					{"x", polygon->norm.x()},
					{"y", polygon->norm.y()},
					{"z", polygon->norm.z()}
				}},
				{"shift", {
					{"x", polygon->shift.x()},
					{"y", polygon->shift.y()}
				}},
				{"scale", {
					{"x", polygon->scale.x()},
					{"y", polygon->scale.y()}
				}},
				{"texture_path", texturePath.toStdString()},
				{"is_texture_missing", texture->isMissing}
			};

			json vertices;
			for (auto* v : polygon->vertices)
			{
				vertices.push_back(indexMap[v]);
			}
			p["vertices"] = vertices;

			json edges;
			for (auto& edge : polygon->edges)
			{
				edges.push_back({
					{"v0", indexMap[edge.v0]},
					{"v1", indexMap[edge.v1]}
					});
			}
			p["edges"] = edges;

			json trianglesLines;
			for (auto& edge : polygon->trianglesLines)
			{
				trianglesLines.push_back({
					{"v0", indexMap[edge.v0]},
					{"v1", indexMap[edge.v1]}
					});
			}
			p["trianglesLines"] = trianglesLines;

			json texCoords;
			for (auto& [vertex, vec] : polygon->verticesMap)
			{
				texCoords.push_back({
					{"vertex_index", indexMap[vertex]},
					{"u", vec.x()},
					{"v", vec.y()}
				});
			}
			p["tex_coords"] = texCoords;

			json triangles;
			for (auto& triangle : polygon->triangles)
			{
				triangles.push_back({
					{"v0",  indexMap[triangle.v0]},
					{"v1",  indexMap[triangle.v1]},
					{"v2",  indexMap[triangle.v2]},
					});
			}
			p["triangles"] = triangles;

			polygons.push_back(p);
		}
		b["polygons"] = polygons;

		output["objects"].push_back(b);
	}

	outStream << output.dump().c_str();
	GlobalData::isStateTouched = false;
}

void MainWindow::handleOpenFileButtonClicked(bool checked)
{
	bool agreed = true;

	if (GlobalData::isStateTouched)
	{
		auto btn = QMessageBox::question(this, "Close map", "All unsaved changes will be lost. Are you sure?");
		agreed = btn != QMessageBox::StandardButton::NoButton;
	}

	if (!agreed)
	{
		return;
	}

	QString filename = QFileDialog::getOpenFileName(this,
		"Open map", QDir::currentPath(),
		"F3D maps (*.map);");

	if (filename.isEmpty())
		return;

	if (!isEditorOpened)
	{
		openEditor();
	}

	GlobalData::clearScene();

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
		QMessageBox::information(this, "Unable to open file", file.errorString());
		return;
	}

	QTextStream inStream(&file);
	QString input = inStream.readAll();
	auto mapData = json::parse(input.toStdString()).get<Types::MapJSON>();
	auto global = GlobalData::getInstance();

	for (auto& object : mapData.objects)
	{
		if (object.type == "brush")
		{
			global->m_scene->addObject(new Brush(object));
		}
	}

	GlobalData::isStateTouched = false;
}

void MainWindow::showGLWidgets()
{
	m_glWidget3D->show();
	m_glWidget2D_X->show();
	m_glWidget2D_Y->show();
	m_glWidget2D_Z->show();
}

void MainWindow::hideGLWidgets()
{
	m_glWidget3D->hide();
	m_glWidget2D_X->hide();
	m_glWidget2D_Y->hide();
	m_glWidget2D_Z->hide();
}

void MainWindow::openEditor()
{
	showGLWidgets();
	enableTools();
	m_glWidgetsContainer->doResize();
	isEditorOpened = true;
}

void MainWindow::closeEditor()
{
	hideGLWidgets();
	disableTools();
	isEditorOpened = false;
}

void MainWindow::enableTools()
{
	m_selectionToolButton->setEnabled(true);
	m_cameraToolButton->setEnabled(true);
	m_blockToolButton->setEnabled(true);
	m_clippingToolButton->setEnabled(true);
	m_textureToolButton->setEnabled(true);
}

void MainWindow::disableTools()
{
	m_selectionToolButton->setDisabled(true);
	m_cameraToolButton->setDisabled(true);
	m_blockToolButton->setDisabled(true);
	m_clippingToolButton->setDisabled(true);
	m_textureToolButton->setDisabled(true);
}

void MainWindow::handleNewButtonClicked(bool checked)
{
	if (!isEditorOpened)
	{
		openEditor();
		return;
	}

	if (!GlobalData::isStateTouched)
	{
		GlobalData::clearScene();
		return;
	}

	auto btn = QMessageBox::question(this, "New map", "All unsaved changes will be lost. Are you sure?");
	bool agreed = btn != QMessageBox::StandardButton::NoButton;

	if (agreed)
	{
		GlobalData::clearScene();
	}
}

void MainWindow::handleCloseButtonClicked(bool checked)
{
	bool agreed = true;

	if (GlobalData::isStateTouched)
	{
		auto btn = QMessageBox::question(this, "Close map", "All unsaved changes will be lost. Are you sure?");
		agreed = btn != QMessageBox::StandardButton::No;
	}

	if (!agreed)
	{
		return;
	}

	GlobalData::clearScene();
	closeEditor();
}

void MainWindow::handleExitButtonClicked(bool checked)
{
	close();
}
