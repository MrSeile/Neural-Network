#include "NeuralNetwork.h"

using namespace nn;

// Constructor
NeuralNetwork::NeuralNetwork(const uint& inputNodes, const std::vector<uint>& hiddenLayers, const uint& outputNodes, const Activation& activationFunciton, const bool& bias)
	: inputs(inputNodes)
	, outputs(outputNodes)
	, m_bias(bias)
	, m_actFunc(activationFunciton)
	, m_count(0)
{
	std::cout << "Initializing neural network...\n";

	// Generate the structure
	std::cout << "Creating the neurons...\n";
	m_neurons.push_back(NeuronBuffer(inputNodes, 0));

	uint index = 1;
	for (const uint& x : hiddenLayers)
	{
		m_neurons.push_back(NeuronBuffer(x, index));
		index++;
	}

	m_neurons.push_back(NeuronBuffer(outputNodes, hiddenLayers.size() + 1));

	// Add the bias
	if (bias)
	{
		for (uint x = 0; x < m_neurons.size() - 1; x++)
		{
			m_neurons[x].PushBack(Neuron(x, m_neurons[x].size(), true));
		}
	}

	// Create the links
	std::cout << "Creating the links...\n";
	for (int x = 0; x < m_neurons.size(); x++)
	{
		if (x != m_neurons.size() - 1)
		{
			m_links.push_back(std::vector<Link*>());
		}

		for (uint y = 0; y < m_neurons[x].size(); y++)
		{
			if (x != m_neurons.size() - 1)
			{
				for (Neuron& next : m_neurons[x + 1].GetArray())
				{
					if (next.bias)
					{
						continue;
					}

					m_links[x].push_back(new Link(m_neurons[x][y], next));
				}
			}
		}
	}

	// Links the liks with its neightbour neurons
	for (std::vector<Link*>& lx : m_links)
	{
		for (Link* l : lx)
		{
			for (NeuronBuffer& nx : m_neurons)
			{
				for (Neuron& n : nx.GetArray())
				{
					if (l->back.x == n.x && l->back.y == n.y)
					{
						n.backLinks.push_back(l);
					}
					if (l->front.x == n.x && l->front.y == n.y)
					{
						n.frontLinks.push_back(l);
					}
				}
			}
		}
	}

	std::cout << "Ready neural network!\n\n";
}

// Save all the witghts into a file
void NeuralNetwork::SaveToFile(const std::string& path)
{
	std::cout << "Saving Neural Network...\n";

	std::fstream file;
	file.open(path, std::fstream::out | std::fstream::trunc);

	file << m_count << "\n";
	file << m_bias << "\n";
	file << m_actFunc.id << "\n";
	file << inputs << "\n";
	file << outputs << "\n";
	file << m_neurons.size() - 2 << "\n";
	
	for (uint i = 1; i < m_neurons.size() - 1; i++)
	{
		file << m_neurons[i].size() - 1 << "\n";
	}

	for (uint x = 0; x < m_links.size(); x++)
	{
		for (uint y = 0; y < m_links[x].size(); y++)
		{
			file << m_links[x][y]->weight << "\n";
		}
	}

	file.close();

	std::cout << "Saved!\n\n";
}

// Set all the witghts from file
void NeuralNetwork::LoadFromFile(const std::string& path)
{
	std::cout << "Loading Neural Network from file...\n";

	std::fstream file;
	file.open(path);
	int x = 0;
	int y = 0;

	uint count;
	bool match = false;
	bool _bias;
	std::string _af;
	uint _inputs;
	uint _outputs;
	uint _size;
	std::vector<uint> _hidden;
	uint i = 0;

	// Check if the structure of the nn is the same of the one in the file
	std::string val;
	for (int line = 1; file >> val; line++)
	{
		if (!match)
		{
			switch (line)
			{
			case 1:
				count = std::stoi(val);

			case 2:
				_bias = std::stoi(val);
				break;
			case 3:
				_af = val;
				break;
			case 4:
				_inputs = std::stoi(val);
				break;
			case 5:
				_outputs = std::stoi(val);
				break;
			case 6:
				_size = std::stoi(val);
				break;
			default:
				if (i < _size)
				{
					_hidden.push_back(std::stoi(val));
					i++;
				}
				else
				{
					if (m_bias == _bias && m_actFunc.id == _af && inputs == _inputs && outputs == _outputs && m_neurons.size() - 2 == _size && std::function<bool()>([&]()->bool
						{
							for (uint j = 0; j < _size; j++)
							{
								if (m_neurons[j + 1].size() - 1 != _hidden[j])
								{
									return false;
								}
							}
							return true;
						})())
					{
						match = true;
					}
					else
					{
						std::cout << "Load failed: Neural Network stucture do not match\n\n";
						return;
					}
				}
				break;
			}
		}
		// If the structure of the nn is the same of the one in the file, set the witghts
		if (match)
		{
			m_links[x][y]->weight = std::stod(val);

			y++;
			if (y == m_links[x].size())
			{
				x++;
				y = 0;
			}

		}
	}
	if (match)
	{
		m_count = count;
	}

	std::cout << "Loaded!\n\n";
}

