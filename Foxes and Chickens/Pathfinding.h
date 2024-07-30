#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

std::vector<sf::Vector2i> bfsPathfinding(const sf::Vector2i& start, const sf::Vector2i& goal, int rows, int cols, const std::vector<std::vector<bool>>& gridOccupancy);
