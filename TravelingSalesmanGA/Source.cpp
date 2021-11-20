#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include "Functions.h"

#define POPSIZE 500  // popsize  ; could be smaller but oh well 
// honestly should maybe mutate all the new people and only keep a little left alone 
#define CRATIO  100 // ratio for crossover 
#define MRATIO 300 // ratio for mutation
#define RRATIO 50// ratio for random 
#define KRATIO 75  // ratio to keep from the previous gen; all these should add up to the popsize;
auto rng = std::default_random_engine {}; // rand eng for shuffling  
int gen = 0; 
struct Chrom
{
	std::vector<sf::CircleShape> order; //gonna hold the cities in its own order 
	float fitness; 

	Chrom(std::vector<sf::CircleShape>& cities, bool rand )// if you want it rand 
	{
		this->order = cities;
		std::shuffle(std::begin(this->order), std::end(this->order), rng);
		this->fitness = fit(calcDistance(this->order));

	}
	Chrom(std::vector<sf::CircleShape> &cities)
	{
		this->order = cities; 
		this->fitness = fit(calcDistance(this->order)); 
	}

	Chrom(Chrom parent1, Chrom parent2) // this is the constructor for crossover 
	{
		// follow coding train crossover vid 
		// take first half from first then rest from second following their order of how they appear 
		

		int firstHalf = (int )(parent1.order.size())/2;
		int secondHalf = (int)(parent2.order.size() - firstHalf); 
		for (int i = 0; i < firstHalf; i++)
		{
			this->order.push_back(parent1.order[i]); 
		}
		//second half 
		for (int i = 0; i < firstHalf ; i++)
		{
			// if it isn't already in the list, then add it 
			
			for (int j = 0; j < parent2.order.size(); j++)
			{
				if (!(equals(this->order[i], parent2.order[j])))
				{
					this->order.push_back(parent2.order[j]); 
				}
			}
		}

		// just calc fit right here 
		this->fitness = this->fit(calcDistance(this->order)); 

	}
	float fit(float distance)
	{// the bigger the distance the less the fit is 
		// i want to square the distance to make a bigger difference between closer values;  might not be necessary 
		return 1.f / pow(distance, 2);
	}



	bool operator < (const Chrom chrom) const // makes the chrom objects comparable to others 
	{
		return (fitness > chrom.fitness);   // this will make it sort in descending order
	}
};
void initRandPop(std::vector<Chrom>& pop, int size, std::vector<sf::CircleShape> &cities);
void newPop(std::vector<Chrom>& pop);  // makes a new pop by mutating the old one and following the ratios

int main()
{
	int width = 600, height = 600; 
	double recordDistance; 

	std::vector<sf::CircleShape> bestEver ; 
	srand(time(NULL)); 
	const int numCities = 12; 
	sf::VertexArray lines(sf::LinesStrip, numCities);
	sf::VertexArray bestLines(sf::LinesStrip, numCities); 
	
	// each city should have a rand x and a rand y 
	std::vector<sf::CircleShape> cities; 
	initCities(cities , numCities, width, height); 
	std::vector<Chrom> population;  
	initRandPop(population, POPSIZE, cities); 
	

	
	//recordDistance = calcDistance(cities); // calculates the distance of the cities in its first state 
	//bestEver = cities; //copy(bestEver, cities ); // copies the cities to the best ever 
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

		// where the population will be determined 
		// one gen per frame 
		// new pop 

		// sort with fit 

		// return best 
		
		
		//mutate

		newPop(population); 
		
		//render 
		window.clear(sf::Color::Black); 
		
		for (int i = 0; i < cities.size(); i++)
		{
			window.draw(population[0].order[i]); 
			lines[i] = sf::Vector2f(population[0].order[i].getPosition().x + (population[0].order[i].getRadius()), population[0].order[i].getPosition().y + (population[0].order[i].getRadius()));
			lines[i].color = sf::Color::Magenta; 
		}
		window.draw(lines); // draws lines between cities;
		
		//tells the window that its done rendering 
		window.display(); 


		
		
		
		
		// random 
		/*int i = (int)(rand() % cities.size()); 
		int j = (int)(rand() % cities.size()); 
		swap(cities, i, j); 
		float d =calcDistance(cities); 
		 if (d < recordDistance)
		 {
			 recordDistance = d; 
			 bestEver = cities; //copy(bestEver, cities); 
			 std::cout << recordDistance << std::endl; 
		 }*/

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

void newPop(std::vector<Chrom>& pop) // makes a new pop by mutating the old one and following the ratios

{
	gen++; 
	/* keeps the top number of the previous (KRATIO)
	* makes new group from crossover CRATIO 
	* makes new group from mutation MRATIO
	* makes a small group rand for variety in each generation 
	*/
	std::vector<Chrom> temp; 

	for (int i = 0; i<KRATIO; i++)
	{
		temp.push_back(pop[i]); 
	} // now temp has KRATIO number of the top chroms 

	std::vector<Chrom> bestFromLast = temp; 
	for (int i = 0; i < CRATIO; i++)
	{
		//crossover 
		int randFirst = (int)(rand() % bestFromLast.size()); 
		int randSecond = (int)(rand() % bestFromLast.size());
		// make a new chrom from the crossover of the two rand best ; 
		// then add it to the full list 
		temp.push_back(Chrom(bestFromLast[randFirst], bestFromLast[randSecond])); 


		

	}

	// cross wit mutate
	 
	for (int i = 0; i < MRATIO; i++)
	{
		//crossover 
		int randFirst = (int)(rand() % bestFromLast.size());
		int randSecond = (int)(rand() % bestFromLast.size());
		// make a new chrom from the crossover of the two rand best ; 
		Chrom child(bestFromLast[randFirst], bestFromLast[randSecond]); 

		int rf = (int)(rand() % child.order.size()); 
		int rs = (int)(rand() % child.order.size()); 
		swap(child.order, rs, rf);
		


	}

	//rand 

	for (int i = 0; i < RRATIO; i++)
	{
		temp.push_back(Chrom(temp[0].order, true));  // just a rand order;  
	}




std:sort(temp.begin(), temp.end()); 
	pop = temp; 

	std::cout << "GEN: " << gen << " BEST FIT: " << pop[0].fitness <<std::endl ; 
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

void initRandPop(std::vector<Chrom>& pop, int size, std::vector<sf::CircleShape> &cities ) // this is the first gen 
{

	
	


	for (int i = 0; i < size; i++)
	{
		std::shuffle(std::begin(cities), std::end(cities), rng); 
		Chrom temp(cities); 
		// find the fitness 
		

		
		 pop.push_back(temp); 
	}
	// sort pop in descending order 
	std::sort(pop.begin(), pop.end()); 


}



bool equals(sf::CircleShape first, sf::CircleShape second)
{

	if (first.getPosition().x == second.getPosition().x && first.getPosition().y == second.getPosition().y)
	{
		return true;
	}
	
	
	return false;
}
