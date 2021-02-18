#include "GlobalData.h"
#include <QDebug>

GlobalData* GlobalData::m_instance = nullptr;
//QOpenGLWidget* GlobalData::widget3D = nullptr;
//QOpenGLWidget* GlobalData::widget2D = nullptr;
//QOpenGLContext* GlobalData::dummyContext = nullptr;
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
