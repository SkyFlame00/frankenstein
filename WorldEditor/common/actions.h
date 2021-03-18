#pragma once

#include "../editor/Brush.h"
#include "GlobalData.h"
#include <unordered_map>

namespace Actions
{
	typedef Types::undo_t undo_t;
	typedef Types::redo_t redo_t;
	typedef Types::cleanup_t cleanup_t;

	/* Brush creating actions */
	struct BrushCreatingData
	{
		Brush* brush;
	};

	void brushcreating_undo(void* data);
	void brushcreating_redo(void* data);
	void brushcreating_cleanup(void* data);

	/* Texture tool dialog: shift actions */
	struct TextureShiftStruct
	{
		Brush* brush;
		QVector2D oldShift;
		QVector2D newShift;
	};
	typedef std::unordered_map<Types::Polygon*, TextureShiftStruct> TextureShiftData;

	void textureshift_undo(void* data);
	void textureshift_redo(void* data);
	void textureshift_cleanup(void* data);
}
