#include "UIObject.h"



ui::UIObject::UIObject(const std::string& id)
	: id(id)
{
}


ui::UIObject::~UIObject()
{
}

void ui::UIObject::Update(const sf::RenderWindow& window)
{
	if (m_hasCustomUpdateFunction)
	{
		m_updateFunction(this);
	}
}

void ui::UIObject::BeginPlay()
{
	if (m_hasCustomBeginPlayFunction)
	{
		m_beginPlayFunction(this);
	}
}

void ui::UIObject::SetUpdateFunction(const std::function<void(UIObject* self)>& function)
{
	m_hasCustomUpdateFunction = true;
	m_updateFunction = function;
}

void ui::UIObject::SetBeginPlayFunction(const std::function<void(UIObject* self)>& function)
{
	m_hasCustomBeginPlayFunction = true;
	m_beginPlayFunction = function;
}
