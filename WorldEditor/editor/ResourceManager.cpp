#include "ResourceManager.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include "GL.h"
#include "../common/GlobalData.h"

QString ResourceManager::m_shadersDirPath;
QString ResourceManager::m_texturesDirPath;
QMap<QString, QString> ResourceManager::m_vsMap;
QMap<QString, QString> ResourceManager::m_fsMap;
QList<ResourceManager::ShaderTriplet> ResourceManager::shaderTriplets;
QMap<QString, Texture*> ResourceManager::m_textureMap;

void ResourceManager::Init()
{
	m_shadersDirPath = QDir::currentPath() + "/shaders";
	m_texturesDirPath = QDir::currentPath() + "/resources/textures";
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
	QString versionString = "#version 330 core\n";
	auto* global = GlobalData::getInstance();

	auto $ = GL::functions();
	int maxTextureUnits;
	GLCall($->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits));
	global->m_maxTextureUnits = maxTextureUnits;

	QString defineString = ("#define MAX_TEXTURE_UNITS " + std::to_string(maxTextureUnits) + "\n").c_str();
	fsSourceCode += versionString + defineString;

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
		fsSourceCode += fsIn.readAll();
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

Texture& ResourceManager::getTexture(const QString& texturePath)
{
	if (m_textureMap.find(texturePath) != m_textureMap.end())
	{
		return *m_textureMap[texturePath];
	}

	auto $ = GL::functions();
	QImage image = QImage(m_texturesDirPath + "/" + texturePath)
		.mirrored()
		.convertToFormat(QImage::Format_RGBA8888);
	GLuint textureId;

	GLCall($->glGenTextures(1, &textureId));
	GLCall($->glBindTexture(GL_TEXTURE_2D, textureId));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall($->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits()));

	Texture* texture = new Texture{ textureId, image.width(), image.height() };
	m_textureMap[texturePath] = texture;
	return *texture;
}
