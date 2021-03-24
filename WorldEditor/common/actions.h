#pragma once

#include "../editor/Brush.h"
#include "GlobalData.h"
#include <unordered_map>
#include "../editor/Texture.h"

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

	/* Deleting brush */
	struct BrushDeletingData
	{
		Brush* brush;
	};

	void brushdeleting_undo(void* data);
	void brushdeleting_redo(void* data);
	void brushdeleting_cleanup(void* data);

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

	/* Texture tool dialog: scale actions */
	struct TextureScaleStruct
	{
		Brush* brush;
		QVector2D oldScale;
		QVector2D newScale;
	};
	typedef std::unordered_map<Types::Polygon*, TextureScaleStruct> TextureScaleData;

	void texturescale_undo(void* data);
	void texturescale_redo(void* data);
	void texturescale_cleanup(void* data);

	/* Texture tool dialog: rotation actions */
	struct TextureRotationStruct
	{
		Brush* brush;
		int oldRotation;
		int newRotation;
	};
	typedef std::unordered_map<Types::Polygon*, TextureRotationStruct> TextureRotationData;

	void texturerotation_undo(void* data);
	void texturerotation_redo(void* data);
	void texturerotation_cleanup(void* data);

	/* Texture picking */
	struct TexturePickingStruct
	{
		Brush* brush;
		Types::Polygon* polygon;
		Texture oldTexture;
		Texture newTexture;
	};
	typedef QList<TexturePickingStruct> TexturePickingData;

	void texturepicking_undo(void* data);
	void texturepicking_redo(void* data);
	void texturepicking_cleanup(void* data);
}
