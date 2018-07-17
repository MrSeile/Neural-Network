#pragma once

#include <SFML\Graphics.hpp>
#include "..\Neural Network\NeuralNetwork.h"

namespace nn
{
	void Draw(sf::RenderWindow& window, NeuralNetwork& nn, const nn::Vec2<float>& sPos, const float& scaleX, const float& scaleY, const float& radius, sf::Font& font, const bool& debug)
	{
		for (std::vector<Link*>& i : nn.GetLinks())
		{
			for (Link* l : i)
			{
				sf::Vector2f pos1;
				if (nn.HasBias() && l->back.x != nn.GetNeurons().size() - 1)
				{
					pos1.x = l->back.x * scaleX;
					pos1.y = (l->back.y * scaleY) - (scaleY * ((nn.GetNeurons()[l->back.x].size() - 2) / 2.f));
				}
				else
				{
					pos1.x = l->back.x * scaleX;
					pos1.y = (l->back.y * scaleY) - (scaleY * ((nn.GetNeurons()[l->back.x].size() - 1) / 2.f));
				}

				sf::Vector2f pos2;
				if (nn.HasBias() && l->front.x != nn.GetNeurons().size() - 1)
				{
					pos2.x = l->front.x * scaleX;
					pos2.y = (l->front.y * scaleY) - (scaleY * ((nn.GetNeurons()[l->front.x].size() - 2) / 2.f));
				}
				else
				{
					pos2.x = l->front.x * scaleX;
					pos2.y = (l->front.y * scaleY) - (scaleY * ((nn.GetNeurons()[l->front.x].size() - 1) / 2.f));
				}

				sf::Vertex line[] = { sf::Vertex({ pos1.x + sPos.x, pos1.y + sPos.y }), sf::Vertex({ pos2.x + sPos.x, pos2.y + sPos.y }) };

				sf::Uint8 color = (sf::Uint8)mapd(l->weight, -1, 1, 0, 255);
				line[0].color = sf::Color(255 - color, color, 0);
				line[1].color = sf::Color(255 - color, color, 0);

				window.draw(line, 2, sf::Lines);
			}
		}

		for (NeuronBuffer& i : nn.GetNeurons())
		{
			for (Neuron& j : i.GetArray())
			{
				sf::CircleShape circle(radius);
				circle.setOrigin(radius, radius);
				circle.setOutlineThickness(j.bias ? 2.f : 1.f);
				circle.setOutlineColor(j.bias ? sf::Color::Red : sf::Color::White);
				sf::Uint8 color = j.bias ? 255 : (sf::Uint8)mapd(j.value, j.activation.range.x, j.activation.range.y, 0, 255);
				circle.setFillColor(sf::Color(color, color, color));

				sf::Vector2f pos;

				if (nn.HasBias() && j.x != nn.GetNeurons().size() - 1)
				{
					pos.x = j.x * scaleX;
					pos.y = (j.y * scaleY) - (scaleY * ((i.size() - 2) / 2.f));
				}
				else
				{
					pos.x = j.x * scaleX;
					pos.y = (j.y * scaleY) - (scaleY * ((i.size() - 1) / 2.f));
				}
				circle.setPosition({ pos.x + sPos.x, pos.y + sPos.y });

				window.draw(circle);

				if (debug)
				{
					sf::Text text;
					text.setFont(font);
					text.setCharacterSize(radius);
					text.setPosition(circle.getPosition().x - radius / 2.f, circle.getPosition().y - radius / 2.f);
					text.setFillColor(sf::Color::White);
					text.setOutlineColor(sf::Color::Black);
					text.setOutlineThickness(1);

					if (!j.bias)
					{
						if (j.x == 0)
						{
							text.setString(std::to_string(j.value));
						}
						else if (j.x == nn.GetNeurons().size() - 1)
						{
							std::stringstream t;
							t << j.value << "\n" << j.error;
							text.setString(t.str());
						}
						else
						{
							text.setString(std::to_string(j.value));
						}
					}

					window.draw(text);
				}

			}
		}
	}
}