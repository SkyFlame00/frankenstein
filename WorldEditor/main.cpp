#include <QApplication>

#include "gui/MainWindow.h"
#include "gui/Debug.h"
#include "editor/ResourceManager.h"
#include "editor/GL.h"
#include "common/GlobalData.h"
#include <cstdlib>
#include <ctime>

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

	MainWindow window;
	//window.showMaximized();
	window.setMinimumSize(320, 240);
	window.show();

	return app.exec();
}
