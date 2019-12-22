#pragma once

#include <SFML/Graphics.hpp>
#include "grid_map.hpp"
#include "united_solver.hpp"


struct Agent
{
	Agent(up::BodyPtr body_)
		: body(body_)
	{}

	const sf::Vector2f& position() const
	{
		return sf::Vector2f(body->position().x, body->position().y);
	}

	up::BodyPtr body;
};
