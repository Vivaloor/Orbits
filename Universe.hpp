#ifndef UNIVERSE_HPP_INCLUDED
#define UNIVERSE_HPP_INCLUDED

#include "Constants.hpp"

#include "Planet.hpp"
#include <vector>
#include <cmath>


class Universe : public sf::Drawable {

public:
	Universe() = default;
	Universe(Universe&);

	std::vector<Planet> planets;
	OurFloat G{1};

	void update();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:

	OurVector basis(OurVector input_vector);
	OurFloat lenghtOfVector(OurVector);
	void actBetween (Planet &firstPlanet, Planet &secondPlanet);
	void collideBetween (Planet &firstPlanet, Planet &secondPlanet);
};

Universe::Universe(Universe &original) {

	planets = original.planets;
	G = original.G;
}

void Universe::update()
{

    for(int i = 0; i < static_cast<int>(planets.size()); i++)
        planets[i].acceleration = OurVector(0, 0);

    for(int i = 0; i < static_cast<int>(planets.size()); i++)
    {

       	for(int j = i + 1; j < static_cast<int>(planets.size()); j++)
		actBetween(planets[i], planets[j]);

		planets[i].update();
	}

}

void Universe::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

	for(auto& current_planet : planets)
		target.draw(current_planet, states);
}

OurVector Universe::basis(OurVector input_vector)
{

	OurFloat input_vector_lenght = std::sqrt(std::pow(input_vector.x, 2) + std::pow(input_vector.y, 2));

	return(OurVector((input_vector.x / input_vector_lenght), (input_vector.y / input_vector_lenght)));
}

OurFloat Universe::lenghtOfVector(OurVector inputVector) {

	return std::sqrt(std::pow(inputVector.x, 2) + std::pow(inputVector.y, 2));
}

void Universe::actBetween (Planet &firstPlanet, Planet &secondPlanet)
{

	OurFloat distanceSquared = std::pow(static_cast<double>(firstPlanet.getPosition().x - secondPlanet.getPosition().x), 2) + std::pow(static_cast<double>(firstPlanet.getPosition().y - secondPlanet.getPosition().y), 2);
	OurFloat scalarValue = G * firstPlanet.mass * secondPlanet.mass / distanceSquared;

	firstPlanet.acceleration += basis((secondPlanet.getPosition() - firstPlanet.getPosition())) * scalarValue / firstPlanet.mass;
	secondPlanet.acceleration += basis((firstPlanet.getPosition() - secondPlanet.getPosition())) * scalarValue / secondPlanet.mass;
}

/*void Universe::collideBetween (Planet &firstPlanet, Planet &secondPlanet) {

	OurFloat distanceBetweenCenters {std::sqrt(std::pow(firstPlanet.getPosition().x - secondPlanet.getPosition().x, 2) + std::pow(firstPlanet.getPosition().y - secondPlanet.getPosition().y, 2))}
	if(firstPlanet.getRadius() + secondPlanet.getRadius() < distanceBetweenCenters) {

		OurFloat angleBetweenPlanets;

		if(firstPlanet.getPosition().x - secondPlanet.getPosition().x) {
			angleBetweenPlanets = std::acos((secondPlanet.getPosition().x - firstPlanet.getPosition().x));
		}
		else {
			angleBetweenPlanets = std::asin((firstPlanet.getPosition().y - secondPlanet.getPosition().y));
		}

		OurFloat angleOfFirst;
		if(firstPlanet.speed.x)
			angleOfFirst = std::asin(firstPlanet.speed.x / lenghtOfVector(firstPlanet.speed));
		else
			angleOfFirst = std::acos(firstPlanet.speed.y / lenghtOfVector(firstPlanet.speed));

		firstPlanet.speed = 
		
	}
}*/

#endif // UNIVERSE_HPP_INCLUDED
