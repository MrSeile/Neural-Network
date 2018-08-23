#pragma once

#include <SFML\Graphics.hpp>
#include <Tools\NeuralNetworkGlobals.h>

class Grid
{
private:
	std::vector<sf::Vertex> m_pixels;
	sf::Vector2u size;
	sf::Vector2f cellSize;

	uint GetIndex(const sf::Vector2u& pos);

public:
	Grid(const sf::Vector2u& size, const sf::Vector2f& quadSize);

	void Clear(const sf::Color& color = sf::Color::Black);
	void SetPixel(const sf::Vector2u& pos, const sf::Color& color);
	void Draw(sf::RenderWindow& window);

	~Grid();
};

