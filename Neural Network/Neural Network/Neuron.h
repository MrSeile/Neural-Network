#pragma once

#include "Global.h"

namespace nn
{
	class Link;

	class Neuron
	{
	public:
		Neuron(const uint& x, const uint& y, const bool& bias = false);

		const bool bias;

		double value;

		double error;

		const uint x;
		const uint y;

		std::vector<Link*> backLinks;
		std::vector<Link*> frontLinks;

		~Neuron();
	};
}