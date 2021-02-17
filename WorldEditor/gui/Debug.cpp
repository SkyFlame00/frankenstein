#include "Debug.h"

#include <QVBoxLayout>
#include <QDebug>

QWidget* Debug::m_debugWindow = nullptr;
QPlainTextEdit* Debug::m_debugTextEdit = nullptr;
QTime Debug::m_timeStart;

void Debug::Init()
{
    m_debugWindow = new QWidget();
    m_debugWindow->setWindowTitle("Debug Log");
    m_debugWindow->setAttribute(Qt::WA_QuitOnClose, false); /* quit only when mainwindow is closed */
    m_debugWindow->setMinimumWidth(800);
    m_debugWindow->setMinimumHeight(400);
    QBoxLayout* layout = new QVBoxLayout();
    m_debugWindow->setLayout(layout);
    m_debugTextEdit = new QPlainTextEdit(m_debugWindow);
    QFont font = QFont("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    m_debugTextEdit->setFont(font);
    m_debugTextEdit->setReadOnly(true);
    layout->addWidget(m_debugTextEdit);
    m_debugWindow->show();
    qInstallMessageHandler(Log);

    m_timeStart = QTime::currentTime();
}

void Debug::Log(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString prefix = "";

    switch (type) {
    case QtDebugMsg:
        prefix = "DEBUG: ";
        break;
    case QtInfoMsg:
        prefix = "INFO: ";
        break;
    case QtWarningMsg:
        prefix = "WARNING: ";
        break;
    case QtCriticalMsg:
        prefix = "CRITICAL: ";
        break;
    case QtFatalMsg:
        prefix = "FATAL: ";
        break;
    }

    m_debugTextEdit->appendPlainText(prefix + msg);
}
