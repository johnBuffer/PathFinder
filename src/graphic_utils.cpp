#include "..\include\graphic_utils.hpp"

void draw_grid(const GridMap& map, sf::RenderTarget& target)
{
	const uint32_t width  = map.width;
	const uint32_t height = map.height;
	const uint32_t vertex_count = width * height;

	sf::VertexArray va(sf::Quads, 4 * vertex_count);

	uint32_t vertex_index = 0U;
	for (uint32_t x(0U); x<width; ++x) {
		for (uint32_t y(0U); y < height; ++y) {
			const float cell_size = static_cast<float>(map.cell_size);
			const float vertex_x = static_cast<float>(cell_size * x);
			const float vertex_y = static_cast<float>(cell_size * y);

			va[4 * vertex_index + 0].position = sf::Vector2f(vertex_x, vertex_y);
			va[4 * vertex_index + 1].position = sf::Vector2f(vertex_x + cell_size, vertex_y);
			va[4 * vertex_index + 2].position = sf::Vector2f(vertex_x + cell_size, vertex_y + cell_size);
			va[4 * vertex_index + 3].position = sf::Vector2f(vertex_x, vertex_y + cell_size);

			sf::Color color = sf::Color::Red;
			if (map.getContentAt(x, y) == Cell::Filled) {
				color = sf::Color::Green;
			} else if (map.getContentAt(x, y) == Cell::Target) {
				color = sf::Color::Red;
			} else if (map.getCellAt(x, y)->dist_to_target > -1) {
				const float dist = 0.5f * map.getCellAt(x, y)->dist_to_target;
				color = sf::Color(dist, dist, dist);
			}

			va[4 * vertex_index + 0].color = color;
			va[4 * vertex_index + 1].color = color;
			va[4 * vertex_index + 2].color = color;
			va[4 * vertex_index + 3].color = color;

			++vertex_index;
		}
	}

	target.draw(va);
}
