#include "ActionHistoryTool.h"
#include <QDebug>
#include "../gui/MainWindow.h"

ActionHistoryTool* ActionHistoryTool::m_instance = nullptr;

ActionHistoryTool::ActionHistoryTool()
{
	Action* action = new Action{ true };
	m_actions.push_back(action);
	m_cur = m_actions.begin();
	m_end = m_cur;
}

ActionHistoryTool::~ActionHistoryTool()
{
	/* DELETE LIST */
}

void ActionHistoryTool::init()
{
	if (m_instance)
	{
		qInfo() << "'ActionsHistoryTool' has already been initialized";
	}

	m_instance = new ActionHistoryTool;
}

void ActionHistoryTool::cleanup()
{
	delete m_instance;
}

void ActionHistoryTool::undo()
{
	auto cur = *m_instance->m_cur;

	if (cur->isRoot)
	{
		return;
	}

	cur->undo(cur->data);
	m_instance->m_cur--;

	if (isRoot())
	{
		MainWindow::getHistoryBackButton()->setDisabled(true);
	}

	MainWindow::getHistoryForthButton()->setEnabled(true);
}

void ActionHistoryTool::redo()
{
	if (m_instance->m_cur == m_instance->m_end)
	{
		return;
	}

	m_instance->m_cur++;
	(*m_instance->m_cur)->redo((*m_instance->m_cur)->data);

	if (isTerminal())
	{
		MainWindow::getHistoryForthButton()->setDisabled(true);
	}

	MainWindow::getHistoryBackButton()->setEnabled(true);
}

void ActionHistoryTool::addAction(undo_t undo, redo_t redo, cleanup_t cleanup, void* data)
{
	for (auto it = m_instance->m_end; it != m_instance->m_cur;)
	{
		it--;
		delete m_instance->m_actions.takeLast();
	}

	auto s = *m_instance->m_cur;

	Action* action = new Action{ false, undo, redo, cleanup, data };
	m_instance->m_actions.push_back(action);
	m_instance->m_cur++;
	m_instance->m_end = m_instance->m_cur;

	MainWindow::getHistoryBackButton()->setEnabled(true);
	MainWindow::getHistoryForthButton()->setDisabled(true);
}

bool ActionHistoryTool::isRoot()
{
	return *m_instance->m_cur == *m_instance->m_actions.begin();
}

bool ActionHistoryTool::isTerminal()
{
	return *m_instance->m_cur == *m_instance->m_end;
}
