#include <SFML/Graphics.hpp>
#include <iostream>
#include "Universe.hpp"
#include "Constants.hpp"
#include "Planet.hpp"
#include "Terminal.hpp"

int main() {

	sf::RenderWindow app(sf::VideoMode(1000, 1000), "Orbity");

	app.setFramerateLimit(60);

	Universe universe;
	Terminal terminal(&universe, &std::cin, &std::cout);

	while (app.isOpen()) 
	{

		sf::Event event;
		while (app.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed or sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				app.close();
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				terminal.readInput();
		}

		app.clear();

		universe.update();
		app.draw(universe);

		app.display();
	}

    return EXIT_SUCCESS;
}
