#include "GlobalData.h"
#include <QDebug>

GlobalData* GlobalData::m_instance = nullptr;
std::unordered_map<QOpenGLContext*, GlobalData::ContextVAOMap*> GlobalData::openglContexts;

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
	onModeDisable(inst->m_editorMode);
	onModeEnable(mode);
	inst->m_editorMode = mode;
}

void GlobalData::onModeEnable(EditorMode mode)
{
	auto* inst = getInstance();

	switch (mode)
	{
	case EditorMode::SELECTING_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.push_back(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.push_back(inst->m_blockToolData.blockInstance);
		}
		break;
	}
}

void GlobalData::onModeDisable(EditorMode mode)
{
	auto* inst = getInstance();

	switch (mode)
	{
	case EditorMode::SELECTING_MODE:
		break;
	case EditorMode::BLOCK_MODE:
		if (inst->m_blockToolData.blockInstance)
		{
			inst->m_scene->m_gui2DObjects.removeOne(inst->m_blockToolData.blockInstance);
			inst->m_scene->m_gui3DObjects.removeOne(inst->m_blockToolData.blockInstance);
		}
		break;
	}
}
