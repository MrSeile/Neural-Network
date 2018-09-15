#include <NeuralNetwork.h>
#include "..\User Interface\Widget.h"
#include <Tools\Application.h>


namespace nn
{
	void Draw(sf::RenderWindow& window, NeuralNetwork& nn, const nn::Vec2<float>& sPos, const float& scaleX, const float& scaleY, const float& radius)
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

				ui::Line line("1", { pos1.x + sPos.x, pos1.y + sPos.y }, { pos2.x + sPos.x, pos2.y + sPos.y });

				double c = tanh(l->weight / 20);

				/*double r = c < 0 ? 1 : 1 - c;
				double g = c > 0 ? 1 : 1 + c;
				double b = 1 - abs(c);

				sf::Color color((sf::Uint8)map<double>(r, 0, 1, 0, 255), (sf::Uint8)map<double>(g, 0, 1, 0, 255), (sf::Uint8)map<double>(b, 0, 1, 0, 255));*/

				line.SetWidth(map<float>(c, -1, 1, 8, 15));

				//line.SetColor(color);

				line.Draw(window);
			}
		}

		for (NeuronBuffer& i : nn.GetNeurons())
		{
			for (Neuron& j : i.GetArray())
			{
				float r = map<float>(j.value, 0, 1, radius - 10, radius);

				sf::CircleShape circle(r);
				circle.setOrigin(r, r);
				circle.setFillColor(sf::Color::Black);

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
			}
		}
	}
}


int main()
{
	srand((uint)time(NULL));

	std::vector<std::vector<double>> input =	{ { 0, 0 }	, { 0, 1 }	, { 1, 0 }	, { 1, 1 }	};
	std::vector<std::vector<double>> output =	{ { 0 }		, { 1 }		, { 1 }		, { 0 }		};

	nn::Activation sigmoid(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x)))); }
	);

	nn::Activation sigmoid2(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return 1; }
	);

	nn::NeuralNetwork nn(2, { 3 }, 1, sigmoid, false);
	nn.LoadFromFile("C:\\Users\\elies\\Desktop\\save.txt");

	std::cout << "           ---------------------\n";
	std::cout << "           |    0    |    1    |\n";
	std::cout << " -------------------------------\n";
	std::cout << " |    0    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " |\n";
	std::cout << " -------------------------------\n";
	std::cout << " |    1    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << " |\n";
	std::cout << " -------------------------------\n\n";

	for (auto& b : nn.GetLinks())
	{
		for (auto x : b)
		{
			x->weight *= 10;
		}
	}

	std::cout << "           ---------------------\n";
	std::cout << "           |    0    |    1    |\n";
	std::cout << " -------------------------------\n";
	std::cout << " |    0    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " |\n";
	std::cout << " -------------------------------\n";
	std::cout << " |    1    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << " |\n";
	std::cout << " -------------------------------\n\n";

	nn.Calculate({ 0, 1 });
	Application* app;

	double lRate = 0.1;
	double dropout = 0;

	bool train = false;
	bool quit = false;

	sf::Font font;
	font.loadFromFile("font/font.ttf");

	/////////////////////////
	// Console loop
	/////////////////////////
	std::thread consoleThread([&]()
	{
		while (!quit)
		{
			std::string cmd;
			std::cin >> cmd;

			if (cmd == "quit")
			{
				quit = true;
				app->End();
			}
			else if (cmd == "set")
			{
				if (cmd == "lRate")
				{
					app->Execute([&]()
					{
						std::cin >> lRate;
						std::cout << "Learning rate set to " << lRate << "\n\n";
					});
				}
				else if (cmd == "dropout")
				{
					app->Execute([&]()
					{
						std::cin >> dropout;
						std::cout << "Dropout set to " << dropout << "\n\n";
					});
				}
			}
			else if (cmd == "get")
			{
				std::cin >> cmd;
				if (cmd == "lRate")
				{
					std::cout << lRate << "\n\n";
				}
				else if (cmd == "count")
				{
					std::cout << nn.GetTrainCount() << "\n\n";
				}
				else if (cmd == "dropout")
				{
					std::cout << dropout << "\n\n";
				}
				else if (cmd == "structure")
				{
					auto struc = nn.GetNeurons();
					bool bias = nn.HasBias();
					std::cout << "bias: " << (bias ? "true" : "false") << "\n";
					std::cout << "i: " << struc[0].size() - bias << "\n";
					for (uint i = 1; i < struc.size() - 1; i++)
					{
						std::cout << i << ": " << struc[i].size() - bias << " " << struc[i].GetActivation().id << "\n";
					}
					std::cout << "o: " << struc.back().size() << " " << struc.back().GetActivation().id << "\n\n";
				}
			}
			else if (cmd == "train")
			{
				std::cin >> cmd;
				if (cmd == "true")
				{
					app->Execute([&]()
					{
						if (!train)
						{
							train = true;
							std::cout << "Started training\n\n";
						}
					});
				}
				else
				{
					app->Execute([&]()
					{
						if (train)
						{
							train = false;
							std::cout << "Stoped training\n\n";
						}
					});
				}
			}
			else if (cmd == "save")
			{
				app->Execute([&]() { nn.SaveToFile("C:\\Users\\elies\\Desktop\\save.txt"); });
			}
			else if (cmd == "load")
			{
				app->Execute([&]() { nn.LoadFromFile("C:\\Users\\elies\\Desktop\\save.txt"); });
			}
			else if (cmd == "test")
			{
				std::cin >> cmd;

				if (cmd == "all")
				{
					app->Execute([&]()
					{
						std::cout << "           ---------------------\n";
						std::cout << "           |    0    |    1    |\n";
						std::cout << " -------------------------------\n";
						std::cout << " |    0    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " |\n";
						std::cout << " -------------------------------\n";
						std::cout << " |    1    | " << std::fixed << std::setprecision(5) << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << " |\n";
						std::cout << " -------------------------------\n\n";
					});
				}
			}
			else
			{
				system(cmd.c_str());
			}
		}
	});

	/////////////////////////
	// Train loop
	/////////////////////////
	std::thread trainThread([&]()
	{
		app = new Application([&]()
		{
			if (train)
			{
				uint index = (rand() % (int)(3 + 1));
				nn.Train(input[index], output[index], lRate, dropout);
			}
		});

		app->Init();
	});

	/////////////////////////
	// Window thread
	/////////////////////////
	std::thread windowThread = std::thread([&]()
	{
		sf::ContextSettings stgs;
		stgs.antialiasingLevel = 16;

		sf::RenderWindow window({ 1200, 800 }, "", sf::Style::Close, stgs);
		while (window.isOpen())
		{
			if (quit)
			{
				window.close();
			}

			sf::Event e;
			while (window.pollEvent(e))
			{
				//ui.CheckInput(window, e);
			}

			window.clear(sf::Color::White);

			//ui.Update(window);
			//ui.Draw(window);

			app->Execute([&]()
			{
				nn::Draw(window, nn, { 100, 400 }, 340, 320, 60);
			});

			window.display();
		}
	});

	trainThread.join();
	consoleThread.join();
	windowThread.join();
}