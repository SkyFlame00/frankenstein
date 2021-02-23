#include "ResourceManager.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include "GL.h"

QString ResourceManager::m_shadersDirPath;
QMap<QString, QString> ResourceManager::m_vsMap;
QMap<QString, QString> ResourceManager::m_fsMap;
QList<ResourceManager::ShaderTriplet> ResourceManager::shaderTriplets;

void ResourceManager::Init()
{
	m_shadersDirPath = QDir::currentPath() + "/shaders";
}

void ResourceManager::Cleanup()
{
	/* DELETE SHADERS */
}

QOpenGLShaderProgram* ResourceManager::getProgram(const QString& vertexShaderName, const QString& fragmentShaderName)
{
	QString vsFilePath = m_shadersDirPath + "/" + vertexShaderName + ".vert.glsl";
	QString fsFilePath = m_shadersDirPath + "/" + fragmentShaderName + ".frag.glsl";
	QString vsSourceCode;
	QString fsSourceCode;

	/* Load vertex shader source code */
	if (!m_vsMap.contains(vertexShaderName))
	{
		QFile vsFile(vsFilePath);

		if (!vsFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qWarning() << "Could not load vertex shader: " << vsFilePath;
		}

		QTextStream vsIn(&vsFile);
		vsSourceCode = vsIn.readAll();
		m_vsMap[vertexShaderName] = vsSourceCode;
	}
	else
	{
		vsSourceCode = m_vsMap.value(vertexShaderName);
	}

	/* Load fragment shader source code */
	if (!m_fsMap.contains(fragmentShaderName))
	{
		QFile fsFile(fsFilePath);

		if (!fsFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qWarning() << "Could not load vertex shader: " << fsFilePath;
		}

		QTextStream fsIn(&fsFile);
		fsSourceCode = fsIn.readAll();
		m_fsMap[fragmentShaderName] = fsSourceCode;
	}
	else
	{
		fsSourceCode = m_fsMap.value(fragmentShaderName);
	}

	/* Check if we already have a program with such shaders */
	for (auto& triplet : shaderTriplets)
	{
		if (triplet.vsName == vertexShaderName && triplet.fsName == fragmentShaderName)
		{
			return triplet.program;
		}
	}

	QOpenGLShaderProgram *program = new QOpenGLShaderProgram;

	GLCall(program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsSourceCode));
	GLCall(program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsSourceCode));
	GLCall(program->link());

	shaderTriplets.append({ vertexShaderName, fragmentShaderName, program });

	return program;
}
