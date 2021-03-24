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
	auto* global = GlobalData::getInstance();
	auto* data = static_cast<BrushCreatingData*>(rawData);
	
	if (global->m_scene->contains(data->brush))
	{
		global->m_scene->removeObject(data->brush);
		delete data->brush;
	}

	delete data;
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

/* Texture rotation */

void Actions::texturerotation_undo(void* rawData)
{
	auto* list = static_cast<TextureRotationData*>(rawData);

	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->rotationAngle = st.oldRotation;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::texturerotation_redo(void* rawData)
{
	auto* list = static_cast<TextureRotationData*>(rawData);

	for (auto& pair : *list)
	{
		auto* polygon = pair.first;
		auto& st = pair.second;
		polygon->rotationAngle = st.newRotation;
		st.brush->sendPolygonDataToGPU(polygon);
	}
}

void Actions::texturerotation_cleanup(void* rawData)
{
	auto* data = static_cast<TextureRotationData*>(rawData);
	delete data;
}

/* Texture picking */

void Actions::texturepicking_undo(void* rawData)
{
	auto* list = static_cast<TexturePickingData*>(rawData);

	for (auto& [brush, polygon, oldTexture, _] : *list)
	{
		brush->updatePolygonTexture(polygon, oldTexture);
	}
}

void Actions::texturepicking_redo(void* rawData)
{
	auto* list = static_cast<TexturePickingData*>(rawData);

	for (auto& [brush, polygon, _, newTexture] : *list)
	{
		brush->updatePolygonTexture(polygon, newTexture);
	}
}

void Actions::texturepicking_cleanup(void* rawData)
{
	auto* list = static_cast<TexturePickingData*>(rawData);
	delete list;
}

void Actions::brushdeleting_undo(void* rawData)
{
	auto* data = static_cast<BrushDeletingData*>(rawData);
	auto* global = GlobalData::getInstance();

	global->m_scene->addObject(data->brush);
}

void Actions::brushdeleting_redo(void* rawData)
{
	auto* data = static_cast<BrushDeletingData*>(rawData);
	auto* global = GlobalData::getInstance();

	global->m_scene->removeObject(data->brush);
}

void Actions::brushdeleting_cleanup(void* rawData)
{
	auto* data = static_cast<BrushDeletingData*>(rawData);
	delete data;
}
