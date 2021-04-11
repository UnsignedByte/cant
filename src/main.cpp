#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "ant.h"
#include "utils.h"
#include <iostream>
#include <cstring>

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
	int x = 0;
	Ant test(urand(0,WIDTH), urand(0,HEIGHT), 1);
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "c++an't [sic]");
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();


		test.move();

		auto render = test.render();

		// std::cout << render << std::endl;
		window.draw(*render);
		delete render;

		window.display();
	}

	return 0;
}