#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Functions.h"




int main()
{
	int width = 600, height = 600; 
	double recordDistance; 
	std::vector<sf::CircleShape> bestEver ; 
	srand(time(NULL)); 
	const int numCities = 5; 
	sf::VertexArray lines(sf::LinesStrip, numCities);
	sf::VertexArray bestLines(sf::LinesStrip, numCities); 
	
	// each city should have a rand x and a rand y 
	std::vector<sf::CircleShape> cities; 
	initCities(cities , numCities, width, height); 

	
	recordDistance = calcDistance(cities); // calculates the distance of the cities in its first state 
	bestEver = cities; //copy(bestEver, cities ); // copies the cities to the best ever 
	sf::RenderWindow window(sf::VideoMode(width,height), "TravelingSalesmanGA");

	while (window.isOpen())
	{
		sf::Event event; 
		//pollevents ; input 
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: 
				window.close(); 
				break; 
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close(); 
				}

				break; 
			}
		}


		// update ; logic 


		
		//render 
		window.clear(sf::Color::Black); 
		
		for (int i = 0; i < cities.size(); i++)
		{
			window.draw(cities[i]); 
			lines[i] = sf::Vector2f(cities[i].getPosition().x + (cities[i].getRadius()), cities[i].getPosition().y + (cities[i].getRadius()));
			bestLines[i] = sf::Vector2f(bestEver[i].getPosition().x + (bestEver[i].getRadius()), bestEver[i].getPosition().y + (bestEver[i].getRadius()));
			bestLines[i].color = sf::Color::Magenta; 
		}
		window.draw(lines); // draws lines between cities;
		window.draw(bestLines); 
		//tells the window that its done rendering 
		window.display(); 

		int i = (int)(rand() % cities.size()); 
		int j = (int)(rand() % cities.size()); 
		swap(cities, i, j); 
		float d =calcDistance(cities); 
		 if (d < recordDistance)
		 {
			 recordDistance = d; 
			 bestEver = cities; //copy(bestEver, cities); 
			 std::cout << recordDistance << std::endl; 
		 }

	}
    
	return 0; 
} 





void initCities(std::vector<sf::CircleShape>& cities, int num, int width, int height)
{
	for (int i = 0; i < num; i++)
	{
		// add a circle of random position 
		sf::CircleShape circ(6.f);
		circ.setFillColor(sf::Color::White);
		circ.setPosition((rand() % width - 50) + 50, (rand() % height - 50) + 50);
		cities.push_back(circ);



	}
}
void swap(std::vector<sf::CircleShape>& arr, int i, int j)
{
	sf::CircleShape dummy = arr[i];
	arr[i] = arr[j];
	arr[j] = dummy;
}

float dist(sf::Vector2f first, sf::Vector2f second)
{
	
	return sqrt(pow((second.x-first.x), 2)+pow((second.y-first.y), 2));
}

void copy(std::vector<sf::CircleShape> &copy, std::vector<sf::CircleShape> &original) // copy function may need a little work down the line 
{
	for (int i = 0; i < original.size(); i++)
	{
		copy[i] = original[i]; 
	}
}

float calcDistance(std::vector<sf::CircleShape>& cities)
{	
	float sum = 0; 
	for (int i = 0; i < cities.size()-1; i++) 
	{
		sum += dist(cities[i].getPosition(), cities[i+1].getPosition()); 
	}
	return sum;
}
