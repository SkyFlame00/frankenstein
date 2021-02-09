#include "Scene.h"

Scene::Scene()
{
	
}

void Scene::setup()
{
	m_objects.append(new Brush);
}

Scene::~Scene()
{
	/* REMOVE OBJECTS */
}

QList<Brush*>& Scene::getObjects()
{
	return m_objects;
}
