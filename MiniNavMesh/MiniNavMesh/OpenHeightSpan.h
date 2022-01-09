#pragma once
#ifndef _OPENHEIGHTSPAN_H_
#define _OPENHEIGHTSPAN_H_

/*
OpenHeightSpan���ڼ�¼OpenHeightField��ÿ��cell�Ŀɿ�Խspan��
����:JohnKkk
����:2022��1��8��
*/
class OpenHeightSpan
{
public:
	//--------------------------------------------------------------
	// ���˵��:
	// �������HeightSpan����һ��,����ʹ��HeightSpanͳһ����ʵ��Span
	// ��ɿ�ԽSpan���������ｫ���Ƿֿ����Ա��ܹ������������࣬
	// ����OpenHeight����͵����ߵ����һϵ��Spane���������ʹ��
	// Floor(�ذ�)��Ceilling(�컨��)������һ��OpenHeightSpan
	//--------------------------------------------------------------


	OpenHeightSpan(int floor, int height):floor(floor), height(height), next(nullptr), neighborConnection0(nullptr), neighborConnection1(nullptr), neighborConnection2(nullptr), neighborConnection3(nullptr) {
		if (floor < 0 || height < 1) {
			// ����,�������¼��
		}
	};
	~OpenHeightSpan();

	// һ���������spans����Ϊ�ǲ��ɱ�����
	static const int NULL_REGION = 0;

	// һ����ʱ����,������Ϊ�����¼��ʱ״̬
	// ��ʹ�ú�Ӧ����0
	int flags = 0;

	int regionID = 0;
	int distanceToRegionCore = 0;
	int distanceToBorder = 0;

	int floor;
	int height;

	OpenHeightSpan * next;
	OpenHeightSpan *neighborConnection0;
	OpenHeightSpan *neighborConnection1;
	OpenHeightSpan *neighborConnection2;
	OpenHeightSpan *neighborConnection3;

	/*
	���ص�ǰcell��ǰspan���컨��߶�
	*/
	int ceiling()const { return floor + height; }

	/*
	��span�������heightfield�߽�ľ��롣
	*/
	int getDistanceToBorder()const { return distanceToBorder; };

	int getDistanceToRegionCore()const { return distanceToRegionCore; };

	int getFloor()const { return floor; };

	/*
	�ذ����ϵ��컨��ĸ߶�,�ⶨ���˵�ǰ�ذ��Ƿ�������ߡ�
	*/
	int getHeight()const { return height; };

	/*
	�����ھӡ�
	*/
	OpenHeightSpan* getNeighbor(int direction);
private:

};


#endif // !_OPENHEIGHTSPAN_H_
