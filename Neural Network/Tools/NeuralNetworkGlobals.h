#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include "..\Neural Network\Vec2.h"

typedef unsigned int uint;
#define mapf map<float>
#define mapd map<double>

template<typename T>
inline T map(const T& value, const T& inputMin, const T& inputMax, const T& outputMin, const T& outputMax)
{
	return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
}

template<typename T>
T randRange(const T& min, const T& max)
{
	return min + (rand() / (RAND_MAX / ((max + 0) - min)));
}