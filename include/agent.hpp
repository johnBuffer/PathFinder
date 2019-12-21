#pragma once

#include <SFML/Graphics.hpp>
#include "grid_map.hpp"


struct Agent
{
	Agent(const sf::Vector2f& position_ = sf::Vector2f(0.0f, 0.0f))
		: position(position_)
		, velocity(0.0f, 0.0f)
	{}

	void update()
	{
		position += velocity;
	}

	sf::Vector2f position;
	sf::Vector2f velocity;
};
