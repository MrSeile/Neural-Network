#pragma once

#include "..\Tools\NeuralNetworkGlobals.h"
#include "NeuronBuffer.h"

namespace nn
{
	class HiddenLayers
	{
	private:
		std::vector<NeuronBuffer> m_neurons;

	public:
		HiddenLayers();

		void AddLayer(const uint& neuronsInLayer, const Activation& activationFunction);

		std::vector<NeuronBuffer>& GetArray();
		uint GetSize() const;

		NeuronBuffer& operator[] (std::size_t index)
		{
			return m_neurons[index];
		}

		~HiddenLayers();
	};
}
