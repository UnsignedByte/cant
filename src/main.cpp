/*
* @Author: UnsignedByte
* @Date:   2021-04-11 11:24:20
* @Last Modified by:   UnsignedByte
* @Last Modified time: 2021-04-16 23:20:45
*/
#include <SFML/Graphics.hpp>
// #include <SFML/OpenGL.hpp>
#include <cmath>
#include "ant.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstring>
#include "render.hpp"
#include "hill.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const sf::Time frameTime = sf::seconds(1.f/60.f);

int main()
{
	int x = 0;
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "c++an't [sic]");
	window.setVerticalSyncEnabled(0);
	sf::Clock renderClock;
	sf::Time elapsed = sf::seconds(0);

	// glEnable(GL_TEXTURE_2D);

	// window.setActive(1);

	Render renderer(WIDTH, HEIGHT);

	renderer.addHill(Hill::randomHill(WIDTH, HEIGHT, 500));

	// window.setFramerateLimit(60);
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
			// Close window: exit
			case sf::Event::Closed:
				window.close();
				break;
			// case sf::Event::Resized:
			// glViewport(0,0, event.size.width, event.size.height);
			// 	break;
			case sf::Event::KeyPressed:
				switch(event.key.code)
				{
					case sf::Keyboard::W:
						renderer.W = 1;
						break;
					case sf::Keyboard::A:
						renderer.A = 1;
						break;
					case sf::Keyboard::S:
						renderer.S = 1;
						break;
					case sf::Keyboard::D:
						renderer.D = 1;
						break;
				}
				break;
			case sf::Event::KeyReleased:
				switch(event.key.code)
				{
					case sf::Keyboard::W:
						renderer.W = 0;
						break;
					case sf::Keyboard::A:
						renderer.A = 0;
						break;
					case sf::Keyboard::S:
						renderer.S = 0;
						break;
					case sf::Keyboard::D:
						renderer.D = 0;
						break;
				}
				break;
			}
		}

		window.clear();

		renderer.tick();

		renderer.renderHills();

		window.draw(renderer.getDrawn());

		window.display();

		// std::cout << renderClock.getElapsedTime().asMilliseconds() << std::endl;
		sf::sleep(frameTime-renderClock.getElapsedTime());
		renderClock.restart();
	}

	return 0;
}