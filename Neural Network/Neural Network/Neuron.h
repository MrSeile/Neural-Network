#pragma once

#include "..\Tools\NeuralNetworkGlobals.h"
#include "Activation.h"

namespace nn
{
	class Link;

	class Neuron
	{
	public:
		Neuron(const uint& x, const uint& y, const Activation& activation, const bool& bias = false);

		nn::Activation activation;

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