#include <SFML\Graphics.hpp>
#include <NeuralNetwork.h>

struct Data
{
	Data(double pPos, double bPosX, double bPosY, double bVel, double bDir, double y, double maxVel, const sf::Vector2f& wSize, bool log = false)
		: pPos(pPos / wSize.y)
		, bPosX(bPosX / wSize.x)
		, bPosY(bPosY / wSize.y)
		, bVel(bVel / maxVel)
		, bDir(bDir)
		, y(y)
	{
		if (log)
		{
			//std::cout << std::fixed << std::setprecision(4) << this->pPos << " - " << this->bPosX << " - " << this->bPosY << " - " << this->bVel << " - " << this->bDir << " - " << this->y << "\n";
		}
	}

	double pPos;
	double bPosX;
	double bPosY;
	double bVel;
	double bDir;
	double y;
};

class Player
{
public:
	Player(const float& pos)
		: pos(50, pos)
		, size(20, 100)
	{
		shape.setSize(size);
		shape.setOrigin(size / 2.f);
		shape.setPosition(this->pos);
		shape.setFillColor(sf::Color::White);
	}

	void Draw(sf::RenderWindow& window, const sf::Color& color)
	{
		shape.setFillColor(color);
		window.draw(shape);
	}

	void Update(float& delta, const sf::RenderWindow& window)
	{
		pos.y += delta * 10;

		if (pos.y - size.y / 2.f < 0)
		{
			pos.y = 0 + size.y / 2.f;
			delta = 0;
		}
		if (pos.y + size.y / 2.f > window.getSize().y)
		{
			pos.y = window.getSize().y - size.y / 2.f;
			delta = 0;
		}

		shape.setPosition(pos);
	}

	sf::RectangleShape shape;
	sf::Vector2f pos;
	sf::Vector2f size;
};

class Ball
{
public:
	Ball(const sf::RenderWindow& window)
		: pos((sf::Vector2f)window.getSize() / 2.f)
		, vel(maxVel.x, randRange<float>(-10, 10))
		, size(20, 20)
		, maxVel(7, 10)
	{
		shape.setSize(size);
		shape.setOrigin(size / 2.f);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::White);
	}

	void Update(const sf::RenderWindow& window, const Player& p, const float& delta, nn::NeuralNetwork& nn, std::vector<Data>& data)
	{
		pos += vel;

		if (pos.y - size.y / 2.f < 0)
		{
			pos.y = 0 + size.y / 2.f;
			vel.y *= -1;
		}
		if (pos.y + size.y / 2.f > window.getSize().y)
		{
			pos.y = window.getSize().y - size.y / 2.f;
			vel.y *= -1;
		}
		if (pos.x + size.x / 2.f > window.getSize().x)
		{
			pos.x = window.getSize().x - size.x / 2.f;
			vel.x *= -1;
		}
		if (pos.x - size.x < p.pos.x + p.size.x / 2.f && pos.y - size.y / 2.f >= p.pos.y - p.size.y / 2.f && pos.y + size.y / 2.f <= p.pos.y + p.size.y / 2.f && pos.x - size.x / 2.f > p.pos.x)
		{
			std::cout << data.size() << "\n";

			for (Data& x : data)
			{
				nn.Train({ x.pPos, x.bPosX, x.bPosY, x.bVel, x.bDir }, { x.y }, 0.0001, 0.0);
			}

			data.clear();

			pos.x = p.pos.x + (p.size.x / 2.f) + (size.x / 2.f) + 5;
			vel.x *= -1;

			vel.y += delta * randRange<float>(1, 3);

			if (vel.y > maxVel.y)
				vel.y = maxVel.y;

			if (vel.y < -maxVel.y)
				vel.y = -maxVel.y;
		}
		if (pos.x + size.x < -100)
		{
			pos = (sf::Vector2f)window.getSize() / 2.f;
			vel = { maxVel.x, randRange<float>(-maxVel.y, maxVel.y) };
			data.clear();
		}

		if (count >= max)
		{
			count = 0;
			data.push_back(Data(p.pos.y, pos.x, pos.y, vel.y, vel.x / maxVel.x, delta, maxVel.y, (sf::Vector2f)window.getSize(), true));
		}

		count++;

		shape.setPosition(pos);
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	sf::RectangleShape shape;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f size;
	sf::Vector2f maxVel;

	int max = 5;
	int count = 1;
};

int main()
{
	srand((uint)time(NULL));

	sf::RenderWindow window({ 1000, 600 }, "Pong", sf::Style::Close);
	window.setFramerateLimit(60);

	Player p(window.getSize().y / 2.f);
	Player bot(window.getSize().y / 2.f);

	Ball b(window);

	std::vector<Data> data;

	nn::Activation hyper(
		"HyperbolicTangent",
		{ -1, 1 },
		[](const double& x)->double { return tanh(x); },
		[](const double& x)->double { return 1 - pow(x, 2); }
	);

	nn::NeuralNetwork nn(5, { 10, 10 }, 1, hyper);
		
	float delta = 0;

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			delta -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			delta += 1;
		}

		window.clear();


		Data d(bot.pos.y, b.pos.x, b.pos.y, b.vel.y, b.vel.x / b.maxVel.x, 0, b.maxVel.y, (sf::Vector2f)window.getSize());

		float y;

		double input = nn.Calculate({ d.pPos, d.bPosX, d.bPosY, d.bVel, d.bDir })[0];
		//std::cout << input << "\n";
		if (input <= -1.0 / 3.0)
			y = -1;

		else if (input <= 1.0 / 3.0)
			y = 0;

		else if (input <= 1.0)
			y = 1;

		bot.Update(y, window);
		bot.Draw(window, sf::Color::Red);

		p.Update(delta, window);
		p.Draw(window, sf::Color::White);

		b.Update(window, p, delta, nn, data);
		b.Draw(window);

		window.display();

		delta = 0;
	}
}