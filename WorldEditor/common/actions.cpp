#include "actions.h"

void Actions::brushcreating_undo(void* rawData)
{
	auto* global = GlobalData::getInstance();
	auto& sdata = global->m_selectionToolData;
	auto* data = static_cast<BrushCreatingData*>(rawData);

	global->m_scene->removeObject(data->brush);

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
}

void Actions::brushcreating_cleanup(void* rawData)
{
	auto* data = static_cast<BrushCreatingData*>(rawData);
	delete data->brush;
}
