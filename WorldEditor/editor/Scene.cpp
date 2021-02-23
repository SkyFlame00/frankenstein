#include "Scene.h"

Scene::Scene()
{	
}

Scene::~Scene()
{
}

void Scene::setup()
{
	m_objects.append(new Brush);
}

QList<Brush*>& Scene::getObjects()
{
	return m_objects;
}
