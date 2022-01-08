#include "OpenHeightSpan.h"

OpenHeightSpan * OpenHeightSpan::getNeighbor(int direction)
{
	switch (direction)
	{
	case 0:
		return neighborConnection0;
	case 1:
		return neighborConnection1;
	case 2:
		return neighborConnection2;
	case 3:
		return neighborConnection3;
	default:
		break;
	}
	return nullptr;
}
