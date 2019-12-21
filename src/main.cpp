#include <iostream>
#include "graphic_utils.hpp"
#include "grid_map.hpp"
#include "agent.hpp"


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


void computeAgentCellCollision(Agent& agent, const Cell* cell, const GridMap& map)
{
	if (cell) {
		if (cell->content == Cell::Filled) {
			sf::Vector2f cell_pos((cell->position.x + 0.5f) * map.cell_size, (cell->position.y + 0.5f) * map.cell_size);
			sf::Vector2f v = cell_pos - agent.position;
			const float length = sqrt(v.x*v.x + v.y*v.y);

			if (length < 10.0f) {
				agent.position.x -= (10.0f - length) * v.x / length;
				agent.position.y -= (10.0f - length) * v.y / length;
			}
		}
	}
}


void computeAgentsMapCollisions(std::vector<Agent>& agents, const GridMap& map)
{
	for (Agent& agent : agents)
	{
		const sf::Vector2i current_pos(agent.position.x / map.cell_size, agent.position.y / map.cell_size);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(0, 0)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(1, 0)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(-1, 0)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(0, 1)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(0, -1)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(1, 1)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(1, -1)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(-1, 1)), map);
		computeAgentCellCollision(agent, map.getCellAt(current_pos + sf::Vector2i(-1, -1)), map);
	}
}


void computeAgentsCollisions(std::vector<Agent>& agents)
{
	const uint64_t agent_count = agents.size();

	for (uint64_t i(0); i < agent_count; ++i) {
		Agent& b1 = agents[i];
		for (uint64_t k(i+1); k < agent_count; ++k) {
			Agent& b2 = agents[k];

			const float col_radius(10.0f);
			sf::Vector2f col_axe(b1.position - b2.position);
			const float length2(col_axe.x*col_axe.x + col_axe.y*col_axe.y);
			if (length2 < col_radius*col_radius && length2 > 0.01f)
			{
				const float length = sqrt(length2);
				const float delta_col(0.5f * (col_radius - length));

				b1.position.x += delta_col * col_axe.x / length;
				b1.position.y += delta_col * col_axe.y / length;

				b2.position.x -= delta_col * col_axe.x / length;
				b2.position.y -= delta_col * col_axe.y / length;
			}
		}
	}
}


void updateAgent(Agent& agent, const GridMap& map)
{
	const sf::Vector2i current_pos(agent.position.x / map.cell_size, agent.position.y / map.cell_size);

	const Cell* min_cell = nullptr;
	int32_t min_dist = -1;

	check_min(current_pos + sf::Vector2i(1, 0), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(-1, 0), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(0, 1), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(0, -1), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(1, 1), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(1, -1), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(-1, 1), min_dist, min_cell, map);
	check_min(current_pos + sf::Vector2i(-1, -1), min_dist, min_cell, map);

	if (min_cell) {
		sf::Vector2f next_cell_pos((min_cell->position.x + 0.5f) * map.cell_size, (min_cell->position.y + 0.5f) * map.cell_size);
		sf::Vector2f v = next_cell_pos - agent.position;
		const float length = sqrt(v.x*v.x + v.y*v.y);

		agent.velocity.x = v.x / length;
		agent.velocity.y = v.y / length;
	}
}


int main()
{
	const uint32_t WIN_WIDTH = 1000;
	const uint32_t WIN_HEIGHT = 1000;
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "PathFinder");
	window.setVerticalSyncEnabled(true);

	GridMap map(10, 100, 100);

	std::vector<Agent> crowd;
	for (int i(1000); i--;) {
		crowd.emplace_back(sf::Vector2f(rand() % 900 + 50, rand() % 900 + 50));
	}

	sf::CircleShape agent_shape(5);
	agent_shape.setOrigin(5, 5);
	agent_shape.setFillColor(sf::Color::Yellow);

	bool click = false;
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
				click = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				click = false;
			}
			else if (event.type == sf::Event::KeyReleased) {
				crowd_update = !crowd_update;
			}
		}

		if (!click) {
			map.compute_dist_to(mouse_position.x / map.cell_size, mouse_position.y / map.cell_size);
		}
		else {
			map.setCellAt(mouse_position.x / map.cell_size, mouse_position.y / map.cell_size, Cell::Filled);
		}

		window.clear();

		draw_grid(map, window);
		
		if (crowd_update) {
			computeAgentsCollisions(crowd);
			computeAgentsMapCollisions(crowd, map);
			for (Agent& agent : crowd)
			{
				updateAgent(agent, map);
				agent.update();
			}
		}

		for (Agent& agent : crowd)
		{
			agent_shape.setPosition(agent.position);
			window.draw(agent_shape);
		}

		window.display();
	}

	return 0;
}