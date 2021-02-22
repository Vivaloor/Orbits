#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include "Universe.hpp"
#include "Constants.hpp"
#include "Planet.hpp"



int main() {

	sf::RenderWindow app(sf::VideoMode(1000, 1000), "Orbity");

	sf::Font font;
	font.loadFromFile("LucidaSansRegular.ttf");
	sf::Text T("hello", font);

	app.setFramerateLimit(60);

	Planet planet(10, sf::CircleShape(5), OurVector(500, 300), OurVector(0, 0));
	Planet planet_2(100000, sf::CircleShape(50), OurVector(500, 500), OurVector(0, 0));
	Planet planet_3(10, sf::CircleShape(5), OurVector(500, 700), OurVector(0, 0));

	planet.setFillColor(sf::Color::Blue);
	planet_3.setFillColor(sf::Color::Green);

	Universe universe;
	universe.planets.push_back(planet);
	universe.planets.push_back(planet_2);
	universe.planets.push_back(planet_3);


	while (app.isOpen()) {

        sf::Event event;
        while (app.pollEvent(event)) {

	if (event.type == sf::Event::Closed)
		app.close();
        }

	app.clear();

	universe.update();
	app.draw(universe);

        app.display();
    }

    return EXIT_SUCCESS;
}
