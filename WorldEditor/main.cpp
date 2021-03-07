#include <QApplication>

#include "gui/MainWindow.h"
#include "gui/Debug.h"
#include "editor/ResourceManager.h"
#include "editor/GL.h"
#include "common/GlobalData.h"
#include <cstdlib>
#include <ctime>
#include "common/ActionHistoryTool.h"

int main(int argc, char* argv[])
{
	std::srand(std::time(nullptr));

	QApplication app(argc, argv);

#ifndef QT_NO_DEBUG_OUTPUT
	Debug::Init();
#endif

	ResourceManager::Init();
	GL::init();
	GlobalData::init();
	ActionHistoryTool::init();
	MainWindow::init();

	auto window = MainWindow::getInstance();
	//window.showMaximized();
	window->setMinimumSize(320, 240);
	window->show();

	int status = app.exec();

	MainWindow::cleanup();
	ActionHistoryTool::cleanup();
	GlobalData::cleanup();
	//GL::cleanup();
	//ResourceManager::cleanup();

	return status;
}
