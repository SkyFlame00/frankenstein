#include "ConstructionBlock.h"
#include "ResourceManager.h"
#include <algorithm>
#include <math.h>

ConstructionBlock::ConstructionBlock(QVector3D startPoint, QVector3D endPoint)
{
	m_boundingBox.startX = std::min(startPoint.x(), endPoint.x());
	m_boundingBox.endX = std::max(startPoint.x(), endPoint.x());
	m_boundingBox.startY = std::min(startPoint.y(), endPoint.y());
	m_boundingBox.endY = std::max(startPoint.y(), endPoint.y());
	m_boundingBox.startZ = std::min(startPoint.z(), endPoint.z());
	m_boundingBox.endZ = std::max(startPoint.z(), endPoint.z());

	m_vertices.push_back(startPoint);
	m_vertices.push_back(QVector3D(endPoint.x(), startPoint.y(), startPoint.z()));
	m_vertices.push_back(QVector3D(endPoint.x(), startPoint.y(), endPoint.z()));
	m_vertices.push_back(QVector3D(startPoint.x(), startPoint.y(), endPoint.z()));
	m_vertices.push_back(endPoint);
	m_vertices.push_back(QVector3D(startPoint.x(), endPoint.y(), endPoint.z()));
	m_vertices.push_back(QVector3D(startPoint.x(), endPoint.y(), startPoint.y()));
	m_vertices.push_back(QVector3D(endPoint.x(), endPoint.y(), startPoint.z()));

	m_indices[0] = 0; // Line 1
	m_indices[1] = 1;
	m_indices[2] = 1; // Line 2
	m_indices[3] = 2;
	m_indices[4] = 2; // Line 3
	m_indices[5] = 3;
	m_indices[6] = 3; // Line 4
	m_indices[7] = 0;
	m_indices[8] = 4; // Line 5
	m_indices[9] = 5;
	m_indices[10] = 5; // Line 6
	m_indices[11] = 6;
	m_indices[12] = 6; // Line 7
	m_indices[13] = 7;
	m_indices[14] = 7; // Line 8
	m_indices[15] = 4;
	m_indices[16] = 0; // Line 9
	m_indices[17] = 6;
	m_indices[18] = 1; // Line 10
	m_indices[19] = 7;
	m_indices[20] = 2; // Line 11
	m_indices[21] = 4;
	m_indices[22] = 3; // Line 12
	m_indices[23] = 5;

	createLinesVertices();

	m_vao.create();
	m_vao.bind();
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
	$->glEnableVertexAttribArray(0);
	$->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	m_vbo.release();
	m_vao.release();
	m_program = ResourceManager::getProgram("plain_with_uniform_color", "plain_with_uniform_color");
}

ConstructionBlock::~ConstructionBlock()
{
}

void ConstructionBlock::createLinesVertices()
{
	for (int i = 0; i < sizeof(m_indices) / sizeof(int); i++)
	{
		auto v = m_vertices[m_indices[i]];
		m_linesVertices[i * 3] = v.x();
		m_linesVertices[i * 3 + 1] = v.y();
		m_linesVertices[i * 3 + 2] = v.z();
	}
}

void ConstructionBlock::get2DBounds(Axis axis, float* horStart, float* horEnd, float* verStart, float* verEnd)
{
	switch (axis)
	{
	case Axis::X:
		*horStart = m_boundingBox.startZ;
		*horEnd = m_boundingBox.endZ;
		*verStart = m_boundingBox.startY;
		*verEnd = m_boundingBox.endY;
	case Axis::Y:
		*horStart = m_boundingBox.startX;
		*horEnd = m_boundingBox.endX;
		*verStart = m_boundingBox.startZ;
		*verEnd = m_boundingBox.endZ;
	case Axis::Z:
		*horStart = m_boundingBox.startX;
		*horEnd = m_boundingBox.endX;
		*verStart = m_boundingBox.startY;
		*verEnd = m_boundingBox.endY;
	}
}

