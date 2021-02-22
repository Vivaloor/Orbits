#ifndef PLANET_HPP_INCLUDED
#define PLANET_HPP_INCLUDED
#include "Constants.hpp"

class Planet : public sf::CircleShape {

public:
	Planet(float cMass, sf::CircleShape cPicture, OurVector cPosition = {0, 0}, OurVector cSpeed = {0, 0}, OurVector cAcceleration = {0, 0});

	void update();

	float mass;
	OurVector speed;
	OurVector acceleration;
};

Planet::Planet(float cMass, sf::CircleShape cPicture, OurVector cPosition, OurVector cSpeed, OurVector cAcceleration)
    : mass{cMass}, CircleShape(cPicture),  speed{cSpeed}, acceleration{cAcceleration}
{

    setPosition(cPosition);
    setOrigin(OurVector(getRadius(), getRadius()));
}

void Planet::update()
{

    speed += acceleration;
    move(speed);
}

#endif // PLANET_HPP_INCLUDED
