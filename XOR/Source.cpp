#include <NeuralNetwork.h>
#include <Tools\DrawNeuralNetwork.h>
#include <Tools\DrawFunction.h>

int main()
{
	srand((uint)time(NULL));

	std::vector<std::vector<double>> input = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };
	std::vector<std::vector<double>> output = { { 0 }, { 1 }, { 1 }, { 0 } };

	nn::Activation sigmoid(
		"Sigmoid",
		{ 0, 1 },
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return x * (1 - x); }
	);

	nn::NeuralNetwork nn(2, { 3 }, 1, sigmoid);
	sf::Font font;
	font.loadFromFile("font/font.ttf");

	DrawFunction df({ 500, 300 }, { 200, 200 }, { 1, 1 }, { 0, 1 }, { 0, 1 }, [&](const sf::Vector2f& i)->float
	{
		return nn.Calculate({ i.x, i.y })[0];
	});

	bool draw = false;

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
		nn.Train(input[index], output[index], 0.1);
		if (draw)
		{
			nn::Draw(window, nn, { 50, 300 }, 100, 80, 10, font, true);
			df.Draw(window);
		}

		window.display();
	}
}