#include "Neuron.h"

using namespace nn;

Neuron::Neuron(const uint& x, const uint& y, const bool& bias)
	: x(x)
	, y(y)
	, bias(bias)
	, value(bias ? 1 : 0)
	, error(0)
{
}

Neuron::~Neuron()
{
}
