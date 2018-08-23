#pragma once

#include <SFML\Graphics.hpp>
#include "NeuralNetworkGlobals.h"

class DrawFunction
{
private:
	std::vector<sf::Vertex> m_pixels;
	nn::Vec2<float> m_size;
	nn::Vec2<float> m_cellSize;
	nn::Vec2<float> m_inputRange;
	nn::Vec2<float> m_outputRange;

	std::function<float(const nn::Vec2<float>& input)> m_func;

public:
	DrawFunction(const nn::Vec2<float>& pos, const nn::Vec2<float>& size, const nn::Vec2<float>& cellSize, const nn::Vec2<float>& inputRange, const nn::Vec2<float>& outputRange, const std::function<float(const nn::Vec2<float>& input)>& function);
	DrawFunction();

	void Create(const nn::Vec2<float>& pos, const nn::Vec2<float>& size, const nn::Vec2<float>& cellSize, const nn::Vec2<float>& inputRange, const nn::Vec2<float>& outputRange, const std::function<float(const nn::Vec2<float>& input)>& function);
	void Draw(sf::RenderWindow& window);


	~DrawFunction();
};

