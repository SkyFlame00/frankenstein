#include "Scene.h"

Scene::Scene()
{	
}

Scene::~Scene()
{
}

void Scene::setup()
{
	//m_objects.append(new Brush);
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

