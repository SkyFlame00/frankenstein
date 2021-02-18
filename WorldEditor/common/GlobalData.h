#pragma once

#include "types.h"
#include "../editor/ConstructionBlock.h"
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <unordered_map>
#include "../editor/Renderable.h"
#include <QOpenGLVertexArrayObject>

class GlobalData
{
public:
	typedef std::unordered_map<Renderable*, QOpenGLVertexArrayObject*> ContextVAOMap;

	static void init();
	static inline GlobalData* getInstance() { return m_instance; }

	EdtitorMode m_editorMode = EdtitorMode::BLOCK_MODE;
	struct {
		BlockToolState state = BlockToolState::CREATING;
		ConstructionBlock* blockInstance = nullptr;
		QVector3D startPoint, endPoint;
		bool shouldCreate = false;
	} m_blockToolData;

	//static QOpenGLWidget* widget3D;
	//static QOpenGLWidget* widget2D;
	//static QOpenGLContext* dummyContext;
	static std::unordered_map<QOpenGLContext*, ContextVAOMap*> openglContexts;

private:
	GlobalData();
	~GlobalData();

	static GlobalData* m_instance;
};
