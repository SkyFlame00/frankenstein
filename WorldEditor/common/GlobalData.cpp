#include "GlobalData.h"
#include <QDebug>

GlobalData* GlobalData::m_instance = nullptr;

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
