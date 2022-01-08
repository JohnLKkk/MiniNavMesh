#include "Geometry.h"

float Geometry::getPointSegmentDistanceSq(int px, int py, int ax, int ay, int bx, int by)
{
	// 参考:http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
	// 该算法的目标是在线段 AB 上找到离 P 最近的点，然后计算 P 到该点的距离。
	
	const float deltaABx = bx - ax;
	const float deltaABy = by - ay;
	const float deltaAPx = px - ax;
	const float deltaAPy = py - ay;

	const float segmentABLengthSq =
		deltaABx * deltaABx + deltaABy * deltaABy;

	if (segmentABLengthSq == 0)
		// AB 不是线段。 所以只需将 P 到 A(或到B)的距离返回即可
		return deltaAPx * deltaAPx + deltaAPy * deltaAPy;

	const float u =
		(deltaAPx * deltaABx + deltaAPy * deltaABy) / segmentABLengthSq;

	if (u < 0)
		// 线 AB 上的最近点在 AB 线段之外，这个点离 A 更近。所以返回P 到 A的距离即可
		return deltaAPx * deltaAPx + deltaAPy * deltaAPy;
	else if (u > 1)
		// 反之返回P 到 B的距离即可
		return (px - bx) * (px - bx) + (py - by) * (py - by);

	// 线 AB 上的最近点在线段 AB 内。 所以找到 AB 上的确切点并计算从它到 P 的距离Sq。
	// 括号内计算的是线段上点的位置。
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
		// AB 不是线段。 所以只需将 P 到 A(或到B)的距离返回即可
		return deltaAPx * deltaAPx
		+ deltaAPy * deltaAPy
		+ deltaAPz * deltaAPz;

	float u = (deltaABx * deltaAPx
		+ deltaABy * deltaAPy
		+ deltaABz * deltaAPz) / segmentABDistSq;

	if (u < 0)
		// 线 AB 上的最近点在 AB 线段之外，这个点离 A 更近。所以返回P 到 A的距离即可
		return deltaAPx * deltaAPx
		+ deltaAPy * deltaAPy
		+ deltaAPz * deltaAPz;
	else if (u > 1)
		// 反之返回P 到 B的距离即可
		return (px - bx)*(px - bx)
		+ (py - by)*(py - by)
		+ (pz - bz)*(pz - bz);


	// 线 AB 上的最近点在线段 AB 内。 所以找到 AB 上的确切点并计算从它到 P 的距离Sq。
	// 括号内计算的是线段上点的位置。
	const float deltaX = (ax + u * deltaABx) - px;
	const float deltaY = (ay + u * deltaABy) - py;
	const float deltaZ = (az + u * deltaABz) - pz;

	return deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
}

bool Geometry::segmentsIntersect(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
	// 代码参考2D线相交测试
	int deltaABx = bx - ax;
	int deltaABy = by - ay;
	int deltaCAx = ax - cx;
	int deltaCAy = ay - cy;
	int deltaCDx = dx - cx;
	int deltaCDy = dy - cy;

	int numerator = (deltaCAy * deltaCDx) - (deltaCAx * deltaCDy);
	int denominator = (deltaABx * deltaCDy) - (deltaABy * deltaCDx);

	// 早期测试
	if (denominator == 0 && numerator != 0)
	{
		// 如果分子为零，则线是共线的。 
		// 如果不是，那么这些线必须是平行的。
		return false;
	}

	// 注意:直线与线段的区别,线相交并非一定线段相交
	// 所以在这里进一步判断是否线段相交
	// 通过分母的转换强制这两个浮点除法。
	float factorAB = numerator / (float)denominator;
	float factorCD = ((deltaCAy * deltaABx) - (deltaCAx * deltaABy))
		/ (float)denominator;

	// 确定交点的类型
	if ((factorAB >= 0.0f)
		&& (factorAB <= 1.0f)
		&& (factorCD >= 0.0f)
		&& (factorCD <= 1.0f))
	{
		return true;  // 两线段相交。
	}

	// 线本身相交，但线段不相交。

	return false;
}
