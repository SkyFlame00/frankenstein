#pragma once

#include <QOpenGLShaderProgram>
#include <QString>
#include <QMap>

class ResourceManager
{
public:
	static void Init();
	static void Cleanup();
	static QOpenGLShaderProgram* getProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath);

private:
	struct ShaderTriplet {
		QString vsName;
		QString fsName;
		QOpenGLShaderProgram* program;
	};

	static QString m_shadersDirPath;
	static QMap<QString, QString> m_vsMap;
	static QMap<QString, QString> m_fsMap;
	static QList<ShaderTriplet> shaderTriplets;
};
