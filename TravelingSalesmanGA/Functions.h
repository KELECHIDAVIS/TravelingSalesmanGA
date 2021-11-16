#pragma once

void initCities(std::vector<sf::CircleShape>& cities, int num, int width, int height); 
void swap(std::vector<sf::CircleShape>& arr, int i, int j);
float dist(sf::Vector2f first, sf::Vector2f second); 
void copy(std::vector<sf::CircleShape>& copy, std::vector<sf::CircleShape>& original); 
float calcDistance(std::vector<sf::CircleShape> &cities); // takes an array of points;  returns distance between them in that order 
float fitness(); // this is going to take in the distance and return a value based on it ; less distance will yeild higher fit 