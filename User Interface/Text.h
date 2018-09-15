#pragma once

#include "Global.h"
#include "UIObject.h"

namespace ui
{

	class Text : public sf::Text, public ui::UIObject
	{
	public:
		// Constructor
		Text(const std::string& id);

		// Generals
		void Draw(sf::RenderWindow& window) override;
	};
}