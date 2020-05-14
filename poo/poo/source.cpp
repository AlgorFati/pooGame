#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
using namespace std;

const float GameWidth = 640;
const float GameHeight = 800;

// github
// AlgorFati

// 0~1 
float randomFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

class Poo
{
public:
	Poo(const sf::Texture& texture)
	{
		_sprite.setTexture(texture);
		_sprite.setScale(0.1f, 0.1f);

		_sprite.setPosition(randomFloat() * GameWidth, randomFloat() * 100);

		_velocity = sf::Vector2f(0, 3.0f + randomFloat() * 2.5f);

	}

	const sf::Sprite& getSprite() const { return _sprite; }

	void move()
	{
		_velocity += sf::Vector2f(0, 0.1f + randomFloat() * 0.1f);

		_sprite.move(_velocity);
	}

private:
	sf::Sprite _sprite;
	sf::Vector2f _velocity;

};

std::vector<Poo> pooList;
int level = 0;


float getRadiusFromSprite(const sf::Sprite& sprite)
{
	sf::FloatRect rect = sprite.getGlobalBounds();
	return std::min<float>(rect.width / 2, rect.height / 2);
}

sf::Vector2f getCenterFromSprite(const sf::Sprite& sprite)
{
	sf::FloatRect rect = sprite.getGlobalBounds();
	float x = rect.left + rect.width / 2;
	float y = rect.top + rect.height / 2;

	return sf::Vector2f(x, y);
}

float distanceSq(sf::Vector2f a, sf::Vector2f b)
{
	return (b.x - a.x)* (b.x - a.x) + (b.y - a.y)* (b.y - a.y);
}

bool collideSprite(const sf::Sprite& a, const sf::Sprite& b)
{
	float aRadius = getRadiusFromSprite(a);
	sf::Vector2f aCenter = getCenterFromSprite(a);

	float bRadius = getRadiusFromSprite(b);
	sf::Vector2f bCenter = getCenterFromSprite(b);

	float disSq = distanceSq(aCenter, bCenter);

	return disSq < (aRadius + bRadius) * (aRadius + bRadius);
}

void spawnPoo(const sf::Texture& texture)
{
	float val = randomFloat();

	if (val < 0.1f * level)
	{
		pooList.push_back(Poo(texture));
	}
}

void removePoo()
{
	pooList.erase(
		remove_if(pooList.begin(), pooList.end(), [](const Poo& poo)
	{
		return poo.getSprite().getPosition().y > 700;
	}),
		pooList.end()
		);
}


// 똥피하기 게임
int main()
{
	sf::RenderWindow window(sf::VideoMode(GameWidth, GameHeight), "name");
	srand((unsigned int)time(NULL));

	sf::Texture pooText;
	pooText.loadFromFile("poo.png");

	sf::Texture playerText;
	playerText.loadFromFile("player.png");

	sf::Sprite playerSprite;
	playerSprite.setTexture(playerText);
	playerSprite.setPosition(0, GameHeight - 100);

	int cnt = 300;
	while (window.isOpen())
	{
		cnt++;
		level = cnt / 300;

		sf::Event ev;
		while (window.pollEvent(ev))
		{
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerSprite.move(sf::Vector2f(-5, 0));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerSprite.move(sf::Vector2f(5, 0));
		}

		spawnPoo(pooText);

		for (int i = 0; i < pooList.size(); ++i)
		{
			if (collideSprite(pooList[i].getSprite(), playerSprite))
			{
				window.close();
			}
			pooList[i].move();
		}

		removePoo();


		window.clear(sf::Color::White);

		for (int i = 0; i < pooList.size(); ++i)
		{
			window.draw(pooList[i].getSprite());
		}

		window.draw(playerSprite);

		window.display();

	}

	return 0;
}