#pragma once

#include <QWidget>
#include "GLWidget3D/GLWidget3D.h"
#include "GLWidget2D/GLWidget2D.h"
#include <QHBoxLayout>
#include <QPoint>
#include "../common/types.h"
#include <QLabel>

class GLWidgetsContainer : public QWidget
{
	Q_OBJECT

public:
	GLWidgetsContainer(GLWidget3D* gl3D, GLWidget2D* gl2DX, GLWidget2D* gl2DY, GLWidget2D* gl2DZ, QWidget* parent = nullptr);
	void doResize();

public slots:
	void handleGLWidgetShown(GLWidget2D* instance);
	void handleGLWidgetHidden(GLWidget2D* instance);

private:
	enum class ResizeDirection
	{
		NONE,
		HORIZONTAL,
		VERTICAL,
		BOTH
	};

	const int LABEL_WIDTH = 80;
	const int LABEL_HEIGHT = 20;
	GLWidget3D* m_gl3D;
	GLWidget2D* m_gl2DX, * m_gl2DY, * m_gl2DZ;
	QPoint m_handleCenter;
	float m_margin = 5.0f;
	bool m_isResizing = false;
	ResizeDirection m_resizeDir = ResizeDirection::NONE;
	ButtonDownState m_leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
	const float RESIZE_THRESHOLD = 100.0f;
	QLabel* m_axisLabel_X;
	QLabel* m_axisLabel_Y;
	QLabel* m_axisLabel_Z;

	void resizeEvent(QResizeEvent* event) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
};
