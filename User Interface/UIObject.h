#pragma once

#include "Global.h"

namespace ui
{
	class UIObject
	{
	protected:
		std::function<void(UIObject* self)> m_updateFunction;
		std::function<void(UIObject* self)> m_beginPlayFunction;

		bool m_hasCustomUpdateFunction = false;
		bool m_hasCustomBeginPlayFunction = false;

	public:
		UIObject(const std::string& id);
		~UIObject();

		// Generals
		virtual void Update(const sf::RenderWindow& window);
		virtual void Draw(sf::RenderWindow& window) = 0;
		virtual void BeginPlay();

		// Set
		void SetUpdateFunction(const std::function<void(UIObject* self)>& function);
		void SetBeginPlayFunction(const std::function<void(UIObject* self)>& function);

		// Variables
		const std::string id;

	};
}
