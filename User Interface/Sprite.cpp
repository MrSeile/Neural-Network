#include "Sprite.h"

ui::Sprite::Sprite(const std::string& id)
	: UIObject(id)
{

}

void ui::Sprite::Draw(sf::RenderWindow& window)
{
	window.draw(*this);
}
