#include "Link.h"

using namespace nn;

Link::Link(Neuron& n1, Neuron& n2)
	: back(n1)
	, front(n2)
{
	weight = randRange<double>(-1, 1);

	//std::cout << n1.x << ", " << n1.y << (n1.bias ? " (bias)" : "") << " -> " << n2.x << ", " << n2.y << " (" << weight << ")" << std::endl;
}

Link::~Link()
{

}
