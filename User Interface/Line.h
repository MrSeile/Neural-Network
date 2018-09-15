#pragma once

#include "Global.h"
#include "UIObject.h"

namespace ui {

	class Line : public ui::UIObject
	{
	private:
		float m_width;
		sf::Vector2f m_iPos;
		sf::Vector2f m_fPos;

		sf::RectangleShape shape;

		void Reset();

	public:
		Line(const std::string& id, const sf::Vector2f& iPos, const sf::Vector2f& fPos);

		void Draw(sf::RenderWindow& window) override;
		void SetWidth(const float& width);
		void SetColor(const sf::Color& color);

		~Line();
	};
}
