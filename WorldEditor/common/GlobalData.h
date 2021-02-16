#pragma once

#include "types.h"
#include "../editor/ConstructionBlock.h"

class GlobalData
{
public:
	static void init();
	static inline GlobalData* getInstance() { return m_instance; }

	EdtitorMode m_editorMode = EdtitorMode::BLOCK_MODE;
	struct {
		BlockToolState state = BlockToolState::CREATING;
		ConstructionBlock* blockInstance = nullptr;
	} m_blockToolData;

private:
	GlobalData();
	~GlobalData();

	static GlobalData* m_instance;
};
