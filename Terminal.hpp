#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Planet.hpp"
#include "Universe.hpp"
#include "Constants.hpp"

class Terminal {

public:
	Terminal (Universe *cUniverse, std::istream *cInputStream, std::ostream *cOutputStream);

	Universe *universe;
	std::istream *inputStream;
	std::ostream *outputStream;

	void readInput();
	std::vector<std::string> divideIntoWords(std::string textToDivide);
	sf::Color stringToEnumColor(std::string inputString);
	void toLower(std::string & inputString);
	Planet& findByName(std::string);
};

std::vector<std::string> Terminal::divideIntoWords(std::string textToDivide) {

	std::vector<std::string> result;
	result.push_back("");
	for(auto currentLetter : textToDivide) {
		if(currentLetter == ' ')
			result.push_back("");
		else
			result.back() += currentLetter;
	}

	return result;

}

void Terminal::toLower(std::string &inputString) {

	for(auto &letter : inputString)
		if(letter >= 'A' and letter <= 'Z')
			letter -= 'A' - 'a';
}

sf::Color Terminal::stringToEnumColor(std::string inputString) {

	toLower(inputString);

	#define ICRE(a,b) else if (inputString == "a") return sf::Color::b
	
	if(inputString == "?") {

		*outputStream 	<< "Available predefined colors are:" << std::endl
				<< "Black, White, Red, Green, Blue, Yellow, Magenta, Cyan, Transparent" << std::endl;
	}
	ICRE("black", Black);
	ICRE("white", White);
	ICRE("red", Red);
	ICRE("green", Green);
	ICRE("blue", Blue);
	ICRE("yellow", Yellow);
	ICRE("magenta", Magenta);
	ICRE("cyan", Cyan);
	ICRE("transparent", Transparent);
	else 
		*outputStream << "This color is not predefined." << std::endl;

	#undef ICRE
}

Terminal::Terminal (Universe *cUniverse = nullptr, std::istream *cInputStream = nullptr, std::ostream *cOutputStream = nullptr) 
:universe{cUniverse}, inputStream{cInputStream}, outputStream{cOutputStream} {}

Planet& Terminal::findByName(std::string planetsName) {

	for(auto& currentPlanet : universe->planets) 
		if(currentPlanet.name == planetsName)
			return currentPlanet;
	
	*outputStream << "Planet " << planetsName << " does not exist" << std::endl;
}

void Terminal::readInput() {

	*outputStream << "Entered command mode - program is paused" << std::endl;

	while(true) {
		std::vector<std::string> arguments;
		
		{
			std::string input;
			std::getline(*inputStream, input);
			arguments = divideIntoWords(input);	
		}	

		if (arguments[0] == "newPlanet") {

			universe->planets.push_back(Planet(arguments[1]));
		}
		else if(arguments[0] == "setRadius") {

			findByName(arguments[1]).setRadius(std::stod(arguments[2]));
		}
		else if(arguments[0] == "setColor") {

			findByName(arguments[1]).setFillColor(stringToEnumColor(arguments[2]));
		}
		else if(arguments[0] == "setVelocity") {
	
			findByName(arguments[1]).speed = OurVector(std::stod(arguments[2]), std::stod(arguments[3]));
		}
		else if(arguments[0] == "setPosition") {

			findByName(arguments[1]).setPosition(std::stod(arguments[2]), std::stod(arguments[3]));
		}
		else if(arguments[0] == "setMass") {

			findByName(arguments[1]).mass = std::stod(arguments[2]);
		}
		else if(arguments[0] == "setG") {

			universe->G = std::stod(arguments[1]);
		}
		else if (arguments[0] == "listPlanets") {

			if(!universe->planets.size())

				*outputStream << "There are no planets yet! Try creating one with newPlanet";
			else {

				*outputStream << "Planet name \t mass \t radius \t positon \t velocity" << std::endl;
				
				for(auto cPlanet : universe->planets)
					*outputStream << cPlanet.name << '\t' << cPlanet.mass << '\t' << cPlanet.getRadius() << "\t(" << cPlanet.getPosition().x << ", " << cPlanet.getPosition().y << ")\t(" << cPlanet.speed.x << ", " << cPlanet.speed.y << std::endl;
			}
				
		}
		else if (arguments[0] == "editPlanet") {

			*outputStream << "pasdjfopaejpoa Twoja Stara";
		}
		else if (arguments[0] == "continue") {
			
			*outputStream << "Continuing" << std::endl;
			return;
		}
		else
			*outputStream << "This command does not exist" << std::endl;
	}
}
	

#endif
