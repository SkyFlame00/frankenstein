#include "Scene.h"

Scene::Scene()
{	
}

Scene::~Scene()
{
}

void Scene::setup()
{
}

QList<Brush*>& Scene::getObjects()
{
	return m_objects;
}

void Scene::addObject(Brush* brush)
{
	m_objects.push_back(brush);
}

void Scene::removeObject(Brush* brush)
{
	m_objects.removeOne(brush);
}

bool Scene::contains(Brush* brush)
{
	for (auto obj : m_objects)
		if (obj == brush)
			return true;
	return false;
}

void Scene::clear()
{
	for (auto obj : m_objects)
		delete obj;
	m_objects.clear();
}
