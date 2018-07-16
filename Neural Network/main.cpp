/*#include "Neural Network\NeuralNetwork.h"
#include "Draw Neural Network\DrawNeuralNetwork.h"
#include <SFML\Graphics.hpp>

int main()
{
	nn::Activation sigmoid(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return x * (1 - x); }
	);

	nn::NeuralNetwork nn(3, { 5, 5 }, 2, sigmoid);
	nn.Calculate({ randRange<double>(0, 1), randRange<double>(0, 1) , randRange<double>(0, 1) });

	sf::RenderWindow window(sf::VideoMode(1600, 700), "");
	sf::Font font;
	font.loadFromFile("font/font.ttf");

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

		nn::Draw(window, nn, { 100, 300 }, 200, 100, 10, font, false);

		window.display();
	}
}

/*#include "Neural Network\NeuralNetwork.h"
#include "DrawFunction\DrawFunction.h"
#include <thread>

/*double output(sf::Vector2<double> i)
{
	if ((i.x > 0 && i.y > 0) || (i.x < 0 && i.y < 0))
	{
		return 1;
	}
	else
	{
		return -1;
	}
	//double y = -4 * (i.x - 0.03) * (i.x - 0.47) * (i.x - 0.7) + 0.5;
	//return pow(1 - abs(y - i.y), 10);
}


int main()
{
	std::cout << "Initializing data..." << std::endl;

	srand((uint)time(NULL));

	NeuralNetwork::Activation sigmoid({ 0, 1 },
	[](const double& x)->double
	{
		return 1.f / (1.f + exp(-x));
	},
	[](const double& x)->double
	{
		return x * (1 - x);
	});

	NeuralNetwork::Activation hyper({ -1, 1 },
	[](const double& x)->double
	{
		return tanh(x);
	},
	[](const double& x)->double
	{
		return 1 - pow(x, 2);
	});

	namespace fs = std::experimental::filesystem;

	int max = 200;

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

	std::cout << "Data ready!" << std::endl;

	NeuralNetwork nn(28 * 28, { 500, 500, 400, 300 }, 10, sigmoid);


	///////////////////////
	// SFML stuff
	///////////////////////
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	sf::RenderWindow window;

	sf::View view;

	/*DrawFunction df;
	df.Create({ 200, 200 }, { 600, 600 }, { 4, 4 }, (sf::Vector2f)nn.GetActivation().range, (sf::Vector2f)nn.GetActivation().range, [&](sf::Vector2f inputs)->float
	{
		return (float)nn.Calculate({ inputs.x, inputs.y })[0];
	});
	
	DrawFunction optimal;
	optimal.Create({ 700, -200 }, { 400, 400 }, { 1, 1 }, (sf::Vector2f)nn.GetActivation().range, (sf::Vector2f)nn.GetActivation().range, [](sf::Vector2f inputs)->float
	{
		return output((sf::Vector2<double>)inputs);
	});
	
	///////////////////////
	// Global variables
	///////////////////////
	double lRate = 0.005;
	
	bool train = true;
	bool training = false;
	bool draw = false;
	bool debug = false;
	bool done = false;
	bool quit = false;
	bool testD = false;
	int count = 0;

	uint maxN = 10;

	/////////////////////////
	// Manage the console
	/////////////////////////
	std::thread consoleThread([&]()
	{
		while (!quit)
		{
			std::string temp;
			std::cin >> temp;
			if (temp == "quit")
			{
				quit = true;
				draw = false;
				done = false;
			}
			else if (temp == "show" && !draw)
			{
				draw = true;
				done = false;
			}
			else if (temp == "hide" && draw)
			{
				draw = false;
				done = false;
			}
			else if (temp == "lRate")
			{
				std::cin >> lRate;
			}
			else if (temp == "get")
			{
				std::cin >> temp;
				if (temp == "lRate")
				{
					std::cout << lRate << std::endl;
				}
				if (temp == "count")
				{
					std::cout << count << std::endl;
				}
			}
			else if (temp == "save")
			{
				nn.SaveToFile("C:\\Users\\elies\\Desktop\\save.txt");
			}
			else if (temp == "load")
			{
				nn.LoadFromFile("C:\\Users\\elies\\Desktop\\save.txt");
			}
			else if (temp == "test")
			{
				train = false;
				testD = true;
			}
			else
			{
				system(temp.c_str());
			}
		}
	});


	while (!quit)
	{
		if (draw && !done)
		{
			window.create(sf::VideoMode(600, 600), "Neural Network", sf::Style::Default, settings);

			view.setSize((sf::Vector2f)window.getSize());
			window.setView(view);

			done = true;
		}
		if (!draw && !done)
		{
			window.close();
			done = true;
		}

		if (draw && done)
		{
			sf::Event e;
			while (window.pollEvent(e))
			{
				if (e.type == sf::Event::Resized)
				{
					view.setSize((sf::Vector2f)window.getSize());
					view.setCenter((window.getSize().x / 2.f) - 50, 0);
					window.setView(view);
				}
				if (e.type == sf::Event::KeyPressed)
				{
					if (e.key.code == sf::Keyboard::D)
					{
						debug = debug ? 0 : 1;
					}
					if (e.key.code == sf::Keyboard::Up)
					{
						lRate += 0.0001;
						std::cout << lRate << std::endl;
					}
					if (e.key.code == sf::Keyboard::Down && lRate >= 0.0001)
					{
						lRate -= 0.0001;
						std::cout << lRate << std::endl;
					}
				}
			}
			window.clear();

			if (debug)
			{
				//nn.Draw(window, 100, 50, 10, false);
			}
			//optimal.Draw(window);
			//df.Draw(window);

			window.display();
		}


		//std::vector<double> i = { randRange<double>(-1, 1), randRange<double>(-1, 1) };
		//std::vector<double> o = { output({ i[0], i[1] }) };

		if (train)
		{
			uint n = (rand() % static_cast<int>(maxN - 1 + 1));
			uint index = (rand() % static_cast<int>(max - 1 + 1));

			nn.Train(data[n][index], optimal[n], lRate);

			count++;

			if (!train)
			{
				for (int n = 0; n < maxN; n++)
				{
					std::vector<double> output = nn.Calculate(test[n]);

					for (int i = 0; i < 10; i++)
					{
						std::cout << std::fixed << std::setprecision(8) << output[i] << " ";
					}
					std::cout << std::endl;
				}

				train = true;
			}
		}
	}

	consoleThread.join();
}
#endif*/