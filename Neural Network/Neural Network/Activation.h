#pragma once

#include "..\Tools\NeuralNetworkGlobals.h"

namespace nn
{
	struct Activation
	{
		Activation(const std::string& id, const nn::Vec2<double>& range, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate);
		
		Activation();

		std::function<double(const double& x)> funct;
		std::function<double(const double& x)> derivate;

		const nn::Vec2<double> range;
		const std::string id;
	};
}