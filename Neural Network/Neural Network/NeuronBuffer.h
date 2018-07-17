#pragma once

#include "Global.h"
#include "Neuron.h"
#include "Activation.h"

namespace nn
{
	class NeuronBuffer
	{
	private:
		std::vector<Neuron> m_neurons;
		const uint m_size;

	public:
		NeuronBuffer(const uint& size, const uint& index, const nn::Activation& activation);
		~NeuronBuffer();

		uint size();
		std::vector<Neuron>& GetArray();
		void PushBack(const Neuron& newNeuron);

		Neuron& operator[] (std::size_t index)
		{
			return m_neurons[index];
		}
	};
}