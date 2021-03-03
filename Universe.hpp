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

	OurFloat lenghtOfVector(OurVector);
	OurVector basis(OurVector);
	void actBetween (Planet &firstPlanet, Planet &secondPlanet);
	void collideBetween (Planet &firstPlanet, Planet &secondPlanet);
	OurFloat dotProduct(OurVector, OurVector);
};

Universe::Universe(Universe &original) {

	planets = original.planets;
	G = original.G;
}

void Universe::update()
{

	for(int i = 0; i < static_cast<int>(planets.size()) - 1; i++) {

		planets[i].update();

		for(int j = i + 1; j < static_cast<int>(planets.size()); j++) {

			collideBetween(planets[i], planets[j]);
			actBetween(planets[i], planets[j]);
		}

	}

	if(planets.size())
		planets[planets.size() - 1].update();
}

void Universe::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

	for(auto& current_planet : planets)
		target.draw(current_planet, states);
}

OurFloat Universe::lenghtOfVector(OurVector inputVector) {

	return std::sqrt(std::pow(inputVector.x, 2) + std::pow(inputVector.y, 2));
}

OurVector Universe::basis(OurVector input_vector) {
	OurFloat input_vector_lenght = std::sqrt(std::pow(input_vector.x, 2) + std::pow(input_vector.y, 2));

	return(OurVector((input_vector.x / input_vector_lenght), (input_vector.y / input_vector_lenght)));
}

void Universe::actBetween (Planet &firstPlanet, Planet &secondPlanet)
{

	OurFloat distanceSquared = std::pow(firstPlanet.getPosition().x - secondPlanet.getPosition().x, 2) + std::pow(firstPlanet.getPosition().y - secondPlanet.getPosition().y, 2);
	OurFloat scalarValue = G * firstPlanet.mass * secondPlanet.mass / distanceSquared;

	firstPlanet.speed += basis((secondPlanet.getPosition() - firstPlanet.getPosition())) * scalarValue / firstPlanet.mass;
	secondPlanet.speed += basis((firstPlanet.getPosition() - secondPlanet.getPosition())) * scalarValue / secondPlanet.mass;
}

OurFloat Universe::dotProduct(OurVector f, OurVector s) {

	return f.x * s.x + f.y * s.y;
}

void Universe::collideBetween (Planet &firstPlanet, Planet &secondPlanet) {


	if((firstPlanet.getRadius() + secondPlanet.getRadius()) * (firstPlanet.getRadius() + secondPlanet.getRadius()) >= (firstPlanet.getPosition().x - secondPlanet.getPosition().x) * (firstPlanet.getPosition().x - secondPlanet.getPosition().x) + (firstPlanet.getPosition().y - secondPlanet.getPosition().y) * (firstPlanet.getPosition().y - secondPlanet.getPosition().y)) {

		{
			OurVector vectorBetween = secondPlanet.getPosition() - firstPlanet.getPosition();
			OurFloat overlapDistanceDividedByTwo = (firstPlanet.getRadius() + secondPlanet.getRadius() - lenghtOfVector(vectorBetween)) / 2;

			OurVector normalisedVector = vectorBetween / lenghtOfVector(vectorBetween);

			firstPlanet.setPosition(firstPlanet.getPosition() - normalisedVector * overlapDistanceDividedByTwo);
			secondPlanet.setPosition(secondPlanet.getPosition() + normalisedVector * overlapDistanceDividedByTwo);
		}

		OurVector normalisedVector = (secondPlanet.getPosition() - firstPlanet.getPosition()) / lenghtOfVector(firstPlanet.getPosition() - secondPlanet.getPosition());
		OurVector k(firstPlanet.speed - secondPlanet.speed);
		OurFloat p = 2 * dotProduct(normalisedVector, k) / (firstPlanet.mass + secondPlanet.mass);
		firstPlanet.speed = firstPlanet.speed - p * secondPlanet.mass * normalisedVector;
		secondPlanet.speed = secondPlanet.speed + p * firstPlanet.mass * normalisedVector;
	}
}

#endif // UNIVERSE_HPP_INCLUDED
