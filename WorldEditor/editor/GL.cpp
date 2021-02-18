#include "GL.h"

#include <QDebug>
#include "../common/GlobalData.h"

GL* GL::m_functions = nullptr;
QOpenGLContext* GL::openGLContext = nullptr;
QOffscreenSurface* GL::surface = nullptr;
bool GL::m_isInitialized = false;

void GL::init()
{
	if (m_isInitialized)
	{
		qInfo() << "OpenGL functions have been already initialized";
		return;
	}

	GL* functions = new GL();
	m_functions = functions;
}

GL::GL()
{
	/* Set up a dummy context which is needed to only get function pointers */
	QSurfaceFormat format;
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);

	openGLContext = new QOpenGLContext();
	//GlobalData::dummyContext = openGLContext;

	openGLContext->setFormat(format);
	if (!openGLContext->create()) {
		qDebug() << "Failed to initialize OpenGL context";
	}

	surface = new QOffscreenSurface;
	surface->setFormat(format);
	surface->create();
	openGLContext->makeCurrent(surface);

	initializeOpenGLFunctions();
	
	m_isInitialized = true;
}
