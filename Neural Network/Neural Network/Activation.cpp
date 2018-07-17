#include "Activation.h"

nn::Activation::Activation(const std::string& id, const nn::Vec2<double>& range, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate)
	: funct(funct)
	, derivate(derivate)
	, range(range)
	, id(id)
{

}

nn::Activation::Activation()
	: range(0, 1)
	, id("none")
{

}