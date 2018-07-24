#pragma once

#include <iostream>
#include <vector>

typedef unsigned int uint;

namespace nn
{
	// Applies a filter to an image. It reduces a pixel the output image
	std::vector<double> ApplyFilter(const std::vector<double>& filter, const std::vector<double> source, const uint& sourceX, const uint& sourceY)
	{
		auto get = [](const int& x, const int& y, const int& xMax)->int
		{
			return x + xMax * y;
		};

		std::vector<double> newI;

		int fMaxX = (int)sqrt(filter.size());

		int n = (fMaxX - 1) / 2;

		double total = 0;
		for (const double& i : filter)
		{
			total += abs(i);
		}

		for (uint y = n; y < sourceY - n; y++)
		{
			for (uint x = n; x < sourceX - n; x++)
			{
				double sum = 0;
				for (int i = -n; i <= n; i++)
				{
					for (int j = -n; j <= n; j++)
					{
						sum += source[get(x + i, y + j, sourceX)] * filter[get(i + n, j + n, fMaxX)];
					}
				}
				newI.push_back(sum / total);
			}
		}

		return newI;
	}
}