#include <NeuralNetwork.h>
#include <Tools\DrawNeuralNetwork.h>
#include <Tools\DrawFunction.h>
#include <Tools\Application.h>

int main()
{
	srand((uint)time(NULL));

	std::vector<std::vector<double>> input =	{ { 0, 0 }	, { 0, 1 }	, { 1, 0 }	, { 1, 1 }	};
	std::vector<std::vector<double>> output =	{ { 0 }		, { 1 }		, { 1 }		, { 0 }		};

	nn::Activation sigmoid(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return x * (1 - x); }
	);

	nn::NeuralNetwork nn(2, { 4, 4 }, 1, sigmoid);
	Application* app;

	std::thread windowThread([&]() {});

	double lRate = 0.1;
	double dropout = 0.25;

	bool train = true;
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
			else if (cmd == "show")
			{
				std::cout << "\n";

				windowThread.detach();
				windowThread = std::thread([&]()
				{
					DrawFunction df({ 500, 300 }, { 200, 200 }, { 4, 4 }, { 0, 1 }, { 0, 1 }, [&](const nn::Vec2<float>& i)->float
					{
						return (float)nn.Calculate({ i.x, i.y })[0];
					});

					sf::RenderWindow window({ 1200, 600 }, "");
					while (window.isOpen())
					{
						sf::Event e;
						while (window.pollEvent(e))
						{
							if (e.type == sf::Event::Closed)
							{
								window.close();
							}
						}

						window.clear();

						app->Execute([&]()
						{
							nn::Draw(window, nn, { 50, 300 }, 100, 80, 10, font, true);
							df.Draw(window);
						});

						window.display();
					}
				});
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

	trainThread.join();
	consoleThread.join();
	windowThread.join();

	/*DrawFunction df({ 500, 300 }, { 200, 200 }, { 4, 4 }, { 0, 1 }, { 0, 1 }, [&](const sf::Vector2f& i)->float
	{
	return (float)nn.Calculate({ i.x, i.y })[0];
	});

	bool draw = true;

	sf::RenderWindow window({ 1200, 600 }, "");
	while (window.isOpen())
	{
	sf::Event e;
	while (window.pollEvent(e))
	{
	if (e.type == sf::Event::Closed)
	{
	window.close();
	}
	if (e.type == sf::Event::KeyPressed)
	{
	if (e.key.code == sf::Keyboard::D)
	{
	draw = draw ? false : true;
	}
	}
	}

	window.clear();

	uint index = (rand() % static_cast<int>(3 + 1));
	nn.Train(input[index], output[index], 0.1, 0);
	if (draw)
	{
	nn::Draw(window, nn, { 50, 300 }, 100, 80, 10, font, true);
	df.Draw(window);
	}

	window.display();
	}*/
}