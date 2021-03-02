#include "GLWidget2D.h"
#include "../../common/GlobalData.h"
#include "../../common/helpers.h"

void GLWidget2D::processBlockTool()
{
	auto globalData = GlobalData::getInstance();
	auto blockToolData = &globalData->m_blockToolData;
	float x, y;
	float nearestX, nearestY;
	getMouseCoordinates(m_inputData.mouseX, m_inputData.mouseY, &x, &y);
	getNearestPointFromScreen(m_inputData.mouseX, m_inputData.mouseY, &nearestX, &nearestY);

	if (blockToolData->state == BlockToolState::CREATING)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			QVector3D pointPos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY, 1.0f);
			m_dragData.startPoint = new Point(1.0f, pointPos);
			m_scene->m_gui2DObjects.push_back(m_dragData.startPoint);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			QVector3D endPointPos = Helpers::get3DPointFrom2D(m_axis, nearestX, nearestY, -1.0f);
			QVector3D startPointPos = m_dragData.startPoint->m_origin;
			bool isSamePoint = this->isSamePoint(startPointPos, endPointPos);
			bool isLyingOnSameAxis = hasSameCoordinate(startPointPos, endPointPos);

			if (!isSamePoint && !isLyingOnSameAxis)
			{
				m_scene->m_gui2DObjects.removeOne(m_dragData.startPoint);
				m_scene->m_gui2DObjects.removeOne(m_dragData.endPoint);

				if (blockToolData->blockInstance)
				{
					m_scene->m_gui2DObjects.removeOne(blockToolData->blockInstance);
					m_scene->m_gui3DObjects.removeOne(blockToolData->blockInstance);
					delete blockToolData->blockInstance;
				}

				blockToolData->blockInstance = new ConstructionBlock(startPointPos, endPointPos);
				m_scene->m_gui2DObjects.push_back(blockToolData->blockInstance);
				m_scene->m_gui3DObjects.push_back(blockToolData->blockInstance);
			}
			else if (!blockToolData->blockInstance && !isSamePoint && isLyingOnSameAxis)
			{
				if (m_dragData.endPoint)
				{
					m_scene->m_gui2DObjects.removeOne(m_dragData.endPoint);
					delete m_dragData.endPoint;
				}

				m_dragData.endPoint = new Point(1.0f, endPointPos);
				m_scene->m_gui2DObjects.push_back(m_dragData.endPoint);
			}
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			if (m_dragData.startPoint)
			{
				m_scene->m_gui2DObjects.removeOne(m_dragData.startPoint);
				delete m_dragData.startPoint;
				m_dragData.startPoint = nullptr;
			}
			if (m_dragData.endPoint)
			{
				m_scene->m_gui2DObjects.removeOne(m_dragData.endPoint);
				delete m_dragData.endPoint;
				m_dragData.endPoint = nullptr;
			}
			if (blockToolData->blockInstance)
			{
				blockToolData->state = BlockToolState::READY_TO_EDIT;
				blockToolData->blockInstance->m_isEditingMode = true;
			}
		}
	}
	else if (blockToolData->state == BlockToolState::READY_TO_EDIT)
	{
		Qt::CursorShape cursor = blockToolData->blockInstance->checkHover(x, y, m_axis, getZoomFactor());
		setCursor(cursor);

		if (m_inputData.keyEscape == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			m_scene->m_gui2DObjects.removeOne(blockToolData->blockInstance);
			m_scene->m_gui3DObjects.removeOne(blockToolData->blockInstance);
			delete blockToolData->blockInstance;
			blockToolData->blockInstance = nullptr;
			blockToolData->state = BlockToolState::CREATING;
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			auto _state = blockToolData->blockInstance->startDrag(m_axis, QVector2D(x, y), getZoomFactor());
			auto state = Helpers::mapToBlockToolState(_state);
			blockToolData->state = state;

			if (state == BlockToolState::MOVE)
				setCursor(Qt::ClosedHandCursor);
		}
	}
	else if (blockToolData->state == BlockToolState::RESIZE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			blockToolData->blockInstance->doResizeStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			blockToolData->state = BlockToolState::READY_TO_EDIT;
			setCursor(Qt::ArrowCursor);
		}
	}
	else if (blockToolData->state == BlockToolState::MOVE)
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_PROCESSED)
		{
			float step = static_cast<float>(m_grid->getStep());
			blockToolData->blockInstance->doMoveStep(m_axis, QVector2D(x, y), step);
		}
		else if (m_inputData.leftMouseDown == ButtonDownState::RELEASED_NOT_PROCESSED)
		{
			blockToolData->state = BlockToolState::READY_TO_EDIT;
			setCursor(Qt::ArrowCursor);
		}
	}
}