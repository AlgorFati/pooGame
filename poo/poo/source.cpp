#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

float GameWidth = 640;
float GameHeight = 800;

float randomFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float getRadiusFromSprite(const Sprite& sprite)
{
	FloatRect rect = sprite.getGlobalBounds();

	return std::min(rect.width / 2, rect.height / 2);
}

Vector2f getCenterFromSprite(const Sprite& sprite)
{
	FloatRect rect = sprite.getGlobalBounds();

	return Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2);
}

float distanceSq(Vector2f a, Vector2f b)
{
	return (a.x - b.x)* (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// collide
bool collideSprite(const Sprite& a, const Sprite& b)
{
	float aRadius = getRadiusFromSprite(a);
	float bRadius = getRadiusFromSprite(b);

	Vector2f aCenter = getCenterFromSprite(a);
	Vector2f bCenter = getCenterFromSprite(b);

	float disSq = distanceSq(aCenter, bCenter);

	return disSq < (aRadius + bRadius)* (aRadius + bRadius);
}

vector<Sprite> poos;

void spawnPoo(const Texture& texture)
{
	if (randomFloat() < 0.1f)
	{
		Sprite pooSprite;
		pooSprite.setTexture(texture);
		pooSprite.setScale(0.1, 0.1);
		// random pos
		pooSprite.setPosition(randomFloat() * GameHeight, randomFloat() * 100);

		poos.push_back(pooSprite);
	}
}

void removePoo()
{
	poos.erase(
		remove_if(poos.begin(), poos.end(), [](const Sprite& poo) { return poo.getPosition().y > 700; }),
		poos.end()
	);
}

int main()
{
	RenderWindow window(VideoMode(GameWidth, GameHeight), "poo Game");

	Texture pooText;
	pooText.loadFromFile("poo.png");

	Texture playerText;
	playerText.loadFromFile("player.png");

	Sprite playerSprite;
	playerSprite.setTexture(playerText);
	playerSprite.setPosition(0, 700);

	while (window.isOpen())
	{
		Event ev;
		while (window.pollEvent(ev))
		{
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerSprite.move(Vector2f(-5, 0));
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			playerSprite.move(Vector2f(5, 0));
		}

		spawnPoo(pooText);


		for (int i = 0; i < poos.size(); ++i)
		{
			if (collideSprite(poos[i], playerSprite))
			{
				window.close();
			}
		}

		// move
		for (int i = 0; i < poos.size(); ++i)
		{
			poos[i].move(Vector2f(0, 5));
		}

		removePoo();

		window.clear(Color::White);

		for (int i = 0; i < poos.size(); ++i)
		{
			window.draw(poos[i]);
		}
		window.draw(playerSprite);

		window.display();
	}
}
