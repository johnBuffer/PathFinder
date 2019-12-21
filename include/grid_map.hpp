#pragma once

#include <vector>

struct GridMap
{
	GridMap(uint32_t cell_size_, uint32_t width_, uint32_t height_)
		: cell_size(cell_size_)
		, width(width_)
		, height(height_)
	{

	}

	uint32_t getCellAtt(int32_t x, int32_t y)
	{
		// Edge cases
		if (x < 0 || x >= width) {
			return 0;
		}
		if (y < 0 || y >= height) {
			return 0;
		}

		return map[y * width + x];
	}

	const uint32_t cell_size;
	const uint32_t width;
	const uint32_t height;
	std::vector<uint32_t> map;
};
