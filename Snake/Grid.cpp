#include "Grid.h"

uint Grid::GetIndex(const sf::Vector2u& pos)
{
	return (pos.x * size.x + pos.y) * 4;
}

Grid::Grid(const sf::Vector2u& size, const sf::Vector2f& cellSize)
	: size(size)
	, cellSize(cellSize)
{
	for (float x = 0; x < size.x; x++)
	{
		for (float y = 0; y < size.y; y++)
		{
			sf::Vertex topLeft;
			sf::Vertex topRight;
			sf::Vertex bottomLeft;
			sf::Vertex bottomRight;

			float pixelX = x * cellSize.x;
			float pixelY = y * cellSize.y;

			topLeft.position =		{ pixelX,				pixelY };
			topRight.position =		{ pixelX + cellSize.x,	pixelY };
			bottomLeft.position =	{ pixelX,				pixelY + cellSize.x };
			bottomRight.position =	{ pixelX + cellSize.x,	pixelY + cellSize.y };

			topLeft.color =		sf::Color::Black;
			topRight.color =	sf::Color::Black;
			bottomLeft.color =	sf::Color::Black;
			bottomRight.color = sf::Color::Black;

			m_pixels.push_back(topLeft);
			m_pixels.push_back(bottomLeft);
			m_pixels.push_back(bottomRight);
			m_pixels.push_back(topRight);
		}
	}
}

void Grid::Clear(const sf::Color& color)
{
	for (sf::Vertex& x : m_pixels)
	{
		x.color = color;
	}
}

void Grid::SetPixel(const sf::Vector2u& pos, const sf::Color& color)
{
	if (pos.x >= size.x || pos.x < 0 || pos.y >= size.y || pos.y < 0)
	{
		return;
	}

	uint index = GetIndex(pos);

	for (int i = 0; i < 4; i++)
	{
		m_pixels[index + i].color = color;
	}
}

void Grid::Draw(sf::RenderWindow& window)
{
	window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
}


Grid::~Grid()
{
}
