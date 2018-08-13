#pragma once

#include "Tools/NeuralNetworkGlobals.h"
#include "Neural Network/Link.h"
#include "Neural Network/Neuron.h"
#include "Neural Network/NeuronBuffer.h"
#include "Neural Network/HiddenLayers.h"
#include "Neural Network/Activation.h"

namespace nn
{
	class NeuralNetwork
	{
	public:

	private:
		std::vector<NeuronBuffer> m_neurons;
		std::vector<std::vector<Link*>> m_links;
		uint m_count;
		bool m_bias;
		uint outputs;
		uint inputs;

	public:
		NeuralNetwork(const uint& inputNodes, const std::vector<uint>& hiddenLayers, const uint& outputNodes, const Activation& activationFunciton, const bool& bias = true);
		NeuralNetwork(const uint& inputNodes, nn::HiddenLayers& hiddenLayers, const uint& outputNodes, const Activation& outputActivation, const bool& bias = true);

		bool HasBias();
		std::vector<NeuronBuffer>& GetNeurons();
		std::vector<std::vector<Link*>>& GetLinks();
		uint GetTrainCount();

		std::vector<double> Calculate(const std::vector<double>& input);
		void Train(const std::vector<double>& input, const std::vector<double>& optimal, const double& lRate, const double& dropout);
		void SaveToFile(const std::string& path);
		void LoadFromFile(const std::string& path);

		~NeuralNetwork();
	};
}