// Returns if the nn has bias or not
bool NeuralNetwork::HasBias()
{
	return m_bias;
}

// Returns the activation function (temporal)
const nn::Activation NeuralNetwork::GetActivation()
{
	return m_actFunc;
}

std::vector<NeuronBuffer>& nn::NeuralNetwork::GetNeurons()
{
	return m_neurons;
}

std::vector<std::vector<Link*>>& nn::NeuralNetwork::GetLinks()
{
	return m_links;
}

uint nn::NeuralNetwork::GetTrainCount()
{
	return m_count;
}

// Guess an output for an input
std::vector<double> NeuralNetwork::Calculate(const std::vector<double>& input)
{
	// Check if the # of inputs match the nn # of inputs
	if (input.size() != inputs)
	{
		std::cout << "\n";
		std::cout << "========================\n";
		std::cout << "  Input size not match  \n";
		std::cout << "========================\n";
		std::cout << "\n";

		std::vector<double> output;

		for (uint i = 0; i < outputs; i++)
		{
			output.push_back(-1);
		}

		return output;
	}

	// Set the input neurons the inputs value
	for (uint i = 0; i < inputs; i++)
	{
		m_neurons[0][i].value = input[i];
	}

	// Feedforward
	for (uint x = 0; x < m_neurons.size() - 1; x++)
	{
		for (uint y = 0; y < m_neurons[x + 1].size(); y++)
		{
			if (m_neurons[x + 1][y].bias)
			{
				continue;
			}

			double value = 0;

			for (Link* l : m_links[x])
			{
				if (l->front.x == x + 1 && l->front.y == y)
				{
					value += (l->back.value * l->weight);
				}
			}

			m_neurons[x + 1][y].value = m_actFunc.funct(value);
		}
	}

	// Return the outputs
	std::vector<double> output;

	for (Neuron& n : m_neurons.back().GetArray())
	{
		output.push_back(n.value);
	}

	return output;
}

// Backpropagate
void NeuralNetwork::Train(const std::vector<double>& input, const std::vector<double>& optimal, const double& lRate)
{
	// Check if the # of inputs and outputs match the nn # of inputs and outputs
	if (input.size() != inputs || optimal.size() != outputs)
	{
		std::cout << "\n";
		std::cout << "==================================\n";
		std::cout << "  Input or output size not match  \n";
		std::cout << input.size() << " - " << inputs << "\n";
		std::cout << "==================================\n";
		std::cout << "\n";

		return;
	}

	// Make a guess
	std::vector<double> output = Calculate(input);

	// Calculate the errors
	for (uint i = 0; i < outputs; i++)
	{
		m_neurons.back()[i].error = optimal[i] - output[i];
	}

	for (uint x = (uint)m_neurons.size() - 2; x >= 1; x--)
	{
		for (uint y = 0; y < m_neurons[x].size(); y++)
		{
			if (m_neurons[x][y].bias)
			{
				continue;
			}

			m_neurons[x][y].error = 0;

			for (Link* l : m_neurons[x][y].backLinks)
			{
				m_neurons[x][y].error += l->weight * l->front.error;
			}
		}
	}

	// Adjust the weights of the links
	for (std::vector<Link*> lb : m_links)
	{
		for (Link* l : lb)
		{
			float delta = m_actFunc.derivate(l->front.value) * l->front.error * lRate * l->back.value;

			l->weight += delta;
		}
	}

	m_count++;
}

NeuralNetwork::~NeuralNetwork()
{

}
