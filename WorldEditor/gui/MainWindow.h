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
#include "TextureToolDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	static void init();
	static void cleanup();
	inline static MainWindow* getInstance() { return m_instance; }
	inline static QAction* getHistoryBackButton() { return m_instance->m_historyBackButton; }
	inline static QAction* getHistoryForthButton() { return m_instance->m_historyForthButton; }
	inline static TextureToolDialog* getTextureToolDialog() { return m_instance->m_textureToolDialog; }
	inline static QAction* getSelectionToolButton() { return m_instance->m_selectionToolButton; }
	inline static QAction* getCameraToolButton() { return m_instance->m_cameraToolButton; }
	inline static QAction* getBlockToolButton() { return m_instance->m_blockToolButton; }
	inline static QAction* getClippingToolButton() { return m_instance->m_clippingToolButton; }
	inline static QAction* getTextureToolButton() { return m_instance->m_textureToolButton; }
	
	QMenuBar* menuBar;
	QMenu* fileMenu;
	
	struct
	{
		ButtonDownState keyCtrl = ButtonDownState::RELEASED_PROCESSED;
		ButtonDownState keyZ = ButtonDownState::RELEASED_PROCESSED;
		ButtonDownState keyShift = ButtonDownState::RELEASED_PROCESSED;
		ButtonDownState keyQ = ButtonDownState::RELEASED_PROCESSED;
	} m_inputData;

private:
	void setupMenu();
	void setupTopToolbar();
	void setupLeftToolbar();
	void setupDocks();
	void setupEditor();
	void setupWindows();
	void enableMouseTracking();
	void handleToolChange(QAction* action);
	void handleHistoryChange(QAction* action);
	void processShortcuts();
	void endInputProcessing(bool isReleased);
	void closeEvent(QCloseEvent* event);
	void handleSaveButtonClicked(bool checked);
	void handleSaveAsButtonClicked(bool checked);
	void handleOpenFileButtonClicked(bool checked);
	void handleNewButtonClicked(bool checked);
	void handleCloseButtonClicked(bool checked);
	void handleExitButtonClicked(bool checked);
	void openEditor();
	void closeEditor();
	void showGLWidgets();
	void hideGLWidgets();
	void enableTools();
	void disableTools();
	void saveMap(const QString& filename);

	static MainWindow* m_instance;

	QWidget* m_centralWidget;
	GLWidget3D* m_glWidget3D;
	GLWidget2D* m_glWidget2D_X, *m_glWidget2D_Y, *m_glWidget2D_Z;
	Camera* m_camera3D;
	Camera* m_camera2D_X, *m_camera2D_Y, *m_camera2D_Z;
	Scene* m_scene;
	Renderer3D* m_renderer3D;
	Renderer2D* m_renderer2D_X, *m_renderer2D_Y, *m_renderer2D_Z;
	GLWidgetsContainer* m_glWidgetsContainer;
	bool isEditorOpened = false;
	bool m_isFilepathDefined = false;
	QString m_filepath;

	/* Menu */
	QAction* m_newBtn;
	QAction* m_openFileBtn;
	QAction* m_saveBtn;
	QAction* m_saveAsBtn;
	QAction* m_closeBtn;
	QAction* m_exitBtn;

	/* Top toolbar */
	QToolBar* m_topToolbar;
	QActionGroup* m_historyGroup;
	QAction* m_historyBackButton;
	QAction* m_historyForthButton;

	/* Left toolbar */
	QToolBar* m_leftToolbar;
	QActionGroup* m_leftToolbarGroup;
	QAction* m_selectionToolButton;
	QAction* m_cameraToolButton;
	QAction* m_blockToolButton;
	QAction* m_clippingToolButton;
	QAction* m_textureToolButton;

	TextureToolDialog* m_textureToolDialog = nullptr;
};
