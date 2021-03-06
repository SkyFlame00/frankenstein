#include "GlobalData.h"
#include <QDebug>

GlobalData* GlobalData::m_instance = nullptr;
std::unordered_map<QOpenGLContext*, GlobalData::ContextVAOMap*> GlobalData::openglContexts;
const int GlobalData::CONTEXTS_NUM = 4;
int GlobalData::contextsReady = 0;

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

void GlobalData::setMode(EditorMode mode)
{
	auto* inst = getInstance();
	onModeChange(mode);
	inst->m_editorMode = mode;
}

void GlobalData::onModeChange(EditorMode nextMode)
{
	auto* inst = getInstance();
	auto prevMode = inst->m_editorMode;

	switch (nextMode)
	{
	case EditorMode::SELECTION_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.push_back(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.push_back(inst->m_blockToolData.blockInstance);
		}
		break;
	case EditorMode::CLIPPING_MODE:
		auto* brush = inst->m_selectionToolData.renderable;
		if (brush)
			brush->m_isInClippingMode = true;
		break;
	}

	switch (prevMode)
	{
	case EditorMode::SELECTION_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.removeOne(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.removeOne(inst->m_blockToolData.blockInstance);
		}
		break;
	case EditorMode::CLIPPING_MODE:
		auto* brush = inst->m_selectionToolData.renderable;
		if (brush)
			brush->m_isInClippingMode = false;
		inst->m_clippingToolData.glWidget->discardClipping();
		break;
	}
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
	}
}
