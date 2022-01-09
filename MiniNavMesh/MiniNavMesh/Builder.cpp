#include "Builder.h"
#include "BlockHeightfield.h"
#include <cmath>

BlockHeightfield * BlockHeightfieldBuilder::build(int vertNums, const float * const vertices, int indNums, const int * const indices)
{
	if (!vertices || !indices)return nullptr;

	// ��ʼ�� heightfield
	BlockHeightfield *const result = new BlockHeightfield(cellSize, cellHeight);

	const float inverseCellSize = 1 / result->getCellSize();
	const float inverseCellHeight = 1 / result->getCellHeight();

	// ���㳡����Χ��
	float xmin = vertices[0];
	float ymin = vertices[1];
	float zmin = vertices[2];
	float xmax = vertices[0];
	float ymax = vertices[1];
	float zmax = vertices[2];
	for (int i = 3; i < vertNums; i += 3)
	{
		xmax = std::fmax(vertices[i], xmax);
		ymax = std::fmax(vertices[i + 1], ymax);
		zmax = std::fmax(vertices[i + 2], zmax);

		xmin = std::fmin(vertices[i], xmin);
		ymin = std::fmin(vertices[i + 1], ymin);
		zmin = std::fmin(vertices[i + 2], zmin);
	}
	result->setBounds(xmin, ymin, zmin, xmax, ymax, zmax);
	
	// ���Դ��������Щ����ε�б���㹻�ͣ����Ա���Ϊ�ǿɱ����ġ� ��Agent ������б�����������ߡ���

	return nullptr;
}

int * BlockHeightfieldBuilder::genMeshWalkableFlags(int vertNums, const float * const vertiecs, int indNums, const int * const indices)
{
	const int* flags = new int[indNums / 3]();

	// һЩ��ʱ����
	float diffAB[3]{ 0 };
	float diffAC[3]{ 0 };
	float crossDiff[3]{ 0 };


	return nullptr;
}
