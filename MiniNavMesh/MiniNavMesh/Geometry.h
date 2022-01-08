#pragma once
#ifndef _GEOMETRY_H_

/*
����Geometry�Ľṹ��ʵ����������ṩ�˼�����ѧ����ȫ�ֺ�������û�а�װ�κ�Mesh���ݡ�
����:JohnKkk
����:2022��1��8��11��22��
*/
class Geometry
{
public:
	Geometry() {};
	~Geometry() {};

	/*
	���ص�(px,py)���߶�a-b�ľ��롣
	*/
	static float getPointSegmentDistanceSq(int px, int py, int ax, int ay, int bx, int by);

	/*
	���ص�(px,py,pz)���߶�a-b�ľ��롣
	*/
	static float getPointSegmentDistanceSq(float px, float py, float pz, float ax, float ay, float az, float bx, float by, float bz);

	/*
	����߶� a-b ���߶� c-d ���κη�ʽ�ཻ���򷵻� TRUE��
	*/
	static bool segmentsIntersect(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy);
private:

};

#endif // !_GEOMETRY_H_
