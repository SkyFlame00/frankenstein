#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLContext>
#include <QOffscreenSurface>

class GL : public QOpenGLExtraFunctions
{
public:
	static void init();
	inline static GL* functions() { return m_functions; }

	static QOpenGLContext* openGLContext;
	static QOffscreenSurface* surface;

private:
	GL();

	static GL* m_functions;
	static bool m_isInitialized;
	
};

class OpenGLFunctions : public QOpenGLExtraFunctions
{
protected:
	GL* $ = GL::functions();
};
