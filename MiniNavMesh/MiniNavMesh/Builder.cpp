#include "Builder.h"
#include "BlockHeightfield.h"

BlockHeightfield * BlockHeightfieldBuilder::build(const float * const vertices, const int * const indices)
{
	if (!vertices || !indices)return nullptr;

	// ³õÊ¼»¯ heightfield
	BlockHeightfield *const result = new BlockHeightfield(cellSize, cellHeight);

	const float inverseCellSize = 1 / result->getCellSize();
	const float inverseCellHeight = 1 / result->getCellHeight();

	float xmin = vertices[0];
	float ymin = vertices[1];
	float zmin = vertices[2];
	float xmax = vertices[0];
	float ymax = vertices[1];
	float zmax = vertices[2];

	return nullptr;
}
