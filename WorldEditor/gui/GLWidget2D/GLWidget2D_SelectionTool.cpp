#include "GLWidget2D.h"
#include "../../common/GlobalData.h"
#include "../../common/helpers.h"
#include "../../common/ActionHistoryTool.h"
#include "../../common/actions.h"

Actions::BrushMovingData* movingData;

float stepsX = 0.0f;
float stepsY = 0.0f;
ResizeDirection resizeDirection;

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
			auto* brush = data->renderable;

			if (state == Types::SelectionToolState::MOVE)
			{
				movingData = new Actions::BrushMovingData;
				movingData->brush = brush;
				auto bbox = brush->getBoundingBox();
				movingData->prevMove.origin = brush->m_origin;
				movingData->prevMove.bbox.startX = bbox.startX;
				movingData->prevMove.bbox.endX = bbox.endX;
				movingData->prevMove.bbox.startY = bbox.startY;
				movingData->prevMove.bbox.endY = bbox.endY;
				movingData->prevMove.bbox.startZ = bbox.startZ;
				movingData->prevMove.bbox.endZ = bbox.endZ;

				setCursor(Qt::ClosedHandCursor);
			}
			else if (state == Types::SelectionToolState::RESIZE)
			{
				resizeDirection = data->renderable->getResizeDirection();
			}
		}
	}
	else if (data->state == Types::SelectionToolState::RESIZE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			float deltaStepsX = 0.0f;
			float deltaStepsY = 0.0f;
			data->renderable->doResizeStep(m_axis, QVector2D(x, y), step, &deltaStepsX, &deltaStepsY);

			stepsX += deltaStepsX;
			stepsY += deltaStepsY;
			GlobalData::showBrushMetrics();
			GlobalData::updateBrushMetrics(data->renderable->getWidth(), data->renderable->getHeight(),
				data->renderable->getLength());
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			Actions::BrushResizingData* resizingData = new Actions::BrushResizingData;
			resizingData->brush = data->renderable;
			resizingData->axis = m_axis;
			resizingData->resizeDirection = resizeDirection;
			resizingData->stepsX = stepsX;
			resizingData->stepsY = stepsY;

			ActionHistoryTool::addAction(Actions::brushresizing_undo, Actions::brushresizing_redo,
				Actions::brushresizing_cleanup, resizingData);

			stepsX = 0.0f;
			stepsY = 0.0f;

			data->state = Types::SelectionToolState::READY_TO_SELECT;
			setCursor(Qt::ArrowCursor);
		}
	}
	else if (data->state == Types::SelectionToolState::MOVE)
	{
		auto* brush = data->renderable;

		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			data->renderable->doMoveStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			data->state = Types::SelectionToolState::READY_TO_SELECT;
			setCursor(Qt::ArrowCursor);

			const auto& bbox = brush->getBoundingBox();
			movingData->nextMove.origin = brush->m_origin;
			movingData->nextMove.bbox.startX = bbox.startX;
			movingData->nextMove.bbox.endX = bbox.endX;
			movingData->nextMove.bbox.startY = bbox.startY;
			movingData->nextMove.bbox.endY = bbox.endY;
			movingData->nextMove.bbox.startZ = bbox.startZ;
			movingData->nextMove.bbox.endZ = bbox.endZ;

			ActionHistoryTool::addAction(Actions::brushmoving_undo, Actions::brushmoving_redo,
				Actions::brushmoving_cleanup, movingData);
		}
	}
}
