#pragma once

#include <vector>
#include <list>


struct Cell
{
	enum CellContent
	{
		Undefined,
		Empty,
		Filled,
		Target
	};

	Cell(CellContent content_ = CellContent::Empty, sf::Vector2i position_ = sf::Vector2i(0, 0))
		: content(content_)
		, position(position_)
		, dist_to_target(-1)
	{}

	CellContent content;
	sf::Vector2i position;
	int32_t dist_to_target;
};


struct GridMap
{
	GridMap(uint32_t cell_size_, uint32_t width_, uint32_t height_)
		: cell_size(cell_size_)
		, width(width_)
		, height(height_)
	{
		for (uint32_t y(0U); y < height; ++y) {
			for (uint32_t x(0U); x < width; ++x) {
				map.emplace_back(Cell::Empty, sf::Vector2i(x, y));
			}
		}
	}

	void setCellAt(int32_t x, int32_t y, Cell::CellContent value)
	{
		if (isValidCell(x, y)) {
			map[y * width + x].content = value;
		}
	}

	Cell::CellContent getContentAt(int32_t x, int32_t y) const
	{
		if (!isValidCell(x, y)) {
			return Cell::Undefined;
		}
	
		return map[y * width + x].content;
	}

	Cell* getCellAt(int32_t x, int32_t y)
	{
		if (!isValidCell(x, y)) {
			return nullptr;
		}

		return &map[y * width + x];
	}

	const Cell* getCellAt(int32_t x, int32_t y) const
	{
		if (!isValidCell(x, y)) {
			return nullptr;
		}

		return &map[y * width + x];
	}

	const Cell* getCellAt(const sf::Vector2i& position) const
	{
		if (!isValidCell(position.x, position.y)) {
			return nullptr;
		}

		return &map[position.y * width + position.x];
	}

	bool isValidCell(int32_t x, int32_t y) const
	{
		if (x < 0 || uint32_t(x) >= width) {
			return false;
		}
		if (y < 0 || uint32_t(y) >= height) {
			return false;
		}
		return true;
	}

	sf::Vector2f cellCoordToWorldCoord(int32_t x, int32_t y) const
	{
		return sf::Vector2f((x + 0.5f) * cell_size, (y + 0.5f) * cell_size);
	}

	sf::Vector2f cellCoordToWorldCoord(const Cell* cell) const
	{
		return cellCoordToWorldCoord(cell->position.x, cell->position.y);
	}

	void check_cell(const sf::Vector2i& pos, int32_t dist, std::list<Cell*>& to_visit)
	{
		Cell* cell = getCellAt(pos.x, pos.y);
		if (cell) {
			if (cell->content != Cell::Filled) {
				if (cell->dist_to_target == -1 || cell->dist_to_target > dist + 1) {
					cell->dist_to_target = dist + 1;
					to_visit.push_back(cell);
				}
			}
		}
	}

	void compute_dist_to(uint32_t x, uint32_t y)
	{
		for (Cell& cell : map) {
			cell.dist_to_target = -1;
		}

		std::list<Cell*> to_visit;
		Cell* start_cell = getCellAt(x, y);
		if (start_cell) {
			start_cell->dist_to_target = 0;
			to_visit.push_back(start_cell);
			while (!to_visit.empty()) {
				Cell* cell = to_visit.front();
				const int32_t current_dist = cell->dist_to_target;
				const sf::Vector2i& current_pos(cell->position);

				std::vector<sf::Vector2i> surrounding = {
					sf::Vector2i( 1,  0),
					sf::Vector2i(-1,  0),
					sf::Vector2i( 0,  1),
					sf::Vector2i( 0, -1),
				};

				for (const sf::Vector2i& offset : surrounding) {
					check_cell(current_pos + offset, current_dist, to_visit);
				}

				to_visit.pop_front();
			}
		}
	}

	const uint32_t cell_size;
	const uint32_t width;
	const uint32_t height;
	std::vector<Cell> map;
};
