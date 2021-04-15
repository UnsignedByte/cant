/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-15 13:12:50
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "ant.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstring>
#include "render.hpp"
#include "hill.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const sf::Time frameTime = sf::seconds(1.f/30.f);


int main()
{
	int x = 0;
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "c++an't [sic]");
	window.setVerticalSyncEnabled(0);
	sf::Clock renderClock;
	sf::Time elapsed = sf::seconds(0); 

	sf::RenderTexture rt;
	if (!rt.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	Render renderer(&rt);

	renderer.addHill(Hill::randomHill(WIDTH, HEIGHT, 200000));

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

		renderer.tick();

		renderer.renderHills();

		window.draw(sf::Sprite((*renderer.getTex()).getTexture()));

		window.display();

		std::cout << renderClock.getElapsedTime().asMilliseconds() << std::endl;
		sf::sleep(frameTime-renderClock.restart());
	}

	return 0;
}