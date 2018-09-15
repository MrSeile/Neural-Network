#include "Line.h"

void ui::Line::Reset()
{
	sf::Vector2f delta = m_fPos - m_iPos;

	float size = sqrt(pow(delta.x, 2) + pow(delta.y, 2));

	shape.setSize({ size, m_width });
	shape.setOrigin(0, shape.getSize().y / 2.f);
	shape.setPosition(m_iPos);

	float r = atan(delta.y / delta.x) * 180 / PI;
	shape.setRotation(r);
}

ui::Line::Line(const std::string& id, const sf::Vector2f& iPos, const sf::Vector2f& fPos)
	: UIObject(id)
	, m_width(3)
	, m_iPos(iPos)
	, m_fPos(fPos)
{
	shape.setFillColor(sf::Color::Black);

	Reset();
}

void ui::Line::Draw(sf::RenderWindow & window)
{
	window.draw(shape);
}

void ui::Line::SetWidth(const float & width)
{
	m_width = width;

	Reset();
}

void ui::Line::SetColor(const sf::Color& color)
{
	shape.setFillColor(color);
}

ui::Line::~Line()
{
}
