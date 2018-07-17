#include "Neuron.h"

using namespace nn;

Neuron::Neuron(const uint& x, const uint& y, const Activation& activation, const bool& bias)
	: x(x)
	, y(y)
	, bias(bias)
	, value(bias ? 1 : 0)
	, error(0)
	, activation(activation)
{
}

Neuron::~Neuron()
{
}
