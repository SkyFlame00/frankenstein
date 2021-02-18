#pragma once

#include <QList>
#include <QVector3D>
#include "Renderable.h"
#include "Brush.h"

class Scene
{
public:
	Scene();
	~Scene();

	void setup();
	QList<Brush*>& getObjects();

	QList<Renderable*> m_gui2DObjects;
	QList<Renderable*> m_gui3DObjects;

private:
	/* Maybe it's better to implement with set instead of list. It's faster to remove from a set */
	QList<Brush*> m_objects;
};
