#pragma once
#ifndef _GEOMETRY_H_

/*
描述Geometry的结构，实际上这里仅提供了几个数学计算全局函数，并没有包装任何Mesh数据。
作者:JohnKkk
日期:2022年1月8日11点22分
*/
class Geometry
{
public:
	Geometry() {};
	~Geometry() {};

	/*
	返回点(px,py)到线段a-b的距离。
	*/
	static float getPointSegmentDistanceSq(int px, int py, int ax, int ay, int bx, int by);

	/*
	返回点(px,py,pz)到线段a-b的距离。
	*/
	static float getPointSegmentDistanceSq(float px, float py, float pz, float ax, float ay, float az, float bx, float by, float bz);

	/*
	如果线段 a-b 与线段 c-d 以任何方式相交，则返回 TRUE。
	*/
	static bool segmentsIntersect(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy);
private:

};

#endif // !_GEOMETRY_H_
