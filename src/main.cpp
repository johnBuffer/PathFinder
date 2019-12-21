#include <iostream>
#include "graphic_utils.hpp"
#include "grid_map.hpp"


int main()
{
	const uint32_t WIN_WIDTH = 1000;
	const uint32_t WIN_HEIGHT = 1000;
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "PathFinder");

	GridMap map(50, 20, 20);
	map.setCellAt(6, 5, Cell::Filled);
	map.setCellAt(7, 5, Cell::Filled);
	map.setCellAt(8, 5, Cell::Filled);
	map.setCellAt(9, 5, Cell::Filled);
	map.setCellAt(10, 5, Cell::Filled);
	map.setCellAt(10, 6, Cell::Filled);
	map.setCellAt(10, 7, Cell::Filled);
	map.setCellAt(10, 8, Cell::Filled);
	map.setCellAt(10, 9, Cell::Filled);
	map.setCellAt(10, 10, Cell::Filled);
	map.setCellAt(10, 11, Cell::Filled);
	map.setCellAt(10, 12, Cell::Filled);
	map.setCellAt(10, 13, Cell::Filled);
	map.setCellAt(10, 14, Cell::Filled);
	map.setCellAt(10, 15, Cell::Filled);
	map.setCellAt(9, 15, Cell::Filled);
	map.setCellAt(8, 15, Cell::Filled);
	map.setCellAt(7, 15, Cell::Filled);
	map.setCellAt(6, 15, Cell::Filled);

	map.setCellAt(19, 10, Cell::Target);

	map.compute_dist_to(19, 10);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		draw_grid(map, window);

		window.display();
	}

	return 0;
}