#pragma once

#include <QList>

#include "Brush.h"

class Scene
{
public:
	Scene();
	~Scene();

	void setup();
	QList<Brush*>& getObjects();

private:
	QList<Brush*> m_objects;
};
