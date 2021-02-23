#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLContext>
#include <QOffscreenSurface>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GL::clearError();\
    x;\
    ASSERT(GL::logCall(#x, __FILE__, __LINE__))

class GL : public QOpenGLExtraFunctions
{
public:
	static void init();
	inline static GL* functions() { return m_functions; }
	static void clearError();
	static bool logCall(const char* function, const char* file, int line);

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
