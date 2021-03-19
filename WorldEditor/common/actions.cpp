#include "actions.h"

/* Brush creating */

void Actions::brushcreating_undo(void* rawData)
{
	auto* global = GlobalData::getInstance();
	auto& sdata = global->m_selectionToolData;
	auto* data = static_cast<BrushCreatingData*>(rawData);

	global->m_scene->removeObject(data->brush);
	data->brush->isOnScene = false;

	if (data->brush->m_selected && data->brush == sdata.renderable)
	{
		data->brush->m_selected = false;
		sdata.state = Types::SelectionToolState::READY_TO_SELECT;
		sdata.renderable = nullptr;
	}
}

void Actions::brushcreating_redo(void* rawData)
{
	auto* global = GlobalData::getInstance();
	auto* data = static_cast<BrushCreatingData*>(rawData);

	global->m_scene->addObject(data->brush);
	data->brush->isOnScene = true;
}

void Actions::brushcreating_cleanup(void* rawData)
{
	auto* data = static_cast<BrushCreatingData*>(rawData);
	delete data->brush;
}

/* Texture shift */

void Actions::textureshift_undo(void* rawData)
{
	auto* list = static_cast<TextureShiftData*>(rawData);

	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->shift = st.oldShift;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::textureshift_redo(void* rawData)
{
	auto* list = static_cast<TextureShiftData*>(rawData);
	
	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->shift = st.newShift;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::textureshift_cleanup(void* rawData)
{
	auto* data = static_cast<TextureShiftData*>(rawData);
	delete data;
}

/* Texture scale */

void Actions::texturescale_undo(void* rawData)
{
	auto* list = static_cast<TextureScaleData*>(rawData);

	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->scale = st.oldScale;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::texturescale_redo(void* rawData)
{
	auto* list = static_cast<TextureScaleData*>(rawData);

	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->scale = st.newScale;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::texturescale_cleanup(void* rawData)
{
	auto* data = static_cast<TextureScaleData*>(rawData);
	delete data;
}
