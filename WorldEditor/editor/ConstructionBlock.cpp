#include "ConstructionBlock.h"
#include "ResourceManager.h"
#include <algorithm>
#include <math.h>
#include "../common/helpers.h"
#include "../common/GlobalData.h"
#include "GL.h"
#include <memory>

ConstructionBlock::ConstructionBlock(QVector3D startPoint, QVector3D endPoint)
	: m_resizePoint(RESIZE_POINT_SIZE, 0.0f, 0.0f, 0.0f)
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
	m_vertices.push_back(QVector3D(startPoint.x(), endPoint.y(), startPoint.z()));
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

	m_vbo.addAttribute<float>(3);
	m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
	createVAO(m_vbo);

	m_program = ResourceManager::getProgram("plain_with_uniform_color", "plain_with_uniform_color");
}

ConstructionBlock::~ConstructionBlock()
{
	Renderable::~Renderable();
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
	m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
}

void ConstructionBlock::doMoveStep(Axis axis, QVector2D pos, float step)
{
	float stepsX = 0.0f, stepsY = 0.0f;
	m_moveDeltaX += pos.x() - m_lastPos.x();
	m_moveDeltaY += pos.y() - m_lastPos.y();

	if (std::abs(m_moveDeltaX) >= step)
	{
		stepsX = Helpers::trunc(m_moveDeltaX / step) * step;
		m_moveDeltaX -= stepsX;
	}

	if (std::abs(m_moveDeltaY) >= step)
	{
		stepsY = Helpers::trunc(m_moveDeltaY / step) * step;
		m_moveDeltaY -= stepsY;
	}

	if (std::abs(stepsX) > 0.0f || std::abs(stepsY) > 0.0f)
	{
		for (auto& v : m_vertices)
		{
			switch (axis)
			{
			case Axis::X:
				v.setZ(v.z() + stepsX);
				v.setY(v.y() + stepsY);
				break;
			case Axis::Y:
				v.setX(v.x() + stepsX);
				v.setZ(v.z() + stepsY);
				break;
			case Axis::Z:
				v.setX(v.x() + stepsX);
				v.setY(v.y() + stepsY);
			}
		}
		
		switch (axis)
		{
		case Axis::X:
			m_boundingBox.startZ += stepsX;
			m_boundingBox.endZ += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
			break;
		case Axis::Y:
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startZ += stepsY;
			m_boundingBox.endZ += stepsY;
			break;
		case Axis::Z:
			m_boundingBox.startX += stepsX;
			m_boundingBox.endX += stepsX;
			m_boundingBox.startY += stepsY;
			m_boundingBox.endY += stepsY;
		}

		createLinesVertices();
		m_vbo.allocate(&m_linesVertices[0], m_linesVerticesCount * 3 * sizeof(float));
	}
	
	m_lastPos = pos;
}

void ConstructionBlock::render2D(QOpenGLContext* context, QMatrix4x4& proj, QVector3D& zoomVec, Camera& camera, Axis axis, float factor)
{
	auto vao = GlobalData::getRenderableVAO(*context, *this);
	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(zoomVec);
	
	GLCall(m_program->bind());
	GLCall(m_program->setUniformValue("proj", proj));
	GLCall(m_program->setUniformValue("view", camera.getViewMatrix()));
	GLCall(m_program->setUniformValue("model", model));
	GLCall(m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f));

	vao->bind();
	GLCall($->glDrawArrays(GL_LINES, 0, verticesCount()));

	if (m_isEditingMode)
	{
		QList<QVector3D>* translationVectors = getResizePointsTranslationVectors(axis, factor, zoomVec);

		for (auto& vec : *translationVectors)
		{
			context->makeCurrent(context->surface());

			model.setToIdentity();
			model.translate(vec);

			m_resizePoint.m_program->bind();
			m_resizePoint.m_program->setUniformValue("proj", proj);
			m_resizePoint.m_program->setUniformValue("view", camera.getViewMatrix());
			m_resizePoint.m_program->setUniformValue("model", model);
			m_resizePoint.m_program->setUniformValue("color", 1.0f, 0.1f, 0.1f);

			m_resizePoint.bindVAO(context);
			$->glDrawArrays(GL_TRIANGLES, 0, m_resizePoint.verticesCount());
		}

		delete translationVectors;
	}
}
