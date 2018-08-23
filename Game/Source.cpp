#include <NeuralNetwork.h>
#include <SFML\Graphics.hpp>

struct Data
{
	Data(double pPos, double pVel, double oPos, double oSize, double y, bool log = false)
		: pPos(pPos / 600.0)
		, pVel(tanh(pVel))
		, oPos(oPos / 1000.0)
		, oSize(oSize / 600.0)
		, y(y)
	{
		if (log)
		{
			std::cout << std::fixed << std::setprecision(3) << this->pPos << " - " << this->pVel << " - " << this->oPos << " - " << this->oSize << " - " << y << "\n";
		}
	}

	double pPos;
	double pVel;
	double oPos;
	double oSize;
	double y;
};

class Obstacle
{
public:
	sf::RectangleShape top;
	sf::RectangleShape bottom;

	float pos;
	float size;
	float gap = 200;

	Obstacle()
		: pos(1200)
	{
		size = randRange<float>(gap / 2.f + 50, 600 - gap / 2.f - 50);

		top.setPosition(pos, 0);
		top.setSize({ 50, size - gap / 2.f });
		top.setFillColor(sf::Color::White);

		bottom.setPosition(pos, 600);
		bottom.setSize({ 50, -(600 - size - gap / 2.f) });
		bottom.setFillColor(sf::Color::White);
	}

	void Update(std::vector<Data>& data, nn::NeuralNetwork& nn)
	{
		if (pos < -100)
		{
			pos = 1200;
			size = randRange<float>(gap / 2.f + 50, 600 - gap / 2.f - 50);
			std::cout << data.size() << "\n";

			for (Data& d : data)
			{
				nn.Train({ d.pPos, d.pVel, d.oPos, d.oSize }, { d.y }, 0.0001, 0.0);
			}

			data.clear();
		}

		pos -= 5;

		top.setPosition(pos, 0);
		top.setSize({ 50, size - gap / 2.f });

		bottom.setPosition(pos, 600);
		bottom.setSize({ 50, -(600 - size - gap / 2.f) });
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(top);
		window.draw(bottom);
	}
};

class Player
{
public:
	Player(bool bot)
		: pos(300)
		, vel(0)
		, acc(0)
		, bot(bot)
	{
		shape.setSize(size);
		shape.setOrigin(shape.getSize().x / 2, shape.getSize().x / 2);
		shape.setFillColor(bot ? sf::Color::Red : sf::Color::White);
	}
	
	bool bot;
	float pos;
	float vel;
	float acc;
	sf::Vector2f size = { 30, 30 };
	sf::RectangleShape shape;

	void ApplyForce(const float& force)
	{
		acc += force / 3;
	}

	void Update(Obstacle& o, std::vector<Data>& data)
	{
		acc += 0.8f;
		vel += acc;
		pos += vel;

		if (pos + size.y / 2.f > 600)
		{
			pos = 600 - size.y / 2.f;
			vel = 0;
		}
		if (pos - size.y / 2.f < 0)
		{
			pos = 0 + size.y / 2.f;
			vel = 0;
		}

		acc = 0;

		shape.setPosition(100, pos);

		if (!bot)
		{
			if (o.top.getGlobalBounds().intersects(shape.getGlobalBounds()) || o.bottom.getGlobalBounds().intersects(shape.getGlobalBounds()))
			{
				o.size = randRange<float>(o.gap / 2.f + 50, 600 - o.gap / 2.f - 50);
				o.pos = 1200;
				data.clear();
			}
		}
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
};

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

	std::vector<Data> data;

	nn::HiddenLayers hidden;
	hidden.AddLayer(30, hyper);
	hidden.AddLayer(30, hyper);
	hidden.AddLayer(30, hyper);

	// Inputs: player pos, player vel, obstacle pos, obstacle size
	nn::NeuralNetwork nn(4, hidden, 1, sigmoid);

	sf::RenderWindow window({ 1000, 600 }, "Game");
	window.setFramerateLimit(60);

	Player p(false);
	Player b(true);
	Obstacle o;

	bool train = false;

	int count = 0;
	int max = 4;

	while (window.isOpen())
	{
		bool press = false;

		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::S)
				{
					nn.SaveToFile("C:\\Users\\elies\\Desktop\\save1.txt");
				}
				if (e.key.code == sf::Keyboard::L)
				{
					nn.LoadFromFile("C:\\Users\\elies\\Desktop\\save1.txt");
				}
				if (e.key.code == sf::Keyboard::Space)
				{
					train = train ? false : true;
				}
			}
		}

		if (train)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				p.ApplyForce(-5);
				press = true;
			}
		}

		Data d(b.pos, b.vel, o.pos, o.size, 0);

		double y = nn.Calculate({ d.pPos, d.pVel, d.oPos, d.oSize })[0];
		if (y > 0.5)
		{
			b.ApplyForce(-5);
		}

		if (train)
		{
			p.Update(o, data);
		}
		b.Update(o, data);
		o.Update(data, nn);

		if (train)
		{
			count++;

			if (count >= max)
			{
				data.push_back(Data(p.pos, p.vel, o.pos, o.size, press, true));
				count = 0;
			}
		}

		window.clear();

		o.Draw(window);
		b.Draw(window);
		if (train)
		{
			p.Draw(window);
		}

		window.display();
	}
}