#include "block.h"

Block::Block(Vector2 position)
{
	this->position = position;
}

void Block::Draw()
{
	DrawRectangleV(position, {4, 4}, {243, 216, 63, 255});
}

Rectangle Block::getRect(){
	return {position.x, position.y, 4, 4};
}