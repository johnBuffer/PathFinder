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

	void compute_dist_to(uint32_t x, uint32_t y)
	{
		for (Cell& cell : map)
		{
			cell.dist_to_target = -1;
		}

		std::list<Cell*> to_visit;
		Cell* start_cell = getCellAt(x, y);
		if (start_cell)
		{
			start_cell->dist_to_target = 0;
			to_visit.push_back(start_cell);
			while (!to_visit.empty())
			{
				Cell* cell = to_visit.front();
				const int32_t current_dist = cell->dist_to_target;
				const sf::Vector2i& current_pos(cell->position);

				// Top
				Cell* top_cell = getCellAt(current_pos.x, current_pos.y - 1);
				if (top_cell) {
					if (top_cell->content != Cell::Filled) {
						if (top_cell->dist_to_target == -1 || top_cell->dist_to_target > current_dist + 1) {
							top_cell->dist_to_target = current_dist + 1;
							to_visit.push_back(top_cell);
						}
					}
				}

				// Bot
				Cell* bot_cell = getCellAt(current_pos.x, current_pos.y + 1);
				if (bot_cell) {
					if (bot_cell->content != Cell::Filled) {
						if (bot_cell->dist_to_target == -1 || bot_cell->dist_to_target > current_dist + 1) {
							bot_cell->dist_to_target = current_dist + 1;
							to_visit.push_back(bot_cell);
						}
					}
				}

				// Left
				Cell* left_cell = getCellAt(current_pos.x - 1, current_pos.y);
				if (left_cell) {
					if (left_cell->content != Cell::Filled) {
						if (left_cell->dist_to_target == -1 || left_cell->dist_to_target > current_dist + 1) {
							left_cell->dist_to_target = current_dist + 1;
							to_visit.push_back(left_cell);
						}
					}
				}

				// Right
				Cell* right_cell = getCellAt(current_pos.x + 1, current_pos.y);
				if (right_cell) {
					if (right_cell->content != Cell::Filled) {
						if (right_cell->dist_to_target == -1 || right_cell->dist_to_target > current_dist + 1) {
							right_cell->dist_to_target = current_dist + 1;
							to_visit.push_back(right_cell);
						}
					}
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
