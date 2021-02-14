#include <QApplication>

#include "gui/MainWindow.h"
#include "gui/Debug.h"
#include "editor/ResourceManager.h"
#include "editor/GL.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

#ifndef QT_NO_DEBUG_OUTPUT
	Debug::Init();
#endif

	ResourceManager::Init();
	GL::init();

	MainWindow window;
	//window.showMaximized();
	window.setMinimumSize(320, 240);
	window.show();

	return app.exec();
}
