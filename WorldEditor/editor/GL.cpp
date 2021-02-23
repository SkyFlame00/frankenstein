#include "GL.h"
#include <QDebug>

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

void GL::clearError()
{
	while (m_functions->glGetError() != GL_NO_ERROR);
}

bool GL::logCall(const char* function, const char* file, int line)
{
	auto getErrorMessage = [](GLenum error) {
		switch (error)
		{
		case GL_INVALID_ENUM:
			return "OPENGL_ERROR: GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "OPENGL_ERROR: GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "OPENGL_ERROR: GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "OPENGL_ERROR: GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY:
			return "OPENGL_ERROR: GL_OUT_OF_MEMORY";
		case GL_STACK_UNDERFLOW:
			return "OPENGL_ERROR: GL_STACK_UNDERFLOW";
		case GL_STACK_OVERFLOW:
			return "OPENGL_ERROR: GL_STACK_OVERFLOW";
		default:
		case GL_NO_ERROR:
			return "No error";
		}
	};

	while (GLenum error = m_functions->glGetError())
	{
		qWarning() << "[OpenGL Error] (" << getErrorMessage(error) << "): " << function <<
			" " << file << ":" << line;
		return false;
	}

	return true;
}
