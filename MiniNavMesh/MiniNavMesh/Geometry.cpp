#include "Geometry.h"

float Geometry::getPointSegmentDistanceSq(int px, int py, int ax, int ay, int bx, int by)
{
	// �ο�:http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
	// ���㷨��Ŀ�������߶� AB ���ҵ��� P ����ĵ㣬Ȼ����� P ���õ�ľ��롣
	
	const float deltaABx = bx - ax;
	const float deltaABy = by - ay;
	const float deltaAPx = px - ax;
	const float deltaAPy = py - ay;

	const float segmentABLengthSq =
		deltaABx * deltaABx + deltaABy * deltaABy;

	if (segmentABLengthSq == 0)
		// AB �����߶Ρ� ����ֻ�轫 P �� A(��B)�ľ��뷵�ؼ���
		return deltaAPx * deltaAPx + deltaAPy * deltaAPy;

	const float u =
		(deltaAPx * deltaABx + deltaAPy * deltaABy) / segmentABLengthSq;

	if (u < 0)
		// �� AB �ϵ�������� AB �߶�֮�⣬������� A ���������Է���P �� A�ľ��뼴��
		return deltaAPx * deltaAPx + deltaAPy * deltaAPy;
	else if (u > 1)
		// ��֮����P �� B�ľ��뼴��
		return (px - bx) * (px - bx) + (py - by) * (py - by);

	// �� AB �ϵ���������߶� AB �ڡ� �����ҵ� AB �ϵ�ȷ�е㲢��������� P �ľ���Sq��
	// �����ڼ�������߶��ϵ��λ�á�
	const float deltaX = (ax + u * deltaABx) - px;
	const float deltaY = (ay + u * deltaABy) - py;

	return deltaX * deltaX + deltaY * deltaY;
}

float Geometry::getPointSegmentDistanceSq(float px, float py, float pz, float ax, float ay, float az, float bx, float by, float bz)
{
	const float deltaABx = bx - ax;
	const float deltaABy = by - ay;
	const float deltaABz = bz - az;
	float deltaAPx = px - ax;
	float deltaAPy = py - ay;
	float deltaAPz = pz - az;

	const float segmentABDistSq = deltaABx * deltaABx
		+ deltaABy * deltaABy
		+ deltaABz * deltaABz;
	if (segmentABDistSq == 0)
		// AB �����߶Ρ� ����ֻ�轫 P �� A(��B)�ľ��뷵�ؼ���
		return deltaAPx * deltaAPx
		+ deltaAPy * deltaAPy
		+ deltaAPz * deltaAPz;

	float u = (deltaABx * deltaAPx
		+ deltaABy * deltaAPy
		+ deltaABz * deltaAPz) / segmentABDistSq;

	if (u < 0)
		// �� AB �ϵ�������� AB �߶�֮�⣬������� A ���������Է���P �� A�ľ��뼴��
		return deltaAPx * deltaAPx
		+ deltaAPy * deltaAPy
		+ deltaAPz * deltaAPz;
	else if (u > 1)
		// ��֮����P �� B�ľ��뼴��
		return (px - bx)*(px - bx)
		+ (py - by)*(py - by)
		+ (pz - bz)*(pz - bz);


	// �� AB �ϵ���������߶� AB �ڡ� �����ҵ� AB �ϵ�ȷ�е㲢��������� P �ľ���Sq��
	// �����ڼ�������߶��ϵ��λ�á�
	const float deltaX = (ax + u * deltaABx) - px;
	const float deltaY = (ay + u * deltaABy) - py;
	const float deltaZ = (az + u * deltaABz) - pz;

	return deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
}

bool Geometry::segmentsIntersect(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
	// ����ο�2D���ཻ����
	int deltaABx = bx - ax;
	int deltaABy = by - ay;
	int deltaCAx = ax - cx;
	int deltaCAy = ay - cy;
	int deltaCDx = dx - cx;
	int deltaCDy = dy - cy;

	int numerator = (deltaCAy * deltaCDx) - (deltaCAx * deltaCDy);
	int denominator = (deltaABx * deltaCDy) - (deltaABy * deltaCDx);

	// ���ڲ���
	if (denominator == 0 && numerator != 0)
	{
		// �������Ϊ�㣬�����ǹ��ߵġ� 
		// ������ǣ���ô��Щ�߱�����ƽ�еġ�
		return false;
	}

	// ע��:ֱ�����߶ε�����,���ཻ����һ���߶��ཻ
	// �����������һ���ж��Ƿ��߶��ཻ
	// ͨ����ĸ��ת��ǿ�����������������
	float factorAB = numerator / (float)denominator;
	float factorCD = ((deltaCAy * deltaABx) - (deltaCAx * deltaABy))
		/ (float)denominator;

	// ȷ�����������
	if ((factorAB >= 0.0f)
		&& (factorAB <= 1.0f)
		&& (factorCD >= 0.0f)
		&& (factorCD <= 1.0f))
	{
		return true;  // ���߶��ཻ��
	}

	// �߱����ཻ�����߶β��ཻ��

	return false;
}
