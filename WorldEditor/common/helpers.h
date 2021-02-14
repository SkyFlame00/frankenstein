#pragma once

#include <cmath>

namespace Helpers
{

	float trunc(float num, int digits = 0)
	{
		float factor = std::pow(10, digits);
		num = std::trunc(num * factor);
		return num / factor;
	}

}
