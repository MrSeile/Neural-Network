#include "NeuronBuffer.h"

using namespace nn;

NeuronBuffer::NeuronBuffer(const uint& size, const uint& index)
	: m_size(size)
{
	for (uint i = 0; i < m_size; i++)
	{
		m_neurons.push_back(Neuron(index, i));
	}
}

uint NeuronBuffer::size()
{
	return m_neurons.size();
}

std::vector<Neuron>& NeuronBuffer::GetArray()
{
	return m_neurons;
}

void NeuronBuffer::PushBack(const Neuron& newNeuron)
{
	m_neurons.push_back(newNeuron);
}

NeuronBuffer::~NeuronBuffer()
{
}