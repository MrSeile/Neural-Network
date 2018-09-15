#pragma once

#define PI 3.1415926535

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>

typedef unsigned int uint;

namespace ui
{
	inline float map(const float& value, const float& inputMin, const float& inputMax, const float& outputMin, const float& outputMax)
	{
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
	}
}