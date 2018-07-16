#pragma once

#include "Global.h"
#include "Link.h"
#include "Neuron.h"
#include "NeuronBuffer.h"
#include "Vec2.h"

namespace nn
{
	struct Activation
	{
		Activation(const std::string& id, const nn::Vec2<double>& range, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate)
			: funct(funct)
			, derivate(derivate)
			, range(range)
			, id(id)
		{

		}

		std::function<double(const double& x)> funct;
		std::function<double(const double& x)> derivate;

		const nn::Vec2<double> range;
		const std::string id;
	};

	class NeuralNetwork
	{
	public:

	private:
		std::vector<NeuronBuffer> m_neurons;
		std::vector<std::vector<Link*>> m_links;
		uint m_count;
		bool m_bias;
		Activation m_actFunc;
		uint outputs;
		uint inputs;

	public:
		NeuralNetwork(const uint& inputNodes, const std::vector<uint>& hiddenLayers, const uint& outputNodes, const Activation& activationFunciton, const bool& bias = true);

		bool HasBias();
		const Activation GetActivation();
		std::vector<NeuronBuffer>& GetNeurons();
		std::vector<std::vector<Link*>>& GetLinks();
		uint GetTrainCount();

		std::vector<double> Calculate(const std::vector<double>& input);
		void Train(const std::vector<double>& input, const std::vector<double>& optimal, const double& lRate);
		void SaveToFile(const std::string& path);
		void LoadFromFile(const std::string& path);

		~NeuralNetwork();
	};
}
