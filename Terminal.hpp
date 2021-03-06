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
	Terminal (Universe *cUniverse = nullptr, std::istream *cInputStream = nullptr, std::ostream *cOutputStream = nullptr);

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
		return sf::Color(std::stoul(inputString));
	#undef ICRE
}

Terminal::Terminal (Universe *cUniverse, std::istream *cInputStream, std::ostream *cOutputStream) 
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

	while(true) {

		if(isOpenedFromFile and inputStream->peek() == EOF) {

			*outputStream << "Instructions have been loaded" << std::endl;
			return;
		}

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
				*outputStream << "Error has been encountered during opening \"" << arguments[1] << "\" file." << std::endl;
			
		}
		else if (arguments[0] == "saveToFile") {

			std::ofstream fileStream(arguments[1]);
			
			if(fileStream.good()) {

				for (auto currentPlanet : universe->planets) {

					fileStream << "newPlanet " << currentPlanet.name << std::endl;
					fileStream << "setRadius " << currentPlanet.name << ' ' << currentPlanet.getRadius() << std::endl;
					fileStream << "setPosition " << currentPlanet.name << ' ' << currentPlanet.getPosition().x << ' ' << currentPlanet.getPosition().y << std::endl;
					fileStream << "setVelocity " << currentPlanet.name << ' ' << currentPlanet.speed.x << ' ' << currentPlanet.speed.y << std::endl;
					fileStream << "setColor " << currentPlanet.name << ' ' << currentPlanet.getFillColor().toInteger() << std::endl;
					fileStream << std::endl;
				}
			}
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
		else if (arguments[0] == "help") {

			*outputStream << "Available commands:\n\
newPlanet [planet's name]\n\
removePlanet [planet's name]\n\
listPlanets\n\
setPosition [planet's name] [x] [y]\n\
setColor [planet's name] [color]\n\
setMass [planet's name] [mass]\n\
setVelocity [planet's name] [x] [y]\n\
setRadius [planet's name] [radius]\n\
setG [G constant]\n\
loadFromFile [path]\n\
You can press space while focused on the display window to stop simulation and enter the commandline mode. Automaticly, after turning program on the \"initiate.orb\" is loaded. If you want to create your own state file, you can create it just like any other text file. The \".orb\" suffix is suggested." << std::endl;
		}
		else if (arguments[0] == "continue") {

			*outputStream << "Instructions have been loaded" << std::endl;
			return;
		}
		else if(arguments[0] == "\n" or arguments[0] == ""){}					
		else 
			*outputStream << "This command does not exist" << std::endl;

	}
}
	

#endif
