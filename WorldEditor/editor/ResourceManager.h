#pragma once

#include <QOpenGLShaderProgram>
#include <QString>
#include <QMap>
#include <QImage>
#include "Texture.h"

class ResourceManager
{
public:
	static void Init();
	static void Cleanup();
	static QOpenGLShaderProgram* getProgram(const QString& vertexShaderName, const QString& fragmentShaderName);
	static Texture& getTexture(const QString& texturePath);

private:
	struct ShaderTriplet {
		QString vsName;
		QString fsName;
		QOpenGLShaderProgram* program;
	};

	static QString m_shadersDirPath;
	static QString m_texturesDirPath;
	static QMap<QString, QString> m_vsMap;
	static QMap<QString, QString> m_fsMap;
	static QList<ShaderTriplet> shaderTriplets;
	static QMap<QString, Texture*> m_textureMap;
};
