#include <NeuralNetwork.h>
#include <Tools\ConvolutionalTools.h>
#include <Tools\Application.h>
#include <SFML\Graphics\Image.hpp> // Load Images
#include <SFML\Graphics.hpp>

int main()
{
	srand((uint)time(NULL));

	std::cout << "Initializing data...\n";

	namespace fs = std::experimental::filesystem;

	int max = 1000;

	// Teaining data
	// num / index / data
	std::vector<std::vector<std::vector<double>>> data;

	for (int n = 0; n < 10; n++)
	{
		data.push_back(std::vector<std::vector<double>>());

		std::string path = "C:\\Users\\elies\\Desktop\\mnist_png\\training\\";
		path += std::to_string(n);

		int count = 0;
		for (auto& p : fs::directory_iterator(path))
		{
			if (count >= max)
			{
				break;
			}

			data[n].push_back(std::vector<double>());

			sf::Image img;
			img.loadFromFile(p.path().string());

			for (uint y = 0; y < 28; y++)
			{
				for (uint x = 0; x < 28; x++)
				{
					data[n][count].push_back(map<double>(img.getPixel(x, y).r, 0, 255, -1, 1));
				}
			}

			count++;
		}
	}

	// Testing data
	std::vector<std::vector<double>> test;

	for (int n = 0; n < 10; n++)
	{
		test.push_back(std::vector<double>());

		std::string path = "C:\\Users\\elies\\Desktop\\mnist_png\\testing\\";
		path += std::to_string(n);

		for (auto& p : fs::directory_iterator(path))
		{
			sf::Image img;
			img.loadFromFile(p.path().string());

			for (uint y = 0; y < 28; y++)
			{
				for (uint x = 0; x < 28; x++)
				{
					test[n].push_back(map<double>(img.getPixel(x, y).r, 0, 255, -1, 1));
				}
			}

			break;
		}
	}

	std::vector<std::vector<double>> optimal =
	{
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	};

	std::cout << "Data ready!\n\n";

	nn::Activation sigmoid(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return x * (1 - x); }
	);

	nn::Activation hyper(
		"HyperbolicTangent",
		{ -1, 1 },
		[](const double& x)->double { return tanh(x); },
		[](const double& x)->double { return 1 - pow(x, 2); }
	);

	/////////////////////////
	// Global variables
	/////////////////////////
	nn::NeuralNetwork nn(784, { 200, 200, 100, 50 }, 10, sigmoid);
	Application* app;

	double lRate = 0.001;
	double dropout = 0.2;

	bool train = true;
	bool quit = false;

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
				if (cmd == "count")
				{
					std::cout << nn.GetTrainCount() << "\n\n";
				}
				else if (cmd == "dropout")
				{
					std::cout << dropout << "\n\n";
				}
				if (cmd == "structure")
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
						std::cout << "           -----------------------------------------------------------------------------------------------------\n";
						std::cout << "          ";
						for (int n = 0; n < 10; n++)
						{
							std::cout << " |    " << n << "   ";
						}
						std::cout << " |\n";

						for (int n = 0; n < 10; n++)
						{
							std::cout << " ---------------------------------------------------------------------------------------------------------------\n";

							std::vector<double> output = nn.Calculate(test[n]);

							std::cout << " |    " << n << "    | ";

							for (int i = 0; i < 10; i++)
							{
								std::cout << std::fixed << std::setprecision(5) << output[i] << " | ";
							}
							std::cout << "\n";
						}
						std::cout << " ---------------------------------------------------------------------------------------------------------------\n\n";
					});
				}
				else if (cmd == "path")
				{
					std::cin >> cmd;

					std::vector<double> temp;

					sf::Image img;
					img.loadFromFile(cmd);

					for (uint y = 0; y < img.getSize().y; y++)
					{
						for (uint x = 0; x < img.getSize().x; x++)
						{
							temp.push_back(map<double>(img.getPixel(x, y).r, 0, 255, -1, 1));
						}
					}

					app->Execute([&]()
					{
						std::vector<double> output = nn.Calculate(temp);

						for (double x : output)
						{
							std::cout << std::fixed << std::setprecision(5) << x << " ";
						}
						std::cout << "\n\n";
					});
				}
				else if (cmd == "draw")
				{
					app->Execute([&]()
					{
						bool pressing = false;

						sf::RenderWindow window(sf::VideoMode(504, 504), "", sf::Style::Close);
						while (window.isOpen())
						{
							sf::Event e;
							while (window.pollEvent(e))
							{
								if (e.type == sf::Event::KeyPressed)
								{
									if (e.key.code == sf::Keyboard::Escape)
									{
										window.close();
									}
									if (e.key.code == sf::Keyboard::Space)
									{
										window.clear();
									}
									if (e.key.code == sf::Keyboard::Return)
									{
										std::vector<double> temp;

										sf::Texture text;
										text.create(504, 504);
										text.update(window);

										sf::Image img = text.copyToImage();

										std::fstream file;
										file.open("C:\\Users\\elies\\Desktop\\save1.txt", std::fstream::out | std::fstream::trunc);

										for (int y = 0; y < 28; y++)
										{
											for (int x = 0; x < 28; x++)
											{
												int xx = (int)((x / 27.f) * 503);
												int yy = (int)((y / 27.f) * 503);

												double val = map<double>(img.getPixel(xx, yy).r, 0, 255, -1, 1);
												file << val << "\n";
												temp.push_back(val);
											}
										}
										file.close();
										window.close();

										auto output = nn.Calculate(temp);

										std::cout << " -----------------------------------------------------------------------------------------------------\n";
										for (int n = 0; n < 10; n++)
										{
											std::cout << " |    " << n << "   ";
										}
										std::cout << " |\n";
										std::cout << " -----------------------------------------------------------------------------------------------------\n";
										std::cout << " | ";
										for (int i = 0; i < 10; i++)
										{
											std::cout << std::fixed << std::setprecision(5) << output[i] << " | ";
										}
										std::cout << "\n";
										std::cout << " -----------------------------------------------------------------------------------------------------\n";
										std::cout << "\n\n";
									}
								}
								if (e.type == sf::Event::MouseButtonPressed)
								{
									if (e.mouseButton.button == sf::Mouse::Left)
									{
										pressing = true;
									}
								}
								if (e.type == sf::Event::MouseButtonReleased)
								{
									if (e.mouseButton.button == sf::Mouse::Left)
									{
										pressing = false;
									}
								}
							}

							if (pressing)
							{
								sf::Vector2i pos = sf::Mouse::getPosition(window);

								if (pos.x <= 504 && pos.y <= 504 && pos.x >= 0 && pos.y >= 0)
								{
									sf::CircleShape circle;
									circle.setRadius(30);
									circle.setOrigin(30, 30);
									circle.setFillColor(sf::Color::White);
									circle.setPosition((sf::Vector2f)pos);

									window.draw(circle);
								}
							}

							window.display();
						}
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
			uint n = (rand() % static_cast<int>(10 - 1 + 1));
			uint index = (rand() % static_cast<int>(max - 1 + 1));

			if (train)
			{
				nn.Train(data[n][index], optimal[n], lRate, dropout);
			}
		});

		app->Init();
	});

	trainThread.join();
	consoleThread.join();
}