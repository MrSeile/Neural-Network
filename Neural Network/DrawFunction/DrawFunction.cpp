#include "DrawFunction.h"

inline float map(const float& value, const float& inputMin, const float& inputMax, const float& outputMin, const float& outputMax)
{
	return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
}

DrawFunction::DrawFunction(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& cellSize, const sf::Vector2f& inputRange, const sf::Vector2f& outputRange, const std::function<float(const sf::Vector2f& input)>& function)
{
	Create(pos, size, cellSize, inputRange, outputRange, function);
}

DrawFunction::DrawFunction()
{
}

void DrawFunction::Create(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& cellSize, const sf::Vector2f& inputRange, const sf::Vector2f& outputRange, const std::function<float(const sf::Vector2f& input)>& function)
{
	m_pixels.clear();

	m_cellSize = cellSize;
	m_func = function;
	m_inputRange = inputRange;
	m_outputRange = outputRange;
	m_size = size;

	for (float x = 0; x < size.x; x += cellSize.x)
	{
		for (float y = 0; y < size.y; y += cellSize.y)
		{
			sf::Vertex topLeft;
			sf::Vertex topRight;
			sf::Vertex bottomLeft;
			sf::Vertex bottomRight;

			float pixelX = pos.x + x;
			float pixelY = pos.y + y;

			topLeft.position =		{ pixelX,				pixelY };
			topRight.position =		{ pixelX + cellSize.x,	pixelY };
			bottomLeft.position =	{ pixelX,				pixelY + cellSize.x };
			bottomRight.position =	{ pixelX + cellSize.x,	pixelY + cellSize.y };

			m_pixels.push_back(topLeft);
			m_pixels.push_back(bottomLeft);
			m_pixels.push_back(bottomRight);
			m_pixels.push_back(topRight);
		}
	}
}


void DrawFunction::Draw(sf::RenderWindow& window)
{
	sf::Vector2i cells;
	cells.x = (int)(m_size.x / m_cellSize.x);
	cells.y = (int)(m_size.y / m_cellSize.y);

	for (float x = 0; x < cells.x; x++)
	{
		for (float y = 0; y < cells.y; y++)
		{
			int index = (x + cells.x * y) * 4;

			sf::Vector2f input;
			input.x = map(x, 0, cells.x, m_inputRange.x, m_inputRange.y);
			input.y = map(y, 0, cells.y, m_inputRange.x, m_inputRange.y);

			sf::Color color;
			color.r = map(m_func({ input.x, input.y }), m_outputRange.x, m_outputRange.y, 0, 255);
			color.g = map(m_func({ input.x, input.y }), m_outputRange.x, m_outputRange.y, 0, 255);
			color.b = map(m_func({ input.x, input.y }), m_outputRange.x, m_outputRange.y, 0, 255);

			m_pixels[index + 0].color = color;
			m_pixels[index + 1].color = color;
			m_pixels[index + 2].color = color;
			m_pixels[index + 3].color = color;
		}
	}

	window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
}

DrawFunction::~DrawFunction()
{
}
