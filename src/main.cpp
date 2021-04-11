/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-11 13:41:36
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "ant.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstring>

const int WIDTH = 800;
const int HEIGHT = 600;
const sf::Time frameTime = sf::seconds(1.f/60.f);


int main()
{
	int x = 0;
	Ant test(utils::rand::urand(0,WIDTH), utils::rand::urand(0,HEIGHT), 1);
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "c++an't [sic]");
	sf::setVerticalSyncEnabled(0);
	sf::Clock renderClock;
	sf::Time elapsed = sf::seconds(0); 

	// window.setFramerateLimit(60);
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

		std::cout << x << std::endl;
		x++;
		window.display();
	}

	return 0;
}