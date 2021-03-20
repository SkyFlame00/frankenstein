#pragma once

#include "qopengl.h"
#include <QPixmap>

class Texture
{
public:
	GLuint id;
	int width;
	int height;
	QImage image;
};
