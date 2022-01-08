#pragma once
#ifndef _BUILDER_H_

#include "BlockHeightfield.h"
#include <cmath>
#define M_PI 3.14159265358979323846
/*
��������ڽ����ػ���ĳ�����Ϣ����ʵ��voxel(���貿��)��¼Ϊһ��span�������ݡ��ɿ�Խ��span������������Ч��OpenHeightField��
����:JohnKkk
����:2022��1��8��22��13��
*/
class BlockHeightfieldBuilder
{
public:
	BlockHeightfieldBuilder(float cellSize = 1, float cellHeight = 1, int minTraversableHeight = 1, int maxTraversableStep = 1, float maxTraversableSlope = 0.4f, bool clipLedges = false):cellSize(cellSize), cellHeight(cellHeight), minTraversableHeight(minTraversableHeight), maxTraversableStep(maxTraversableStep) {
		maxTraversableSlope = std::fmin(85, std::fmax(0, maxTraversableSlope));


		/*
		������С y ���ߡ�
		�����ο���http://mathworld.wolfram.com/DihedralAngle.html
		ͨ��ȷ���ڼ���֮ǰ�� n1 �� n2 ���й�һ����
		�ο������еķ�ĸ����Ϊ 1 ��
		���Զ��������Բο����̼�Ϊ...

		cos theta = n1 dot n2

		ʹ�ã�
		n1 = (0, 1, 0)����ʾ (x,z) ƽ���ϵ�ƽ�档��
		n2 = (x, y, z) ��һ���� ������ƽ���ϵı��档��
		
		�򻯲���� y��
		cos theta = 0x + 1y + 0z
		y = cos theta
		
		����Ħȡ���maxTraversableSlopeת��Ϊ���Ⱥ��ֵ��
		��������֪�������¶Ƚǵ� y �����Ƕ��١������
		���ε� y ����С���������� y ���ߣ���ô�Ϳ���֪���Ѿ����������߽Ƕȡ�
		*/
		minNormalY = float(std::cosf(std::fabsf(maxTraversableSlope) / 180 * M_PI));
	};
	~BlockHeightfieldBuilder() {};

	/*
	���ṩ��Դ������������BlockHeightfield,BlockHeightfield����ʾԴ�������ݡ����衱��
	���ػ�������
	*/
	BlockHeightfield* build(const float *const vertices, const int *const indices);
private:

	// һ�����ò���
	//------------------------------------------------------------------
	// ���˵��:
	// ͨ������ʹ��NavMesh��ϵͳ�У���ῴ�����Խ�������һ�����������
	// ���������������NavMesh���ݣ���Щ������һ����ȫ������NavMeshϵͳ
	// һ�£��������Ǻܴ�̶�����������ͬ����ʹ���˸������������㷨��
	// �ṩ�˶���Ĳ�����
	// �ܵ���˵���кܴ�һ���ֲ���Ӧ���ǹ��еģ�����cellSize��cellHeight
	// �Լ�maxTraversableSlope�Ȳ�����
	//------------------------------------------------------------------


	const bool clipLedges;
	const int minTraversableHeight;
	const int maxTraversableStep;

	// ��ʾ����α���Ϊ�ɱ����������С����Yֵ
	// ���������maxTraversableSlope(���б����)����õ�
	float minNormalY;

	// ���ڳ�ʼ��Heightfield,����,Heightfield�������ɹ����е���������cell����Ч��Χ,���,�ò���
	// �������ڳ�ʼ��Heightfield�׶�
	const float cellSize;

	// ���ڳ�ʼ��Heightfield,����,Hiehgtfield�������ɹ����е���������cell����Ч��Χ,���,�ò���
	// �������ڳ�ʼ��Heightfield�׶�
	const float cellHeight;

};

#endif // !_BUILDER_H_
