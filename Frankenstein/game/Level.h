#pragma once

#include <string>

class Level
{
public:
	static Level& Load(std::string filename);
private:
	Level();
	~Level();
};
