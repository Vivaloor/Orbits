#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Planet.hpp"
#include "Universe.hpp"
#include "Constants.hpp"

class Terminal {

public:
	Terminal (Universe *cUniverse, std::istream *cInputStream, std::ostream *cOutputStream);

	Universe *universe;
	std::istream *inputStream;
	std::ostream *outputStream;

	void readInput(bool isOpenedFromFile);
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

	while(result.size() < 30)
		result.push_back("");

	return result;

}

void Terminal::toLower(std::string &inputString) {

	for(auto &letter : inputString)
		if(letter >= 'A' and letter <= 'Z')
			letter -= 'A' - 'a';
}

sf::Color Terminal::stringToEnumColor(std::string inputString) {

	toLower(inputString);

	#define ICRE(SMALL_COLOR, BIG_COLOR) else if (inputString == #SMALL_COLOR) return sf::Color::BIG_COLOR
	
	if(inputString == "?") {

		*outputStream 	<< "Available predefined colors are:" << std::endl
				<< "Black, White, Red, Green, Blue, Yellow, Magenta, Cyan, Transparent" << std::endl;
	}
	ICRE(black, Black);
	ICRE(white, White);
	ICRE(red, Red);
	ICRE(green, Green);
	ICRE(blue, Blue);
	ICRE(yellow, Yellow);
	ICRE(magenta, Magenta);
	ICRE(cyan, Cyan);
	ICRE(transparent, Transparent);
	else 
		throw("Color " + inputString + " is not predefiend\n");

	return sf::Color::White;
	#undef ICRE
}

Terminal::Terminal (Universe *cUniverse = nullptr, std::istream *cInputStream = nullptr, std::ostream *cOutputStream = nullptr) 
:universe{cUniverse}, inputStream{cInputStream}, outputStream{cOutputStream} {}

Planet& Terminal::findByName(std::string planetsName) {

	for(auto& currentPlanet : universe->planets) 
		if(currentPlanet.name == planetsName)
			return currentPlanet;
	
	throw("Planet " + planetsName + " does not exist!\n");
}

void Terminal::readInput(bool isOpenedFromFile = false) {

	if(!isOpenedFromFile)
		*outputStream << "Entered command mode - program is paused" << std::endl;
	else
		*outputStream << "Started loading the file" << std::endl;

	while(true) {

		std::vector<std::string> arguments;	
		std::string input;
		std::getline(*inputStream, input);
		arguments = divideIntoWords(input);	

		if (arguments[0] == "newPlanet") {

			bool hasBeenCatched = false;
			try {
				findByName(arguments[1]);
			}
			catch(std::string) {
				universe->planets.push_back(Planet(arguments[1]));
				hasBeenCatched = true;
			}
			
			if(!hasBeenCatched)
				*outputStream << "Planet " << arguments[1] << " has already been defined" << std::endl;
			
		}
		else if (arguments [0] == "removePlanet") {

			for(std::vector<Planet>::iterator iterator = universe->planets.begin(); iterator < universe->planets.end(); iterator++)
				if(iterator->name == arguments[1]) {
					universe->planets.erase(iterator);
					break;
				}
		}
		else if (arguments[0] == "setRadius") {

			try {
				findByName(arguments[1]).setRadius(std::stod(arguments[2]));
			}
			catch(std::string err_txt) {
				*outputStream << err_txt;
			}
		}
		else if (arguments[0] == "setColor") {
 
			try {
				findByName(arguments[1]).setFillColor(stringToEnumColor(arguments[2]));
			}
			catch(std::string err_txt) {
				*outputStream << err_txt;
			}
		}
		else if (arguments[0] == "setVelocity") {
	
			try {
				findByName(arguments[1]).speed = OurVector(std::stod(arguments[2]), std::stod(arguments[3]));
			}
			catch(std::string err_txt) {
				*outputStream << err_txt;
			}
		}
		else if (arguments[0] == "setPosition") {

			try {
				findByName(arguments[1]).setPosition(std::stod(arguments[2]), std::stod(arguments[3]));
			}
			catch(std::string err_txt) {
				*outputStream << err_txt;
			}
		}
		else if (arguments[0] == "setMass") {

			try {
				findByName(arguments[1]).mass = std::stod(arguments[2]);
			}
			catch(std::string err_txt) {
				*outputStream << err_txt;
			}
		}
		else if (arguments[0] == "setG") {

			universe->G = std::stod(arguments[1]);
		}
		else if (arguments[0] == "loadFromFile") { 

			std::ifstream inputFileStream;
			inputFileStream.open(arguments[1]);

			if(inputFileStream.good()) {
				std::istream *ibuffer {inputStream};
				inputStream = &inputFileStream;
		
				readInput(true);

				inputStream = ibuffer;
				inputFileStream.close();
			}
			else 
				std::cout << "Error has been encountered during opening \"" << arguments[1] << "\" file." << std::endl;
			
		}
		else if (arguments[0] == "listPlanets") {

			if(!universe->planets.size())

				*outputStream << "There are no planets yet! Try creating one with newPlanet" << std::endl;
			else {

				*outputStream << "Planet name \tMass \tRadius \tPositon \tVelocity" << std::endl;
				
				for(auto cPlanet : universe->planets)
					*outputStream << cPlanet.name << "\t\t" << cPlanet.mass << '\t' << cPlanet.getRadius() << "\t(" << cPlanet.getPosition().x << ", " << cPlanet.getPosition().y << ")\t\t(" << cPlanet.speed.x << ", " << cPlanet.speed.y << ')' << std::endl;
			}
				
		}
		else if (arguments[0] == "continue" and !isOpenedFromFile) {
			
			*outputStream << "Continuing" << std::endl;
			return;
		}
		else if ((arguments[0] == "continue" or inputStream->peek() == EOF) and isOpenedFromFile) {

			*outputStream << "File has been loaded" << std::endl;
			return;
		}
		else if(arguments[0] == "" and isOpenedFromFile)
		{
		}
		else
			*outputStream << "This command does not exist" << std::endl;
	}
}
	

#endif
