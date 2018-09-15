#include "Widget.h"

ui::Widget::Widget()
{

}

ui::Widget::~Widget()
{
}

// Other
void ui::Widget::Clear()
{
	m_objects.clear();
}

void ui::Widget::AddObject(ui::UIObject* newObject)
{
	m_objects.push_back(newObject);
}

ui::UIObject * ui::Widget::GetUIObject(const std::string& id)
{
	for (ui::UIObject* x : m_objects)
	{
		if (!x)
			continue;

		if (x->id == id)
		{
			return x;
		}
	}
	return nullptr;
}

void ui::Widget::Draw(sf::RenderWindow& window)
{
	for (ui::UIObject* o : m_objects)
	{
		o->Draw(window);
	}
}

void ui::Widget::BeginPlay()
{
	for (ui::UIObject* o : m_objects)
	{
		o->BeginPlay();
	}
}

void ui::Widget::CheckInput(const sf::RenderWindow& window, const sf::Event& e)
{
	for (ui::UIObject* o : m_objects)
	{
		auto i = dynamic_cast<ui::Interactuable*>(o);

		if (i)
		{
			i->CheckInput(window, e);
		}
	}
}

void ui::Widget::Update(const sf::RenderWindow& window)
{
	for (ui::UIObject* o : m_objects)
	{
		o->Update(window);
	}
}