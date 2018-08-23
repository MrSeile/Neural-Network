#include <NeuralNetwork.h>
#include <Tools\Application.h>
#include <cmath>

int main()
{
	srand((uint)time(NULL));

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

	nn::NeuralNetwork nn(1, { 5, 5, 5 }, 1, hyper);
	Application* app;

	double lRate = 0.01;
	double dropout = 0.0;

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
				app->Execute([&]() { std::cout << nn.Calculate({ std::stod(cmd) / 360.0 })[0] << "\n\n"; });
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
				int x = (rand() % (int)(36 + 1));
				double y = cos((x * 10) * PI / 180.f);
				nn.Train({ (x * 10) / 360.0 }, { y }, lRate, dropout);
			}
		});

		app->Init();
	});

	trainThread.join();
	consoleThread.join();
}