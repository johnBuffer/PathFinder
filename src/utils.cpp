#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>


float length(const sf::Vector2f& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}
