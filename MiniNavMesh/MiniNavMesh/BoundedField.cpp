#include "BoundedField.h"
#include <memory>
#include <cfloat>
#include <cmath>

BoundedField::BoundedField()
{
	resetBounds();
	// 在更新边界后再调用
	resetCellInfo();
}

BoundedField::BoundedField(float cellSize, float cellHeight)
{
	this->cellSize = std::fmax(cellSize, FLT_MIN);
	this->cellHeight = std::fmax(cellHeight, FLT_MIN);
	calculateWidthDepth();
}

BoundedField::BoundedField(const float * const gridBoundsMin, const float * const gridBoundsMax, float cellSize, float cellHeight)
{
	// 这里没有做非空判断,假设传入都是有效值
	memccpy(boundsMin, gridBoundsMin, 0, sizeof(boundsMin));
	memccpy(boundsMax, gridBoundsMax, 0, sizeof(boundsMax));

	this->cellSize = std::fmax(cellSize, FLT_MIN);
	this->cellHeight = std::fmax(cellHeight, FLT_MAX);

	calculateWidthDepth();
}

bool BoundedField::isOverlaps(const float * const boundsMin, const float * const boundsMax) const
{
	bool overlasps = false;

	if (!boundsMin || !boundsMax)return overlasps;

	// AABB包围盒测试
	// 这里可以简化为一个判断,但是这里展开判断各个维度
	overlasps = (this->boundsMin[0] > boundsMax[0] || this->boundsMax[0] < boundsMin[0]) ? false : overlasps;
	overlasps = (this->boundsMin[1] > boundsMax[1] || this->boundsMax[1] < boundsMin[1]) ? false : overlasps;
	overlasps = (this->boundsMin[2] > boundsMax[2] || this->boundsMax[2] < boundsMin[2]) ? false : overlasps;

	return overlasps;
}

int BoundedField::getDirOffsetDepth(int dir)
{
	static const int const offset[]{ 0, 1, 0, -1 };
	return offset[dir & 0x03];
}

int BoundedField::getDirOffsetWidth(int dir)
{
	static const int const offset[]{ -1, 0, 1, 0 };
	// 丢弃所有大于3的位,将dir限制在0-3。
	return offset[dir&0x03];
}

int BoundedField::gridIndex(int widthIndex, int depthIndex) const
{
	//-----------------------------------------------------------------------------------
	// 设计说明:
	// 在迭代过程中计算gridIndex时,虽然可在外部控制widthIndex和depthIndex在指定范围内,
	// 但仍然存在潜在越界bug,虽然去除下面代码会提高生成性能,但仍然添加如下代码防止潜在
	// bug.
	//-----------------------------------------------------------------------------------
	if (widthIndex < 0 || depthIndex < 0 || widthIndex >= width || depthIndex >= depth) {
		return -1;
	}
	return widthIndex * depth + depthIndex;
}

void BoundedField::resetBounds()
{
	memset(boundsMin, 0, sizeof(boundsMin));
	memset(boundsMax, 1, sizeof(boundsMax));
	calculateWidthDepth();
}

void BoundedField::setBounds(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
	boundsMin[0] = xmin;
	boundsMin[1] = ymin;
	boundsMin[2] = zmin;
	boundsMax[0] = xmax;
	boundsMax[1] = ymax;
	boundsMax[2] = zmax;
	calculateWidthDepth();
}

void BoundedField::setBounds(const float * const mins, const float * const maxs)
{
	if (!mins || !maxs)return;
	memccpy(boundsMin, mins, 0, sizeof(boundsMin));
	memccpy(boundsMax, maxs, 0, sizeof(boundsMax));
	calculateWidthDepth();
}

void BoundedField::setBoundsMax(const float * const values)
{
	if (values) {
		memccpy(boundsMax, values, 0, sizeof(boundsMax));
		calculateWidthDepth();
	}
}

void BoundedField::setBoundsMin(const float * const values)
{
	if (values) {
		memccpy(boundsMin, values, 0, sizeof(boundsMin));
		calculateWidthDepth();
	}
}

void BoundedField::resetCellInfo()
{
	cellSize = cellHeight = .1f;
	calculateWidthDepth();
}

void BoundedField::setCellSize(float value)
{
	cellSize = std::fmax(value, FLT_MIN);
	calculateWidthDepth();
}

void BoundedField::setCellHeight(float value)
{
	cellHeight = std::fmax(value, FLT_MIN);
}

void BoundedField::calculateWidthDepth()
{
	width = int((boundsMax[0] - boundsMin[0]) / cellSize + .5f);
	depth = int((boundsMax[2] - boundsMin[2]) / cellSize + .5f);
}
