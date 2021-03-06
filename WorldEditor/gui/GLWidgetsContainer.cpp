#include "GLWidgetsContainer.h"

GLWidgetsContainer::GLWidgetsContainer(GLWidget3D* gl3D, GLWidget2D* gl2DX, GLWidget2D* gl2DY, GLWidget2D* gl2DZ, QWidget* parent)
	: QWidget(parent), m_gl3D(gl3D), m_gl2DX(gl2DX), m_gl2DY(gl2DY), m_gl2DZ(gl2DZ)
{
    connect(gl2DX, &GLWidget2D::shown, this, &GLWidgetsContainer::handleGLWidgetShown);
    connect(gl2DX, &GLWidget2D::hidden, this, &GLWidgetsContainer::handleGLWidgetHidden);
    connect(gl2DY, &GLWidget2D::shown, this, &GLWidgetsContainer::handleGLWidgetShown);
    connect(gl2DY, &GLWidget2D::hidden, this, &GLWidgetsContainer::handleGLWidgetHidden);
    connect(gl2DZ, &GLWidget2D::shown, this, &GLWidgetsContainer::handleGLWidgetShown);
    connect(gl2DZ, &GLWidget2D::hidden, this, &GLWidgetsContainer::handleGLWidgetHidden);

    QString styleSheetString = "background: black; color: white; padding: 2px;";

    /* Z/Y label */
    m_axisLabel_X = new QLabel("Z/Y view", parent);
    m_axisLabel_X->raise();
    m_axisLabel_X->setStyleSheet(styleSheetString);
    m_axisLabel_X->hide();

    /* X/Z label */
    m_axisLabel_Y = new QLabel("X/Z view", parent);
    m_axisLabel_Y->raise();
    m_axisLabel_Y->setStyleSheet(styleSheetString);
    m_axisLabel_Y->hide();

    /* X/Y label */
    m_axisLabel_Z = new QLabel("X/Y view", parent);
    m_axisLabel_Z->raise();
    m_axisLabel_Z->setStyleSheet(styleSheetString);
    m_axisLabel_Z->hide();

    QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(gl3D);
	layout->addWidget(gl2DX);
	layout->addWidget(gl2DY);
	layout->addWidget(gl2DZ);
    layout->addWidget(m_axisLabel_X);
    layout->addWidget(m_axisLabel_Y);
    layout->addWidget(m_axisLabel_Z);
	setLayout(layout);
}

void GLWidgetsContainer::resizeEvent(QResizeEvent* event)
{
    doResize();
}

void GLWidgetsContainer::doResize()
{
    auto gl2DX_pos = QPoint(width() / 2 + m_margin, m_margin);
    auto gl2DY_pos = QPoint(m_margin, height() / 2 + m_margin);
    auto gl2DZ_pos = QPoint(width() / 2 + m_margin, height() / 2 + m_margin);

    m_handleCenter = rect().center();
    m_gl3D->setGeometry(QRect(QPoint(m_margin, m_margin), m_handleCenter - QPoint(m_margin, m_margin)));
    m_gl2DX->setGeometry(QRect(gl2DX_pos, QPoint(width() - m_margin, height() / 2 - m_margin)));
    m_gl2DY->setGeometry(QRect(gl2DY_pos, QPoint(width() / 2 - m_margin, height() - m_margin)));
    m_gl2DZ->setGeometry(QRect(gl2DZ_pos, QPoint(width() - m_margin, height() - m_margin)));

    m_axisLabel_X->setGeometry(gl2DX_pos.x(), gl2DX_pos.y(), LABEL_WIDTH, LABEL_HEIGHT);
    m_axisLabel_Y->setGeometry(gl2DY_pos.x(), gl2DY_pos.y(), LABEL_WIDTH, LABEL_HEIGHT);
    m_axisLabel_Z->setGeometry(gl2DZ_pos.x(), gl2DZ_pos.y(), LABEL_WIDTH, LABEL_HEIGHT);

}

