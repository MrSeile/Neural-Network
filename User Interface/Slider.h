#pragma once

#define m_SLIDER

#include "Global.h"
#include "UIObject.h"
#include "Interactuable.h"

namespace ui
{
	class Slider : public ui::UIObject, public ui::Interactuable
	{
	private:
		bool m_pressed = false;
		float m_value = 0;
		float m_offset = 0;
		bool m_showValue = true;

		sf::RectangleShape m_slider;
		sf::Text m_text;
		sf::RectangleShape m_body;

	public:
		Slider(const std::string& id, const sf::Font& font);

		// General
		void CheckInput(const sf::RenderWindow& window, const sf::Event& e) override;
		void Update(const sf::RenderWindow& window) override;
		void Draw(sf::RenderWindow& window) override;

		// Set
		void SetValue(const float& value);
		void SetPosition(const sf::Vector2f& position);
		void SetPosition(const float& x, const float& y);
		void SetSize(const float& width, const float& heigth);
		void SetSize(const sf::Vector2f& size);

		void ShowValue();
		void HideValue();

		// Get
		sf::RectangleShape& GetBody();
		sf::RectangleShape& GetHandle();
		sf::Text& GetText();

		// Get
		float GetValue();
		bool IsPressed();
	};
}