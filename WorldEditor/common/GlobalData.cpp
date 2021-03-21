#include "GlobalData.h"
#include <QDebug>
#include "../gui/MainWindow.h"
#include "../editor/ResourceManager.h"
#include <QDir>

GlobalData* GlobalData::m_instance = nullptr;
std::unordered_map<QOpenGLContext*, GlobalData::ContextVAOMap*> GlobalData::openglContexts;
const int GlobalData::CONTEXTS_NUM = 4;
int GlobalData::contextsReady = 0;
QString GlobalData::texturesPath = "resources/textures/";
Texture GlobalData::applyingTexture;

GlobalData::GlobalData()
{
}

GlobalData::~GlobalData()
{
}

void GlobalData::init()
{
	if (m_instance)
	{
		qWarning() << "GlobalData instance already created";
		return;
	}

	m_instance = new GlobalData;
}

void GlobalData::cleanup()
{
	delete getInstance();
}

QOpenGLVertexArrayObject* GlobalData::getRenderableVAO(QOpenGLContext& context, Renderable& renderable)
{
	auto vaoMapIt = openglContexts.find(&context);

	if (vaoMapIt == openglContexts.end())
	{
		qInfo() << "ConstructionBlock::render2D: Corresponding VAO map was not found";
		return nullptr;
	}

	auto vaoMap = vaoMapIt->second;
	auto vaoIt = vaoMap->find(&renderable);

	if (vaoIt == vaoMap->end())
	{
		qInfo() << "ConstructionBlock::render2D: Corresponding VAO was not found";
		return nullptr;
	}

	return vaoIt->second;
}

void GlobalData::setMode(EditorMode nextMode)
{
	auto* inst = getInstance();
	bool hasChanged = onModeChange(nextMode);

	if (hasChanged)
		inst->m_editorMode = nextMode;
}

bool GlobalData::onModeChange(EditorMode nextMode)
{
	auto* inst = getInstance();
	auto prevMode = inst->m_editorMode;
	auto* mainWindow = MainWindow::getInstance();
	auto* prevTool = getToolButtonByMode(prevMode);
	auto* nextTool = getToolButtonByMode(nextMode);
	auto* selButton = mainWindow->getSelectionToolButton();
	auto* camButton = mainWindow->getCameraToolButton();
	auto* texButton = mainWindow->getTextureToolButton();
	bool activateNextMode = true;
	
	prevTool->setChecked(false);
	nextTool->setChecked(true);

	if (prevMode == nextMode)
		return false;

	switch (prevMode)
	{
	case EditorMode::SELECTION_MODE:
		break;
	case EditorMode::CAMERA_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.removeOne(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.removeOne(inst->m_blockToolData.blockInstance);
		}
		break;
	case EditorMode::CLIPPING_MODE:
	{
		auto* brush = inst->m_selectionToolData.renderable;
		if (brush)
			brush->m_isInClippingMode = false;
		inst->m_clippingToolData.glWidget->discardClipping();
		break;
	}
	case EditorMode::TEXTURE_TOOL:
		if (nextMode == EditorMode::SELECTION_MODE || nextMode == EditorMode::CAMERA_MODE)
		{
			activateNextMode = false;

			if (nextMode == EditorMode::SELECTION_MODE)
			{
				selButton->setChecked(true);
				camButton->setChecked(false);
				texButton->setChecked(true);
				inst->isSelectionForTextureToolActivated = true;
				inst->isCameraForTextureToolActivated = false;
			}	
			else if (nextMode == EditorMode::CAMERA_MODE)
			{
				selButton->setChecked(false);
				camButton->setChecked(true);
				texButton->setChecked(true);
				inst->isSelectionForTextureToolActivated = false;
				inst->isCameraForTextureToolActivated = true;
			}
		}
		else
		{
			auto* dialog = mainWindow->getTextureToolDialog();
			dialog->hide();
			selButton->setChecked(false);
			camButton->setChecked(false);
			inst->isSelectionForTextureToolActivated = false;
			inst->isCameraForTextureToolActivated = false;
		}
		break;
	}

	if (!activateNextMode)
	{
		return false;
	}

	switch (nextMode)
	{
	case EditorMode::SELECTION_MODE:
		break;
	case EditorMode::CAMERA_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.push_back(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.push_back(inst->m_blockToolData.blockInstance);
		}
		break;
	case EditorMode::CLIPPING_MODE:
	{
		auto* brush = inst->m_selectionToolData.renderable;
		if (brush)
			brush->m_isInClippingMode = true;
		break;
	}
	case EditorMode::TEXTURE_TOOL:
	{
		auto* dialog = mainWindow->getTextureToolDialog();
		dialog->show();
		selButton->setChecked(true);
		inst->isSelectionForTextureToolActivated = true;
		break;
	}
	}

	return true;
}

void GlobalData::onContextReady()
{
	if (contextsReady >= CONTEXTS_NUM)
	{
		qWarning() << "Exceeded max number of available OpenGL contexts";
		return;
	}

	contextsReady++;

	if (contextsReady == CONTEXTS_NUM)
	{
		m_instance->m_clippingToolData.point1 = new Point(12.0f, 0.0f, 0.0f, 0.0f);
		m_instance->m_clippingToolData.point2 = new Point(12.0f, 0.0f, 0.0f, 0.0f);
		m_instance->m_clippingToolData.point1->m_enableScale = false;
		m_instance->m_clippingToolData.point2->m_enableScale = false;
		m_instance->m_clippingToolData.line = new Line;

		applyingTexture = ResourceManager::getTexture(QDir::currentPath() + "/assets/missing_texture.png", true);

		auto* mainWindow = MainWindow::getInstance();
		mainWindow->getTextureToolDialog()->init();
	}
}

QAction* GlobalData::getToolButtonByMode(EditorMode mode)
{
	auto* mainWindow = MainWindow::getInstance();

	switch (mode)
	{
	case EditorMode::SELECTION_MODE:
		return mainWindow->getSelectionToolButton();
	case EditorMode::CAMERA_MODE:
		return mainWindow->getCameraToolButton();
	case EditorMode::BLOCK_MODE:
		return mainWindow->getBlockToolButton();
	case EditorMode::CLIPPING_MODE:
		return mainWindow->getClippingToolButton();
	case EditorMode::TEXTURE_TOOL:
		return mainWindow->getTextureToolButton();
	}
}
