#include <NeuralNetwork.h>
#include ""
#include <SFML\Graphics.hpp>
#include "Grid.h"

#define bot 0

#define sX 10
#define sY 10

struct Data
{
	enum
	{
		fObstacle = 0,
		rObstacle,
		lObstacle,
		playerDir,
		playerX,
		playerY,
		playerSize,
		bonusX,
		bonusY,
		input,
		lenght
	};

	std::array<double, Data::lenght> data;

	double& operator[] (std::size_t index)
	{
		return data[index];
	}
};

enum Direction
{
	Right = 0,
	Up = 1,
	Left = 2,
	Down = 3
};

enum class Object
{
	None,
	Player,
	Bonus
};

struct Cell
{
	int dir = Right;
	int prev = Right;
	Object obj = Object::None;
};

void Reset(std::array<std::array<Cell, sY>, sX>& grid, std::vector<sf::Vector2i>& snake, int& dir, int& input)
{
	for (int x = 0; x < sX; x++)
	{
		for (int y = 0; y < sY; y++)
		{
			grid[x][y].dir = Right;
			grid[x][y].prev = Right;
			grid[x][y].obj = Object::None;
		}
	}

	snake = { { 4, sY / 2 }, { 3, sY / 2 }, { 2, sY / 2 } };
	input = 0;
	dir = input;

	for (sf::Vector2i& pos : snake)
	{
		grid[pos.x][pos.y].dir = input;
		grid[pos.x][pos.y].prev = Direction::Right;
	}
}

void ResetBonus(sf::Vector2i& bonus, std::array<std::array<Cell, sY>, sX>& grid)
{
	bonus = { (rand() % static_cast<int>(sX)), (rand() % static_cast<int>(sY)) };

	while (grid[bonus.x][bonus.y].obj == Object::Player)
	{
		bonus = { (rand() % static_cast<int>(sX)), (rand() % static_cast<int>(sY)) };
	}

	grid[bonus.x][bonus.y].obj = Object::Bonus;
}

// Inputs: obstacle front, obstacle right, obstacle left, player X, player Y, player dir, bonus X, bonus Y, angle to bonus
std::vector<double> getInput(Data& data)
{
	std::array<double, 9> output;

	sf::Vector2<double> delta = { data[Data::bonusX] - data[Data::playerX], data[Data::bonusY] - data[Data::playerY] };
	double angle = atan2(delta.y, delta.x) * 180 / PI;

	if (angle < 0)
		angle = 360 + angle;
	
	output[0] = data[Data::fObstacle];
	output[1] = data[Data::rObstacle];
	output[2] = data[Data::lObstacle];
	output[3] = data[Data::playerX];
	output[4] = data[Data::playerY];
	output[5] = map<double>(data[Data::playerDir], 0, 3, -1, 1);
	output[6] = data[Data::bonusX];
	output[7] = data[Data::bonusY];
	output[8] = map<double>(angle, 0, 360, -1, 1);

	return std::vector<double>(output.begin(), output.end());
}

