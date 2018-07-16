#include "Neural Network\NeuralNetwork.h"
#include "Application.h"
#include <SFML\Graphics.hpp> // Load Images

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

			for (uint x = 0; x < 28; x++)
			{
				for (uint y = 0; y < 28; y++)
				{
					data[n][count].push_back(map<double>(img.getPixel(x, y).r, 0, 255, 0, 1));
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

			for (uint x = 0; x < 28; x++)
			{
				for (uint y = 0; y < 28; y++)
				{
					test[n].push_back(map<double>(img.getPixel(x, y).r, 0, 255, 0, 1));
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

	nn::NeuralNetwork nn(28 * 28, { 500, 500, 400, 300 }, 10, sigmoid);
	Application* app;

	/////////////////////////
	// Global variables
	/////////////////////////
	double lRate = 0.0003;

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
			else if (cmd == "lRate")
			{
				app->Execute([&]() 
				{
					std::cin >> lRate;
					std::cout << "Learning rate set to " << lRate << "\n\n"; 
				});
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
				app->Execute([&](){ nn.SaveToFile("C:\\Users\\elies\\Desktop\\save.txt"); });
			}
			else if (cmd == "load")
			{
				app->Execute([&](){ nn.LoadFromFile("C:\\Users\\elies\\Desktop\\save.txt"); });
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

					for (uint x = 0; x < img.getSize().x; x++)
					{
						for (uint y = 0; y < img.getSize().y; y++)
						{
							temp.push_back(map<double>(img.getPixel(x, y).r, 0, 255, 0, 1));
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
				nn.Train(data[n][index], optimal[n], lRate);
			}
		});

		app->Init();
	});

	/*/////////////////////////
	// Window loop
	/////////////////////////
	std::thread windowThread([&]()
	{
		sf::RenderWindow window(sf::VideoMode(300, 300), "");

		while (!quit)
		{
			sf::Event e;
			while (window.pollEvent(e))
			{

			}

			window.clear();



			window.display();
		}
	});*/

	trainThread.join();
	consoleThread.join();
	//windowThread.join();
}