#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class DrawFunction
{
private:
	std::vector<sf::Vertex> m_pixels;
	sf::Vector2f m_size;
	sf::Vector2f m_cellSize;
	sf::Vector2f m_inputRange;
	sf::Vector2f m_outputRange;

	std::function<float(const sf::Vector2f& input)> m_func;

public:
	DrawFunction(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& cellSize, const sf::Vector2f& inputRange, const sf::Vector2f& outputRange, const std::function<float(const sf::Vector2f& input)>& function);
	DrawFunction();

	void Create(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& cellSize, const sf::Vector2f& inputRange, const sf::Vector2f& outputRange, const std::function<float(const sf::Vector2f& input)>& function);
	void Draw(sf::RenderWindow& window);


	~DrawFunction();
};

