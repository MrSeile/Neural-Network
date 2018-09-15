#pragma once

#define m_BUTTON

#include "Global.h"
#include "UIObject.h"
#include "Interactuable.h"

namespace ui
{
	class Button : public ui::UIObject, public ui::Interactuable
	{
	private:
		std::function<void(ui::UIObject* self)> m_clickFunction;

		bool m_hasClickFuncion = false;
		bool m_able = true;

	public:
		// Constructor
		Button(const std::string& id);

		// Generals
		void CheckInput(const sf::RenderWindow& window, const sf::Event& e) override;
		void Update(const sf::RenderWindow& window) override;
		void Draw(sf::RenderWindow& window) override;

		// Set
		void SetClickFunction(const std::function<void(ui::UIObject* self)>& function);
		void SetAble(const bool& able);

		// Get
		bool GetAble();
		std::function<void(Button* self)> GetClickEvent();
		sf::Vector2f GetPosition();

		// Variables
		sf::Text text;
		sf::RectangleShape shape;
	};

}