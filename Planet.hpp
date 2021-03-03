#ifndef PLANET_HPP_INCLUDED
#define PLANET_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

#include "Constants.hpp"

class Planet : public sf::CircleShape {

public:
	Planet(std::string cName, float cMass = 100, sf::CircleShape cPicture = sf::CircleShape(50), OurVector cPosition = {0, 0}, OurVector cSpeed = {0, 0});

	void update();

	void setRadius(OurFloat newRadius);

	float mass;
	OurVector speed;
	std::string name;
	//bool makesTrail {false};
	//sf::Color trailsColor {sf::Color::White}; We will include it when we decide on adding trail option.
};

Planet::Planet(std::string cName, float cMass, sf::CircleShape cPicture, OurVector cPosition, OurVector cSpeed)
    : name{cName}, mass{cMass}, CircleShape(cPicture), speed{cSpeed}
{
	setPosition(cPosition);
	setOrigin(getRadius(), getRadius());
}

void Planet::setRadius(OurFloat newRadius) {

	setPosition(getPosition() - OurVector(getRadius() - newRadius, getRadius() - newRadius));
	sf::CircleShape::setRadius(newRadius);
	setOrigin(newRadius, newRadius);
}


inline void Planet::update()
{
    move(speed);
}

#endif // PLANET_HPP_INCLUDED
