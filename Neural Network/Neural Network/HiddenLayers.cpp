#include "HiddenLayers.h"

nn::HiddenLayers::HiddenLayers()
{
}

void nn::HiddenLayers::AddLayer(const uint& neuronsInLayer, const Activation& activationFunction)
{
	m_neurons.push_back(nn::NeuronBuffer(neuronsInLayer, (uint)m_neurons.size() + 1, activationFunction));
}

std::vector<nn::NeuronBuffer>& nn::HiddenLayers::GetArray()
{
	return m_neurons;
}

uint nn::HiddenLayers::GetSize() const
{
	return (uint)m_neurons.size();
}

nn::HiddenLayers::~HiddenLayers()
{
}