int main()
{
	srand((uint)time(NULL));

	std::array<std::array<Cell, sY>, sX> grid;

	// Draw
	sf::RenderWindow window({ 600, 650 }, "Snake", sf::Style::Close);
	sf::Vector2f quadSize(window.getSize().x / (float)sX, window.getSize().x / (float)sY);
	Grid pixels({ (uint)sX, (uint)sY }, quadSize);

	// Player
	std::vector<sf::Vector2i> snake;
	int input;
	int dir;

	Reset(grid, snake, dir, input);

	sf::Vector2i bonus;
	ResetBonus(bonus, grid);

	sf::Font font;
	font.loadFromFile("font/font.ttf");

#if bot
	double val;

	nn::Activation hyper
	(
		"HyperbolicTangent",
		{ -1, 1 },
		[](const double& x)->double { return tanh(x); },
		[](const double& x)->double { return 1 - pow(x, 2); }
	);

	// Inputs: player X, player Y, player dir, bonus X, bonus Y
	nn::NeuralNetwork nn(9, { 100, 100, 50 }, 1, hyper);

	std::fstream file;
	file.open("C:\\snake\\sendToElies.txt", std::fstream::in);

	std::cout << "Collecting data...\n";
	std::vector<Data> data;
	Data d;
	std::string str;
	int count = 0;
	while (file >> str)
	{
		d[count] = std::stod(str);

		count++;

		if (count == Data::lenght)
		{
			data.push_back(d);
			count = 0;
		}
	}
	std::shuffle(data.begin(), data.end(), std::default_random_engine{});

	std::cout << "Data collected!\n\n";

	std::cout << "Training neural network...\n";
	for (Data& x : data)
	{
		nn.Train(getInput(x), { x[Data::input] }, 0.00001, 0.25);
	}
	std::cout << "Neural network trained!\n\n";

#else
	std::vector<Data> data;

	std::fstream file;
	file.open("C:\\snake\\sendToElies.txt", std::fstream::out | std::fstream::app);
#endif

	sf::Clock delay;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
#if !bot 
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Up)
				{
					input = 0;

					switch (dir)
					{
					case Right:
						input = 1;
						break;

					case Left:
						input = -1;
						break;
					}
				}
				if (e.key.code == sf::Keyboard::Down)
				{
					input = 0;

					switch (dir)
					{
					case Right:
						input = -1;
						break;

					case Left:
						input = 1;
						break;
					}
				}
				if (e.key.code == sf::Keyboard::Right)
				{
					input = 0;

					switch (dir)
					{
					case Up:
						input = -1;
						break;

					case Down:
						input = 1;
						break;
					}
				}
				if (e.key.code == sf::Keyboard::Left)
				{
					input = 0;

					switch (dir)
					{
					case Up:
						input = 1;
						break;

					case Down:
						input = -1;
						break;
					}
				}
			}
