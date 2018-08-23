#include "Link.h"

using namespace nn;

Link::Link(Neuron& n1, Neuron& n2)
	: back(n1)
	, front(n2)
	, weight(randRange<double>(-2, 2))
	, delta(0)
{
}

Link::~Link()
{

}
