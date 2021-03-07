#pragma once

#include "../editor/Brush.h"
#include "GlobalData.h"

namespace Actions
{
	typedef Types::undo_t undo_t;
	typedef Types::redo_t redo_t;
	typedef Types::cleanup_t cleanup_t;

	struct BrushCreatingData
	{
		Brush* brush;
	};

	void brushcreating_undo(void* data);
	void brushcreating_redo(void* data);
	void brushcreating_cleanup(void* data);
}
