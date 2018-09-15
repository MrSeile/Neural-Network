#include "Text.h"

ui::Text::Text(const std::string& id)
	: UIObject(id)
{
}

void ui::Text::Draw(sf::RenderWindow& window)
{
	window.draw(*this);
}
