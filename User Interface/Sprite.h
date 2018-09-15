#pragma once

#include "Global.h"
#include "UIObject.h"


namespace ui
{
	class Sprite : public sf::Sprite, public ui::UIObject
	{
	public:
		Sprite(const std::string& id);

		void Draw(sf::RenderWindow& window) override;
	};
}