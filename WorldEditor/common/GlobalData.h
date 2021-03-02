#pragma once

#include "types.h"
#include "../editor/ConstructionBlock.h"
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <unordered_map>
#include "../editor/Renderable.h"
#include <QOpenGLVertexArrayObject>
#include "../editor/Scene.h"

class GlobalData
{
public:
	typedef std::unordered_map<Renderable*, QOpenGLVertexArrayObject*> ContextVAOMap;

	static void init();
	static inline GlobalData* getInstance() { return m_instance; }
	static QOpenGLVertexArrayObject* getRenderableVAO(QOpenGLContext& context, Renderable& renderable);
	static void setMode(EditorMode mode);
	
	struct {
		BlockToolState state = BlockToolState::CREATING;
		ConstructionBlock* blockInstance = nullptr;
		QVector3D startPoint, endPoint;
		bool shouldCreate = false;
	} m_blockToolData;

	struct {
		Types::SelectionToolState state = Types::SelectionToolState::READY_TO_SELECT;
		Brush* renderable = nullptr;
	} m_selectionToolData;

	static std::unordered_map<QOpenGLContext*, ContextVAOMap*> openglContexts;
	EditorMode m_editorMode;
	Scene* m_scene;

private:
	GlobalData();
	~GlobalData();
	static void onModeEnable(EditorMode mode);
	static void onModeDisable(EditorMode mode);

	static GlobalData* m_instance;
};