void GLWidgetsContainer::mouseMoveEvent(QMouseEvent* e)
{
    if (m_resizeDir == ResizeDirection::NONE)
    {
        float x = e->pos().x();
        float y = e->pos().y();
        float startX = m_handleCenter.x() - m_margin;
        float endX = m_handleCenter.x() + m_margin;
        float startY = m_handleCenter.y() - m_margin;
        float endY = m_handleCenter.y() + m_margin;

        if (startX <= x && x <= endX &&
            startY <= y && y <= endY)
            setCursor(Qt::SizeAllCursor);
        else if (startX <= x && x <= endX)
            setCursor(Qt::SplitHCursor);
        else if (startY <= y && y <= endY)
            setCursor(Qt::SplitVCursor);
        else
            setCursor(Qt::ArrowCursor);

        return;
    }

    float handleCenterX, handleCenterY;
    float x = e->pos().x();
    float y = e->pos().y();
    
    if (x < RESIZE_THRESHOLD)
        x = RESIZE_THRESHOLD;
    else if (x > width() - RESIZE_THRESHOLD)
        x = width() - RESIZE_THRESHOLD;
    if (y < RESIZE_THRESHOLD)
        y = RESIZE_THRESHOLD;
    else if (y > height() - RESIZE_THRESHOLD)
        y = height() - RESIZE_THRESHOLD;

    QPoint newPos, bottomRightPt, bottomLeftPt, topRightPt, topLeftPt;
    QRect newGeo_3D = m_gl3D->geometry();
    QRect newGeo_2DX = m_gl2DX->geometry();
    QRect newGeo_2DY = m_gl2DY->geometry();
    QRect newGeo_2DZ = m_gl2DZ->geometry();

    if (m_resizeDir == ResizeDirection::HORIZONTAL)
    {
        handleCenterX = x;
        handleCenterY = m_handleCenter.y();
        bottomRightPt = QPoint(x, newGeo_3D.bottomRight().y()) + QPoint(-m_margin, 0);
        bottomLeftPt = QPoint(x, newGeo_2DX.bottomLeft().y()) + QPoint(+m_margin, 0);
        topRightPt = QPoint(x, newGeo_2DY.topRight().y()) + QPoint(-m_margin, 0);
        topLeftPt = QPoint(x, newGeo_2DZ.topLeft().y()) + QPoint(+m_margin, 0);
    }
    else if (m_resizeDir == ResizeDirection::VERTICAL)
    {
        handleCenterX = m_handleCenter.x();
        handleCenterY = y;
        bottomRightPt = QPoint(newGeo_3D.bottomRight().x(), y) + QPoint(0, -m_margin);
        bottomLeftPt = QPoint(newGeo_2DX.bottomLeft().x(), y) + QPoint(0, -m_margin);
        topRightPt = QPoint(newGeo_2DY.topRight().x(), y) + QPoint(0, +m_margin);
        topLeftPt = QPoint(newGeo_2DZ.topLeft().x(), y) + QPoint(0, +m_margin);
    }
    else if (m_resizeDir == ResizeDirection::BOTH)
    {
        handleCenterX = x;
        handleCenterY = y;
        bottomRightPt = QPoint(x, y) + QPoint(-m_margin, -m_margin);
        bottomLeftPt = QPoint(x, y) + QPoint(+m_margin, -m_margin);
        topRightPt = QPoint(x, y) + QPoint(-m_margin, +m_margin);
        topLeftPt = QPoint(x, y) + QPoint(+m_margin, +m_margin);
    }

    newGeo_3D.setBottomRight(bottomRightPt);
    m_gl3D->setGeometry(newGeo_3D);

    newGeo_2DX.setBottomLeft(bottomLeftPt);
    m_gl2DX->setGeometry(newGeo_2DX);
    m_axisLabel_X->setGeometry(m_gl2DX->x(), m_gl2DX->y(), LABEL_WIDTH, LABEL_HEIGHT);

    newGeo_2DY.setTopRight(topRightPt);
    m_gl2DY->setGeometry(newGeo_2DY);
    m_axisLabel_Y->setGeometry(m_gl2DY->x(), m_gl2DY->y(), LABEL_WIDTH, LABEL_HEIGHT);

    newGeo_2DZ.setTopLeft(topLeftPt);
    m_gl2DZ->setGeometry(newGeo_2DZ);
    m_axisLabel_Z->setGeometry(m_gl2DZ->x(), m_gl2DZ->y(), LABEL_WIDTH, LABEL_HEIGHT);

    m_handleCenter = QPoint(handleCenterX, handleCenterY);
}

void GLWidgetsContainer::mousePressEvent(QMouseEvent* e)
{
    bool isLeftMouse = e->button() == Qt::LeftButton;

    if (isLeftMouse && m_leftMouseDown != ButtonDownState::RELEASED_PROCESSED)
    {
        return;
    }

    m_leftMouseDown = ButtonDownState::DOWN_PROCESSED;

    float x = e->pos().x();
    float y = e->pos().y();
    float startX = m_handleCenter.x() - m_margin;
    float endX = m_handleCenter.x() + m_margin;
    float startY = m_handleCenter.y() - m_margin;
    float endY = m_handleCenter.y() + m_margin;

    if (startX <= x && x <= endX &&
        startY <= y && y <= endY)
    {
        m_resizeDir = ResizeDirection::BOTH;
        setCursor(Qt::SizeAllCursor);
    }
    else if (startX <= x && x <= endX)
    {
        m_resizeDir = ResizeDirection::HORIZONTAL;
        setCursor(Qt::SplitHCursor);
    }
    else if (startY <= y && y <= endY)
    {
        m_resizeDir = ResizeDirection::VERTICAL;
        setCursor(Qt::SplitVCursor);
    }
}

void GLWidgetsContainer::mouseReleaseEvent(QMouseEvent* e)
{
    bool isLeftMouse = e->button() == Qt::LeftButton;

    if (isLeftMouse && m_leftMouseDown == ButtonDownState::DOWN_PROCESSED)
    {
        m_leftMouseDown = ButtonDownState::RELEASED_PROCESSED;
        m_resizeDir = ResizeDirection::NONE;
        setCursor(Qt::ArrowCursor);
    }
}

void GLWidgetsContainer::handleGLWidgetShown(GLWidget2D* instance)
{
    if (instance == m_gl2DX)
    {
        m_axisLabel_X->show();
    }
    else if (instance == m_gl2DY)
    {
        m_axisLabel_Y->show();
    }
    else if (instance == m_gl2DZ)
    {
        m_axisLabel_Z->show();
    }
}

void GLWidgetsContainer::handleGLWidgetHidden(GLWidget2D* instance)
{
    if (instance == m_gl2DX)
    {
        m_axisLabel_X->hide();
    }
    else if (instance == m_gl2DY)
    {
        m_axisLabel_Y->hide();
    }
    else if (instance == m_gl2DZ)
    {
        m_axisLabel_Z->hide();
    }
}
