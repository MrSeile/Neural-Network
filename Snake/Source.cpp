#if 0
#include <NeuralNetwork.h>
#include <SFML\Graphics.hpp>
#include "Grid.h"

#define bot 0

enum class Direction
{
	None,
	Up,
	Down,
	Right,
	Left
};

enum class Object
{
	None,
	Player,
	Bonus
};

struct Cell
{
	Direction dir = Direction::None;
	Direction prev = Direction::None;
	Object obj = Object::None;
};

void Reset(std::array<std::array<Cell, 20>, 20>& grid, std::vector<sf::Vector2i>& snake, Direction& dir, Direction& input)
{
	for (int x = 0; x < grid.size(); x++)
	{
		for (int y = 0; y < grid[0].size(); y++)
		{
			grid[x][y].dir = Direction::None;
			grid[x][y].prev = Direction::None;
			grid[x][y].obj = Object::None;
		}
	}

	snake = { { 4, 10 }, { 3, 10 }, { 2, 10 }, { 1, 10 } };
	input = Direction::Right;
	dir = input;

	for (sf::Vector2i& pos : snake)
	{
		grid[pos.x][pos.y].dir = input;
		grid[pos.x][pos.y].prev = Direction::Right;
	}
}

void ResetBonus(sf::Vector2i& bonus, std::array<std::array<Cell, 20>, 20>& grid)
{
	bonus = { (rand() % static_cast<int>(grid.size())), (rand() % static_cast<int>(grid.size())) };

	while (grid[bonus.x][bonus.y].obj == Object::Player)
	{
		bonus = { (rand() % static_cast<int>(grid.size())), (rand() % static_cast<int>(grid.size())) };
	}

	grid[bonus.x][bonus.y].obj = Object::Bonus;
}

int main()
{
	srand((uint)time(NULL));

	std::array<std::array<Cell, 20>, 20> grid;

	sf::RenderWindow window({ 600, 600 }, "Snake", sf::Style::Close);

	// Draw
	sf::Vector2f quadSize(window.getSize().x / (float)grid.size(), window.getSize().y / (float)grid[0].size());
	Grid pixels({ (uint)grid.size(), (uint)grid[0].size() }, quadSize);

	// Player
	std::vector<sf::Vector2i> snake;
	Direction input;
	Direction dir;

	Reset(grid, snake, dir, input);
	
	sf::Vector2i bonus;
	ResetBonus(bonus, grid);

	nn::Activation hyper(
		"HyperbolicTangent",
		{ -1, 1 },
		[](const double& x)->double { return tanh(x); },
		[](const double& x)->double { return 1 - pow(x, 2); }
	);

	// Inputs: obstacle left, obstacle right, obstacle front, angle to bonus
	nn::NeuralNetwork nn(4, { 10, 10 }, 0, hyper);
	nn.LoadFromFile("C:\\Users\\elies\\Desktop\\snake.txt");

	sf::Clock delay;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
#if 0
				std::string str;
				std::cin >> str;

				if (str == "save")
				{
					nn.SaveToFile("C:\\Users\\elies\\Desktop\\snake.txt");
				}
#endif

				window.close();
			}
#if !bot 
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Up && dir != Direction::Down)
				{
					input = Direction::Up;
				}
				if (e.key.code == sf::Keyboard::Down && dir != Direction::Up)
				{
					input = Direction::Down;
				}
				if (e.key.code == sf::Keyboard::Right && dir != Direction::Left)
				{
					input = Direction::Right;
				}
				if (e.key.code == sf::Keyboard::Left && dir != Direction::Right)
				{
					input = Direction::Left;
				}
			}
#endif
		}

		if (delay.getElapsedTime().asSeconds() > 0.2f)
		{
			// dir / input

			grid[snake[0].x][snake[0].y].prev = dir;
			dir = input;
			grid[snake[0].x][snake[0].y].dir = dir;

			for (int i = 0; i < snake.size(); i++)
			{
				sf::Vector2i pos = snake[i];
				sf::Vector2i newPos = pos;

				switch (grid[pos.x][pos.y].dir)
				{
				case Direction::Up:
					newPos.y -= 1;
					break;

				case Direction::Down:
					newPos.y += 1;
					break;

				case Direction::Right:
					newPos.x += 1;
					break;

				case Direction::Left:
					newPos.x -= 1;
					break;

				default:
					
					break;
				}

				if ((grid[newPos.x][newPos.y].obj == Object::Player && newPos != snake.back()) || newPos.x < 0 || newPos.x >= grid.size() || newPos.y < 0 || newPos.y >= grid[0].size())
				{
					std::cout << "helo\n";
					Reset(grid, snake, dir, input);
					ResetBonus(bonus, grid);
					break;
				}
				else if (grid[newPos.x][newPos.y].obj == Object::Bonus)
				{
					sf::Vector2i newCell = snake.back();

					switch (grid[newCell.x][newCell.y].prev)
					{
					case Direction::Up:
						newCell.y += 1;
						break;

					case Direction::Down:
						newCell.y -= 1;
						break;

					case Direction::Right:
						newCell.x -= 1;
						break;

					case Direction::Left:
						newCell.x += 1;
						break;

					default:

						break;
					}

					snake.push_back(newCell);

					ResetBonus(bonus, grid);
				}

				grid[pos.x][pos.y].obj = Object::None;
				grid[newPos.x][newPos.y].obj = Object::Player;

				snake[i] = newPos;
			}

			delay.restart();

			std::cout << snake.size() << "\n";
		}

		pixels.Clear();

		for (sf::Vector2i& pos : snake)
		{
			pixels.SetPixel((sf::Vector2u)pos, sf::Color::White);
		}
		pixels.SetPixel((sf::Vector2u)bonus, sf::Color::Red);

		pixels.Draw(window);

		window.display();
	}
}
#endif