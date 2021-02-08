#pragma once

#include <QWidget>
#include <QPlainTextEdit>

class Debug
{
public:
	static void Init();
	static void Log(QtMsgType, const QMessageLogContext&, const QString&);

private:
	static QWidget* m_debugWindow;
	static QPlainTextEdit* m_debugTextEdit;
};
