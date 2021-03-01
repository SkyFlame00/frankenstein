#include "Renderer3D.h"

#include "GL.h"

Renderer3D::Renderer3D(Camera* camera, int width, int height)
	: m_camera(camera), m_brushMap(nullptr)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(m_fov, (float)width / (float)height, 0.001f, 1000.0f);
}

Renderer3D::~Renderer3D()
{
	GLCall($->glDeleteTextures(1, &m_seletionTexture));
	GLCall($->glDeleteRenderbuffers(1, &m_selectionRbo));
	GLCall($->glDeleteFramebuffers(1, &m_selectionFbo));
}

void Renderer3D::setupSelectionBuffer(QOpenGLContext* context)
{
	useContext(context);
	GLCall($->glGenFramebuffers(1, &m_selectionFbo));
	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, m_selectionFbo));

	GLCall($->glGenTextures(1, &m_seletionTexture));
	GLCall($->glBindTexture(GL_TEXTURE_2D, m_seletionTexture));
	GLCall($->glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_screenWidth, m_screenHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall($->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall($->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_seletionTexture, 0));

	GLCall($->glGenRenderbuffers(1, &m_selectionRbo));
	GLCall($->glBindRenderbuffer(GL_RENDERBUFFER, m_selectionRbo));
	GLCall($->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight));
	GLCall($->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_selectionRbo));

	if ($->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		qWarning() << "Framebuffer is not complete!";

	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer3D::setFrustrum(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_projMatrix.setToIdentity();
	m_projMatrix.perspective(m_fov, (float)width / (float)height, m_near, m_far);

	GLCall($->glBindTexture(GL_TEXTURE_2D, m_seletionTexture));
	GLCall($->glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_screenWidth, m_screenHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr));
	GLCall($->glBindRenderbuffer(GL_RENDERBUFFER, m_selectionRbo));
	GLCall($->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight));
}

void Renderer3D::render(QOpenGLContext* context, QList<Brush*>& objects, QList<Renderable*>& guiObjects)
{
	useContext(context);
	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall($->glEnable(GL_DEPTH_TEST));
	GLCall($->glClearColor(0.1, 0.2, 0.4, 1.0));
	GLCall($->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	QVector3D zoomVec(1.0f, 1.0f, 1.0f);

	for (auto& object : objects)
	{
		object->render3D(context, m_projMatrix, zoomVec, *m_camera);
	}

	for (auto& guiObject : guiObjects)
	{
		guiObject->render3D(context, m_projMatrix, *m_camera);
	}

	writeSelectionBuffer(context, objects);
}

void Renderer3D::writeSelectionBuffer(QOpenGLContext* context, QList<Brush*>& objects)
{
	useContext(context);
	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, m_selectionFbo));
	GLCall($->glEnable(GL_DEPTH_TEST));
	GLCall($->glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall($->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	if (m_brushMap)
	{
		delete m_brushMap;
	}
	m_brushMap = new std::unordered_map<float, Brush*>;

	QVector3D zoomVec(1.0f, 1.0f, 1.0f);
	float delta = 1.0f / objects.size();
	float renderId = delta;

	for (auto& object : objects)
	{
		object->changeRenderId(renderId);
		object->writeSelectionBuffer(context, renderId, m_projMatrix, zoomVec, *m_camera);
		//(*m_brushMap)[renderId] = object;
		m_brushMap->insert(std::make_pair(renderId, object));
		renderId += delta;
	}

	useContext(context);
	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

float Renderer3D::getSelectionValue(int x, int y)
{
	float renderId;

	//qInfo() << "x = " << x << ", y = " << y;

	//float* data = new float[m_screenWidth * m_screenHeight];

	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, m_selectionFbo));
	GLCall($->glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, &renderId));
	//GLCall($->glReadPixels(0, 0, m_screenWidth, m_screenHeight, GL_RED, GL_FLOAT, data));
	GLCall($->glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//QList<float> floats;

	//for (int i = 0; i < m_screenWidth * m_screenHeight; i++)
	//{
	//	floats.append(data[i]);

	//	if (data[i] != 0.0f)
	//	{
	//		qInfo() << "here";
	//	}
	//}

	//delete [] data;
	return renderId;
}

Brush* Renderer3D::getBrushByRenderId(float renderId)
{
	auto res = m_brushMap->find(renderId);

	if (res != m_brushMap->end())
	{
		return res->second;
	}

	return nullptr;
}
