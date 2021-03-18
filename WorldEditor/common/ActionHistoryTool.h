#pragma once

#include <QLinkedList >
#include "types.h"

class ActionHistoryTool
{
public:
	typedef Types::undo_t undo_t;
	typedef Types::redo_t redo_t;
	typedef Types::cleanup_t cleanup_t;

	static void init();
	static void cleanup();
	static void undo();
	static void redo();
	static void addAction(undo_t undo, redo_t redo, cleanup_t cleanup, void* data);
	static bool isRoot();
	static bool isTerminal();

private:
	struct Action
	{
		bool isRoot;
		undo_t undo;
		redo_t redo;
		cleanup_t cleanup;
		void* data;

		~Action() { if (!isRoot) cleanup(data); }
	};

	ActionHistoryTool();
	~ActionHistoryTool();
	static void adjustGlobalState();

	static ActionHistoryTool* m_instance;
	QLinkedList<Action*> m_actions;
	QLinkedList<Action*>::const_iterator m_cur;
	QLinkedList<Action*>::const_iterator m_end;
};
