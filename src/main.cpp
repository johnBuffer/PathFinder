#include <iostream>
#include "graphic_utils.hpp"
#include "grid_map.hpp"
#include "agent.hpp"
#include "united_solver.hpp"
#include "utils.hpp"
#include <algorithm>
#include <random>


auto rng = std::default_random_engine{};

void check_min(const sf::Vector2i& position, int32_t& min_dist, const Cell*& min_cell, const GridMap& map)
{
	const Cell* cell = map.getCellAt(position.x, position.y);

	if (cell) {
		if (cell->content != Cell::Filled) {
			if (cell->dist_to_target < min_dist || min_dist == -1) {
				min_dist = cell->dist_to_target;
				min_cell = cell;
			}
		}
	}
}


void updateAgent(Agent& agent, const GridMap& map)
{
	const sf::Vector2f agent_position = agent.position();
	const int32_t agent_x = int32_t(agent_position.x);
	const int32_t agent_y = int32_t(agent_position.y);
	const sf::Vector2i current_pos(agent_x / map.cell_size, agent_y / map.cell_size);

	const Cell* min_cell = nullptr;
	int32_t min_dist = -1;

	std::vector<sf::Vector2i> surrounding = {
		sf::Vector2i( 1,  0),
		sf::Vector2i(-1,  0),
		sf::Vector2i( 0,  1),
		sf::Vector2i( 0, -1),
		sf::Vector2i( 1,  1),
		sf::Vector2i( 1, -1),
		sf::Vector2i(-1,  1),
		sf::Vector2i(-1, -1)
	};

	for (const sf::Vector2i& p : surrounding) {
		check_min(current_pos + p, min_dist, min_cell, map);
	}

	if (min_cell) {
		sf::Vector2f next_cell_pos(map.cellCoordToWorldCoord(min_cell));
		sf::Vector2f v = next_cell_pos - agent.position();
		const float length = sqrt(v.x*v.x + v.y*v.y);

		float force = 3000.0f;
		agent.body->accelerate(force * up::Vec2(v.x / length, v.y / length));
	}
}


int main()
{
	const uint32_t WIN_WIDTH = 1000;
	const uint32_t WIN_HEIGHT = 1000;
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "PathFinder");
	//window.setVerticalSyncEnabled(true);

	const float body_radius = 5.0f;
	GridMap map(2*body_radius, 1000/body_radius, 1000/body_radius);
	up::UnitedSolver solver(up::Vec2(1000, 1000), body_radius, up::Vec2(0, 0));


	std::vector<Agent> crowd;
	for (int i(100); i--;) {
		const up::Vec2 position = up::Vec2(rand() % 900 + 50, rand() % 900 + 50);
		auto agent_body = solver.addBody(position);
		agent_body->friction = 1000.0f;
		crowd.emplace_back(agent_body);
	}

	sf::CircleShape agent_shape(body_radius);
	agent_shape.setOrigin(body_radius, body_radius);
	agent_shape.setFillColor(sf::Color::Yellow);

	bool click = false;
	bool boost = false;
	bool crowd_update = false;
	while (window.isOpen())
	{
		const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed) {
				click = false;
				boost = false;
				if (event.mouseButton.button == sf::Mouse::Left) {
					click = true;
				}
				else {
					boost = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
					click = false;
					boost = false;
			}
			else if (event.type == sf::Event::KeyReleased) {
				crowd_update = !crowd_update;
			}
		}

		if (!click && !boost) {
			map.compute_dist_to(mouse_position.x / map.cell_size, mouse_position.y / map.cell_size);
		}
		else if (click)
		{
			const int32_t cell_x = mouse_position.x / map.cell_size;
			const int32_t cell_y = mouse_position.y / map.cell_size;

			if (map.getContentAt(cell_x, cell_y) != Cell::Filled) {
				map.setCellAt(cell_x, cell_y, Cell::Filled);
				auto b = solver.addBody(up::Vec2((cell_x + 0.5f) * map.cell_size, (cell_y + 0.5f) * map.cell_size));
				b->moving(false);
			}
		}

		window.clear();

		draw_grid(map, window);
		
		if (crowd_update) {
			for (Agent& agent : crowd) {
				updateAgent(agent, map);
			}
			solver.update(0.007);
		}

		/*for (const up::Body& body : solver.getBodies())
		{
			const auto& p = body.position();
			agent_shape.setFillColor(sf::Color::Green);
			agent_shape.setPosition(p.x, p.y);
			window.draw(agent_shape);
		}*/

		for (Agent& agent : crowd)
		{
			agent_shape.setFillColor(sf::Color::Yellow);
			agent_shape.setPosition(agent.position());
			window.draw(agent_shape);
		}

		window.display();
	}

	return 0;
}