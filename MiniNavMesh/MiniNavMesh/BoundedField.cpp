#include "BoundedField.h"
#include <memory>
#include <cfloat>
#include <cmath>

BoundedField::BoundedField()
{
	resetBounds();
	// �ڸ��±߽���ٵ���
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
	// ����û�����ǿ��ж�,���贫�붼����Чֵ
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

	// AABB��Χ�в���
	// ������Լ�Ϊһ���ж�,��������չ���жϸ���ά��
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
	// �������д���3��λ,��dir������0-3��
	return offset[dir&0x03];
}

int BoundedField::gridIndex(int widthIndex, int depthIndex) const
{
	//-----------------------------------------------------------------------------------
	// ���˵��:
	// �ڵ��������м���gridIndexʱ,��Ȼ�����ⲿ����widthIndex��depthIndex��ָ����Χ��,
	// ����Ȼ����Ǳ��Խ��bug,��Ȼȥ���������������������,����Ȼ������´����ֹǱ��
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