#endif
		}

		if (delay.getElapsedTime().asSeconds() > 0.2f)
		{
			sf::Vector2i p = snake[0];

			// Get the coords for the up, right and left segmets
			std::array<sf::Vector2i, 3> frl;
			switch (dir)
			{
			case Up:
				frl[Up] =		{ p.x, p.y - 1 };
				frl[Right] =	{ p.x + 1, p.y };
				frl[Left] =		{ p.x - 1, p.y };
				break;

			case Down:
				frl[Up] =		{ p.x, p.y + 1 };
				frl[Right] =	{ p.x - 1, p.y };
				frl[Left] =		{ p.x + 1, p.y };
				break;

			case Left:
				frl[Up] =		{ p.x - 1, p.y };
				frl[Right] =	{ p.x, p.y - 1 };
				frl[Left] =		{ p.x, p.y + 1 };
				break;

			case Right:
				frl[Up] =		{ p.x + 1, p.y };
				frl[Right] =	{ p.x, p.y + 1 };
				frl[Left] = 	{ p.x, p.y - 1 };
				break;
			}

			Data i;
			for (int j = 0; j < 3; j++)
			{
				double x = -1;

				if (frl[j].x >= sX || frl[j].x < 0 || frl[j].y >= sY || frl[j].y < 0)
				{
					x = 1;
				}
				else if (grid[frl[j].x][frl[j].y].obj == Object::Player)
				{
					x = 1;
				}

				switch (j)
				{
				case Up:
					i[Data::fObstacle] = x;
					break;

				case Right:
					i[Data::rObstacle] = x;
					break;

				case Left:
					i[Data::lObstacle] = x;
					break;
				}
			}

			i[Data::playerX] = p.x / (double)(sX - 1);
			i[Data::playerY] = p.y / (double)(sY - 1);

			i[Data::playerSize] = snake.size();

			i[Data::playerDir] = dir;

			i[Data::bonusX] = bonus.x / (double)(sX - 1);
			i[Data::bonusY] = bonus.y / (double)(sY - 1);

			i[Data::input] = input;

			//i.grid = gridToArray(grid);

			data.push_back(i);

			// Set the previous direction of the position
			grid[snake[0].x][snake[0].y].prev = dir;
#if bot
			// Calculate the movement of the Neural Network
			val = nn.Calculate(getInput(i))[0];
			if (val <= -1.0 / 3.0)
				input = -1;

			else if (val <= 1.0 / 3.0)
				input = 0;

			else if (val <= 1.0)
				input = 1;
#endif
			// Adjust the direction depending on the input
			dir += input;

			if (dir < 0)
			{
				dir = 3;
			}
			else if (dir > 3)
			{
				dir = 0;
			}
			grid[snake[0].x][snake[0].y].dir = dir;

			// Move each snake peace acording to the direction of the position
			bool collision = false;
			for (int i = 0; i < snake.size(); i++)
			{
				sf::Vector2i pos = snake[i];
				sf::Vector2i newPos = pos;

				// Change the position
				switch (grid[pos.x][pos.y].dir)
				{
				case Up:
					newPos.y -= 1;
					break;

				case Down:
					newPos.y += 1;
					break;

				case Right:
					newPos.x += 1;
					break;

				case Left:
					newPos.x -= 1;
					break;
				}
				
				if (newPos.x < 0 || newPos.x >= sX || newPos.y < 0 || newPos.y >= sY)
				{
					collision = true;
					break;
				}
				else if ((grid[newPos.x][newPos.y].obj == Object::Player && newPos != snake.back()))
				{
					collision = true;
					break;
				}
				else if (grid[newPos.x][newPos.y].obj == Object::Bonus)
				{
#if !bot
					for (Data& d : data)
					{
						for (int i = 0; i < Data::lenght; i++)
						{
							file << std::fixed << std::setprecision(30) << d[i] << "\n";
						}
						/*for (double& x : d.grid)
						{
							file << std::fixed << std::setprecision(30) << x << "\n";
						}*/
					}
					data.clear();
#endif
					sf::Vector2i newCell = snake.back();

					switch (grid[newCell.x][newCell.y].prev)
					{
					case Up:
						newCell.y += 1;
						break;

					case Down:
						newCell.y -= 1;
						break;

					case Right:
						newCell.x -= 1;
						break;

					case Left:
						newCell.x += 1;
						break;
					}

					snake.push_back(newCell);

					ResetBonus(bonus, grid);
				}

				grid[pos.x][pos.y].obj = Object::None;
				grid[newPos.x][newPos.y].obj = Object::Player;

				snake[i] = newPos;
			}

			if (collision)
			{
				std::cout << "Score: " << snake.size() << "\n\n";
#if !bot
				data.clear();
#endif
				Reset(grid, snake, dir, input);
				ResetBonus(bonus, grid);
			}

			delay.restart();
			input = 0;
		}

		window.clear();

		pixels.Clear();

		grid[bonus.x][bonus.y].obj = Object::Bonus;

		for (sf::Vector2i& pos : snake)
		{
			pixels.SetPixel((sf::Vector2u)pos, sf::Color::White);
		}
		pixels.SetPixel((sf::Vector2u)bonus, sf::Color::Red);

		pixels.Draw(window);

		sf::Vertex line[] = { sf::Vertex({ 0, 600 }), sf::Vertex({ 600, 600 }) };
		line[0].color = sf::Color::White;
		line[1].color = sf::Color::White;

		sf::Text text;
		std::stringstream str;
		str << std::fixed << std::setprecision(3) <<
			"SCORE: " << snake.size() <<
			"\nINPUT: " <<
#if bot 
			val
#else 
			input
#endif 
			;

		text.setFont(font);
		text.setString(str.str());
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(11);
		text.setPosition(10, 610);
		window.draw(text);

		window.draw(line, 2, sf::Lines);

		window.display();
	}
}