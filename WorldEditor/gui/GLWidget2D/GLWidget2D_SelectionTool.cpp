#include "GLWidget2D.h"
#include "../../common/GlobalData.h"
#include "../../common/helpers.h"

void GLWidget2D::processSelectionTool()
{
	auto globalData = GlobalData::getInstance();
	auto data = &globalData->m_selectionToolData;
	float x, y;
	float nearestX, nearestY;
	bool isWidgetActive = m_inputData.isMouseOver;
	getMouseCoordinates(m_inputData.mouseX, m_inputData.mouseY, &x, &y);
	getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);

	if (isWidgetActive && data->state == Types::SelectionToolState::READY_TO_SELECT)
	{
		if (!data->renderable)
		{
			return;
		}

		Qt::CursorShape cursor = data->renderable->checkHover(x, y, m_axis, getZoomFactor());
		setCursor(cursor);

		if (m_inputData.keyEscape == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			data->renderable->m_selected = false;
			data->renderable = nullptr;
			data->state = Types::SelectionToolState::READY_TO_SELECT;
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			Types::BrushAction _state = data->renderable->startDrag(m_axis, QVector2D(x, y), getZoomFactor());
			auto state = Helpers::mapToSelectionToolState(_state);
			data->state = state;

			if (state == Types::SelectionToolState::MOVE)
				setCursor(Qt::ClosedHandCursor);
		}
	}
	else if (data->state == Types::SelectionToolState::RESIZE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			data->renderable->doResizeStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			data->state = Types::SelectionToolState::READY_TO_SELECT;
			setCursor(Qt::ArrowCursor);
		}
	}
	else if (data->state == Types::SelectionToolState::MOVE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			data->renderable->doMoveStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			data->state = Types::SelectionToolState::READY_TO_SELECT;
			setCursor(Qt::ArrowCursor);
		}
	}
}