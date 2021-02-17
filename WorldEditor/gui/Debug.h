#pragma once

#include <QWidget>
#include <QPlainTextEdit>
#include <QTime>

class Debug
{
public:
	static void Init();
	static void Log(QtMsgType, const QMessageLogContext&, const QString&);

	static QTime m_timeStart;

private:
	static QWidget* m_debugWindow;
	static QPlainTextEdit* m_debugTextEdit;
};
