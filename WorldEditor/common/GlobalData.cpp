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
