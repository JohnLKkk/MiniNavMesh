#pragma once
#ifndef _OPENHEIGHTSPAN_H_
#define _OPENHEIGHTSPAN_H_

/*
OpenHeightSpan用于记录OpenHeightField中每个cell的可跨越span。
作者:JohnKkk
日期:2022年1月8日
*/
class OpenHeightSpan
{
public:
	//--------------------------------------------------------------
	// 设计说明:
	// 这个类与HeightSpan基本一致,可以使用HeightSpan统一处理实体Span
	// 与可跨越Span，但是这里将它们分开，以便能够独立理解这个类，
	// 由于OpenHeight由最低点和最高点组成一系列Spane，因此这里使用
	// Floor(地板)和Ceilling(天花板)来描述一个OpenHeightSpan
	//--------------------------------------------------------------


	OpenHeightSpan(int floor, int height):floor(floor), height(height), next(nullptr), neighborConnection0(nullptr), neighborConnection1(nullptr), neighborConnection2(nullptr), neighborConnection3(nullptr) {
		if (floor < 0 || height < 1) {
			// 错误,在这里记录下
		}
	};
	~OpenHeightSpan();

	// 一个空区域的spans被认为是不可遍历的
	static const int NULL_REGION = 0;

	// 一个临时变量,用于作为缓存记录临时状态
	// 在使用后应该置0
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
	返回当前cell当前span的天花板高度
	*/
	int ceiling()const { return floor + height; }

	/*
	此span与最近的heightfield边界的距离。
	*/
	int getDistanceToBorder()const { return distanceToBorder; };

	int getDistanceToRegionCore()const { return distanceToRegionCore; };

	int getFloor()const { return floor; };

	/*
	地板以上到天花板的高度,这定义了当前地板是否可以行走。
	*/
	int getHeight()const { return height; };

	/*
	返回邻居。
	*/
	OpenHeightSpan* getNeighbor(int direction);
private:

};


#endif // !_OPENHEIGHTSPAN_H_
