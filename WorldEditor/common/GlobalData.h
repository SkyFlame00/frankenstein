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
#include "../editor/grid2d/Point.h"
#include "../editor/grid2d/Line.h"
#include "../gui/GLWidget2D/GLWidget2D.h"

class GlobalData
{
public:
	typedef std::unordered_map<Renderable*, QOpenGLVertexArrayObject*> ContextVAOMap;

	static const int CONTEXTS_NUM;
	static int contextsReady;
	static QString configPath;

	static void init();
	static void cleanup();
	static inline GlobalData* getInstance() { return m_instance; }
	static QOpenGLVertexArrayObject* getRenderableVAO(QOpenGLContext& context, Renderable& renderable);
	static void setMode(EditorMode mode);
	static void onContextReady();
	static void clearScene();
	static void loadConfiguration();
	static void showBrushMetrics();
	static void hideBrushMetrics();
	static void updateBrushMetrics(int width, int height, int length);
	
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

	struct {
		Axis axis = Axis::NONE;
		Types::ClippingToolState state = Types::ClippingToolState::READY;
		Point* point1;
		Point* point2;
		Point* d_pt; /* Displacement point */
		Point* nd_pt; /* Non-displacement point */
		Line* line = nullptr;
		bool pt1_placed = false;
		bool pt2_placed = false;

		struct {
			float a, b, c, d;
			QVector3D p0;
			QVector3D norm;
			bool normReversed = false;
		} plane;

		GLWidget2D* glWidget = nullptr;
	} m_clippingToolData;

	struct {
		std::unordered_map<Types::Polygon*, Brush*> pickedPolygons;
	} textureToolData;

	static std::unordered_map<QOpenGLContext*, ContextVAOMap*> openglContexts;
	EditorMode m_editorMode = EditorMode::SELECTION_MODE;
	Scene* m_scene;
	bool m_isDrawingLines = false;
	bool m_isWireframeMode = false;
	int m_maxTextureUnits = 16;
	bool isSelectionForTextureToolActivated = false;
	bool isCameraForTextureToolActivated = false;
	static QString texturesPath;
	static Texture applyingTexture;
	static bool isStateTouched;
	static bool displayNormals;
	static Types::DrawMode drawMode;

private:
	GlobalData();
	~GlobalData();
	static bool onModeChange(EditorMode nextMode);
	static QAction* getToolButtonByMode(EditorMode mode);

	static GlobalData* m_instance;
};
