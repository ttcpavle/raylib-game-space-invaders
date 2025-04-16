#pragma once
#include "block.h"
#include <vector>

class Obstacle {
public:
	Obstacle(Vector2 position);
	void Draw();
	Vector2 position;
	std::vector<Block> blocks;
	static std::vector<std::vector<int>> grid;

private:
};