void ConstructionBlock::startDrag(Axis axis, QVector2D pos, float zoomFactor)
{
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	float hor = pos.x();
	float ver = pos.y();
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);
	
	if (horBoundStart <= hor && hor <= horBoundEnd &&
		verBoundStart <= ver && ver <= verBoundEnd)
	{
		m_mode = Mode::MOVE;
		m_lastPos = pos;
		m_moveDelta = 0.0f;
		return;
	}

	float leftTopHorStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float leftTopHorEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	float leftTopVerStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float leftTopVerEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	if (leftTopHorStart <= hor && hor <= leftTopHorEnd &&
		leftTopVerStart <= ver && ver <= leftTopVerEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_TOP;
		return;
	}

	float centerTopHorStart = (horBoundStart + horBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	float centerTopHorEnd = (horBoundStart + horBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;
	float centerTopVerStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float centerTopVerEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	if (centerTopHorStart <= hor && hor <= centerTopHorEnd &&
		centerTopVerStart <= ver && ver <= centerTopVerEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_TOP;
		return;
	}

	float rightTopHorStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float rightTopHorEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	float rightTopVerStart = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float rightTopVerEnd = verBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;

	if (rightTopHorStart <= hor && hor <= rightTopHorEnd &&
		rightTopVerStart <= ver && ver <= rightTopVerEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_TOP;
		return;
	}

	float rightCenterHorStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float rightCenterHorEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	float rightCenterVerStart = (verBoundStart + verBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	float rightCenterVerEnd = (verBoundStart + verBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;

	if (rightCenterHorStart <= hor && hor <= rightCenterHorEnd &&
		rightCenterVerStart <= ver && ver <= rightCenterVerEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_CENTER;
		return;
	}

	float rightBottomHorStart = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor;
	float rightBottomHorEnd = horBoundEnd + RESIZE_POINT_MARGIN * zoomFactor + RESIZE_POINT_SIZE * zoomFactor;
	float rightBottomVerStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float rightBottomVerEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	if (rightBottomHorStart <= hor && hor <= rightBottomHorEnd &&
		rightBottomVerStart <= ver && ver <= rightBottomVerEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::RIGHT_BOTTOM;
		return;
	}

	float centerBottomHorStart = (horBoundStart + horBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	float centerBottomHorEnd = (horBoundStart + horBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;
	float centerBottomVerStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float centerBottomVerEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	if (centerBottomHorStart <= hor && hor <= centerBottomHorEnd &&
		centerBottomVerStart <= ver && ver <= centerBottomVerEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::CENTER_BOTTOM;
		return;
	}

	float leftBottomHorStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float leftBottomHorEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	float leftBottomVerStart = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float leftBottomVerEnd = verBoundStart - RESIZE_POINT_MARGIN * zoomFactor;

	if (leftBottomHorStart <= hor && hor <= leftBottomHorEnd &&
		leftBottomVerStart <= ver && ver <= leftBottomVerEnd)
	{
		m_moveDeltaX = 0.0f;
		m_moveDeltaY = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_BOTTOM;
		return;
	}

	float leftCenterHorStart = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor - RESIZE_POINT_SIZE * zoomFactor;
	float leftCenterHorEnd = horBoundStart - RESIZE_POINT_MARGIN * zoomFactor;
	float leftCenterVerStart = (verBoundStart + verBoundEnd) / 2 - RESIZE_POINT_SIZE * zoomFactor / 2;
	float leftCenterVerEnd = (verBoundStart + verBoundEnd) / 2 + RESIZE_POINT_SIZE * zoomFactor / 2;

	if (leftCenterHorStart <= hor && hor <= leftCenterHorEnd &&
		leftCenterVerStart <= ver && ver <= leftCenterVerEnd)
	{
		m_moveDelta = 0.0f;
		m_resizeDirection = ResizeDirection::LEFT_CENTER;
		return;
	}
}

void ConstructionBlock::doResizeStep(Axis axis, QVector2D pos, float step)
{
	float hor = pos.x();
	float ver = pos.y();
	float lastHor = m_lastPos.x();
	float lastVer = m_lastPos.y();
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);

	//if (m_mode == Mode::RESIZE)
	{
		/* LEFT TOP */
		if (m_resizeDirection == ResizeDirection::LEFT_TOP)
		{
			m_moveDeltaX += hor - lastHor;
			m_moveDeltaY += ver - lastVer;
			
			if (m_moveDeltaX >= step)
			{
				float steps = m_moveDeltaX / step;
				m_moveDeltaX -= steps;

				if (steps < 0)
				{
					float length = std::abs(horBoundEnd - horBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, true, true, steps);
			}

			if (m_moveDeltaY >= step)
			{
				float steps = m_moveDeltaY / step;
				m_moveDeltaY -= steps;

				if (steps < 0)
				{
					float length = std::abs(verBoundEnd - verBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, false, false, steps);
			}
		}
		/* CENTER TOP */
		else if (m_resizeDirection == ResizeDirection::CENTER_TOP)
		{
			m_moveDelta += ver - lastVer;

			if (m_moveDelta < step)
			{
				return;
			}

			float steps = m_moveDelta / step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (length + steps < 1.0f)
				{
					steps = length - 1;
				}
			}

			calcResize(axis, false, false, steps);
		}
		/* RIGHT TOP */
		else if (m_resizeDirection == ResizeDirection::RIGHT_TOP)
		{
			m_moveDeltaX += hor - lastHor;
			m_moveDeltaY += ver - lastVer;

			if (m_moveDeltaX >= step)
			{
				float steps = m_moveDeltaX / step;
				m_moveDeltaX -= steps;

				if (steps < 0)
				{
					float length = std::abs(horBoundEnd - horBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, true, false, steps);
			}

			if (m_moveDeltaY >= step)
			{
				float steps = m_moveDeltaY / step;
				m_moveDeltaY -= steps;

				if (steps < 0)
				{
					float length = std::abs(verBoundEnd - verBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, false, false, steps);
			}
		}
		/* RIGHT CENTER */
		else if (m_resizeDirection == ResizeDirection::RIGHT_CENTER)
		{
			m_moveDelta += ver - lastVer;

			if (m_moveDelta < step)
			{
				return;
			}

			float steps = m_moveDelta / step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (length + steps < 1.0f)
				{
					steps = length - 1;
				}
			}

			calcResize(axis, true, false, steps);
		}
		/* RIGHT BOTTOM */
		else if (m_resizeDirection == ResizeDirection::RIGHT_BOTTOM)
		{
			m_moveDeltaX += hor - lastHor;
			m_moveDeltaY += ver - lastVer;

			if (m_moveDeltaX >= step)
			{
				float steps = m_moveDeltaX / step;
				m_moveDeltaX -= steps;

				if (steps < 0)
				{
					float length = std::abs(horBoundEnd - horBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, true, false, steps);
			}

			if (m_moveDeltaY >= step)
			{
				float steps = m_moveDeltaY / step;
				m_moveDeltaY -= steps;

				if (steps < 0)
				{
					float length = std::abs(verBoundEnd - verBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, false, true, steps);
			}
		}
		/* CENTER BOTTOM */
		else if (m_resizeDirection == ResizeDirection::CENTER_BOTTOM)
		{
			m_moveDelta += ver - lastVer;

			if (m_moveDelta < step)
			{
				return;
			}

			float steps = m_moveDelta / step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(verBoundEnd - verBoundStart);

				if (length + steps < 1.0f)
				{
					steps = length - 1;
				}
			}

			calcResize(axis, false, true, steps);
		}
		/* LEFT BOTTOM */
		else if (m_resizeDirection == ResizeDirection::LEFT_BOTTOM)
		{
			m_moveDeltaX += hor - lastHor;
			m_moveDeltaY += ver - lastVer;

			if (m_moveDeltaX >= step)
			{
				float steps = m_moveDeltaX / step;
				m_moveDeltaX -= steps;

				if (steps < 0)
				{
					float length = std::abs(horBoundEnd - horBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, true, true, steps);
			}

			if (m_moveDeltaY >= step)
			{
				float steps = m_moveDeltaY / step;
				m_moveDeltaY -= steps;

				if (steps < 0)
				{
					float length = std::abs(verBoundEnd - verBoundStart);

					if (length + steps < 1.0f)
					{
						steps = length - 1;
					}
				}

				calcResize(axis, false, true, steps);
			}
		}
		/* LEFT CENTER */
		else if (m_resizeDirection == ResizeDirection::LEFT_CENTER)
		{
			m_moveDelta += ver - lastVer;

			if (m_moveDelta < step)
			{
				return;
			}

			float steps = m_moveDelta / step;
			m_moveDelta -= steps;

			if (steps < 0)
			{
				float length = std::abs(horBoundEnd - horBoundStart);

				if (length + steps < 1.0f)
				{
					steps = length - 1;
				}
			}

			calcResize(axis, true, true, steps);
		}
	}

	m_lastPos = pos;
}

void ConstructionBlock::calcResize(Axis axis, bool isHorizontal, bool isReversed, float steps)
{
	float horBoundStart, horBoundEnd, verBoundStart, verBoundEnd;
	float horStart, horEnd, verStart, verEnd;
	get2DBounds(axis, &horBoundStart, &horBoundEnd, &verBoundStart, &verBoundEnd);
	
	if (isReversed)
	{
		horStart = horBoundEnd;
		horEnd = horBoundStart;
		verStart = verBoundEnd;
		verEnd = verBoundStart;
	}
	else
	{
		horStart = horBoundStart;
		horEnd = horBoundEnd;
		verStart = verBoundStart;
		verEnd = verBoundEnd;
	}

	for (auto& v : m_vertices)
	{
		switch (axis)
		{
		case Axis::X:
			if (isHorizontal)
			{
				float factor = std::abs((v.z() - horStart) / (horEnd - horStart));
				v.setZ(v.z() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.y() - verStart) / (verEnd - verStart));
				v.setY(v.y() + steps * factor);
			}
			break;
		case Axis::Y:
			if (isHorizontal)
			{
				float factor = std::abs((v.x() - horStart) / (horEnd - horStart));
				v.setX(v.x() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.z() - verStart) / (verEnd - verStart));
				v.setZ(v.z() + steps * factor);
			}
			break;
		case Axis::Z:
			if (isHorizontal)
			{
				float factor = std::abs((v.x() - horStart) / (horEnd - horStart));
				v.setX(v.x() + steps * factor);
			}
			else
			{
				float factor = std::abs((v.y() - verStart) / (verEnd - verStart));
				v.setY(v.y() + steps * factor);
			}
		}
	}

	switch (axis)
	{
	case Axis::X:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startZ += steps;
			else
				m_boundingBox.endZ += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startY += steps;
			else
				m_boundingBox.endY += steps;
		}
		break;
	case Axis::Y:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startX += steps;
			else
				m_boundingBox.endX += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startZ += steps;
			else
				m_boundingBox.endZ += steps;
		}
		break;
	case Axis::Z:
		if (isHorizontal)
		{
			if (isReversed)
				m_boundingBox.startX += steps;
			else
				m_boundingBox.endX += steps;
		}
		else
		{
			if (isReversed)
				m_boundingBox.startY += steps;
			else
				m_boundingBox.endY += steps;
		}
	}

	createLinesVertices();
}

void ConstructionBlock::render2D(QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera)
{
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(zoomVec);

	m_program->bind();
	m_program->setUniformValue("proj", proj);
	m_program->setUniformValue("view", camera.getViewMatrix());
	m_program->setUniformValue("model", model);
	m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f);

	m_vao.bind();
	$->glDrawArrays(GL_LINES, 0, verticesCount());
}
