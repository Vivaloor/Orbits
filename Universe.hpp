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
	void actBetween (Planet &first_planet, Planet &second_planet);
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

void Universe::actBetween (Planet &first_planet, Planet &second_planet)
{

    OurFloat distance_squared = std::pow(static_cast<double>(first_planet.getPosition().x - second_planet.getPosition().x), 2) + std::pow(static_cast<double>(first_planet.getPosition().y + second_planet.getPosition().y), 2);
    OurFloat scalar_value = G * first_planet.mass * second_planet.mass / distance_squared;

    first_planet.acceleration += basis((second_planet.getPosition() - first_planet.getPosition())) * scalar_value / first_planet.mass;
    second_planet.acceleration += basis((first_planet.getPosition() - second_planet.getPosition())) * scalar_value / second_planet.mass;
}


#endif // UNIVERSE_HPP_INCLUDED
