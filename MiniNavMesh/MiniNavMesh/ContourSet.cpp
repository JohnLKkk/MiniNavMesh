#include "ContourSet.h"

ContourSet::ContourSet(const float * const gridBoundsMin, const float * const gridBoundsMax, float cellSize, float cellHeight, int initSize):BoundedField(gridBoundsMin, gridBoundsMax, cellSize, cellHeight), contours(new vector<Contour>(initSize))
{
}
