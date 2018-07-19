#include "NeuronBuffer.h"

using namespace nn;

NeuronBuffer::NeuronBuffer(const uint& size, const uint& index, const nn::Activation& activation)
	: m_size(size)
	, m_activation(activation)
{
	for (uint i = 0; i < m_size; i++)
	{
		m_neurons.push_back(Neuron(index, i, activation));
	}
}

uint NeuronBuffer::size()
{
	return (uint)m_neurons.size();
}

std::vector<Neuron>& NeuronBuffer::GetArray()
{
	return m_neurons;
}

Activation nn::NeuronBuffer::GetActivation()
{
	return m_activation;
}

void NeuronBuffer::PushBack(const Neuron& newNeuron)
{
	m_neurons.push_back(newNeuron);
}

NeuronBuffer::~NeuronBuffer()
{
}