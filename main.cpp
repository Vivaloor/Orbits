#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>

using OurFloat = float;
using OurVector = sf::Vector2<float>;

const double G{1};

class Planet : public sf::CircleShape {

public:
	Planet(float cMass, sf::CircleShape cPicture, OurVector cPosition = {0, 0}, OurVector cSpeed = {0, 0}, OurVector cAcceleration = {0, 0})
	: mass{cMass}, CircleShape(cPicture),  speed{cSpeed}, acceleration{cAcceleration} {

		setPosition(cPosition);
		setOrigin(OurVector(getRadius(), getRadius()));
	}

	void update() {

		speed += acceleration;
		move(speed);
	}

	float mass;
	OurVector speed;
	OurVector acceleration;
};

class Universe : public sf::Drawable {

public:
	std::vector<Planet> planets;

	void update() {

		for(int i = 0; i < planets.size(); i++)
			planets[i].acceleration = OurVector(0, 0);

		for(int i = 0; i < planets.size(); i++) {

			for(int j = i + 1; j < planets.size(); j++)
				actBetween(planets[i], planets[j]);

			planets[i].update();
		}

	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const {

		for(auto& current_planet : planets)
			target.draw(current_planet, states);
	}

private:

	OurVector basis(OurVector input_vector) {

		OurFloat input_vector_lenght = std::sqrt(std::pow(input_vector.x, 2) + std::pow(input_vector.y, 2));

		return(OurVector((input_vector.x / input_vector_lenght), (input_vector.y / input_vector_lenght)));
	}

	void actBetween (Planet &first_planet, Planet &second_planet) {

		OurFloat distance_squared = std::pow(static_cast<double>(first_planet.getPosition().x - second_planet.getPosition().x), 2) + std::pow(static_cast<double>(first_planet.getPosition().y + second_planet.getPosition().y), 2);
		OurFloat scalar_value = G * first_planet.mass * second_planet.mass / distance_squared;

		first_planet.acceleration += basis((second_planet.getPosition() - first_planet.getPosition())) * scalar_value / first_planet.mass;
		second_planet.acceleration += basis((first_planet.getPosition() - second_planet.getPosition())) * scalar_value / second_planet.mass;
	}
};

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
