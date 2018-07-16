#pragma once

#include "Global.h"
#include "Neuron.h"

namespace nn
{
	class Link
	{
	public:
		Link(Neuron& n1, Neuron& n2);

		Neuron& back;
		Neuron& front;

		double weight;

		~Link();
	};
}