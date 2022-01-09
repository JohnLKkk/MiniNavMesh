#pragma once
#ifndef _REGION_H_
#define _REGION_H_

#include <vector>
#include <iostream>

using std::vector;

/*
用于描述和创建区域(Region)的结构。<br/>
作者:JohnKkk
日期:2022年1月8日09点10分
*/
class Region
{
public:

	// 区域ID
	int id;

	// 该区域的Span数目
	int spanCount;

	// 是否需要更新区域ID
	bool remap;

	// 区域
	// 表示当前区域与其他区域的有序连接表，所有区域至少有一个连接，即使连接到空区域。
	// 一个区域存在多个连接到其他区域的点，比如:1,2,3,1，表示当前区域有有个点连接到区域1。
	// 之所以存储多个连接点，是因为在进行区域合并时，需要测试是否会产生无效多边形。
	vector<int> *const connections;

	// 与当前区域重叠的非空区域
	// 1.如果当前区域的span在高度方向低于另一个区域的span
	// 则认为这些区域重叠
	// 2.如果当前区域存在自我重叠的span，则也存进这里(表示自身重叠)
	vector<int> *const overlappingRegions;


	Region(int id = 0) :id(id), spanCount(0), remap(false), connections(new vector<int>()), overlappingRegions(new vector<int>()) {};

	~Region() {
		delete connections;
		delete overlappingRegions;
	};

	/*
	重置当前区域。
	*/
	void reset(int id) {
		this->id = id;
		spanCount = 0;
		connections->clear();
		overlappingRegions->clear();
	}

	/*
	用于测试。
	*/
	friend std::ostream& operator<<(std::ostream& out, const Region& r) {
		out << "id:" << r.id << ",spans:" << r.spanCount << std::endl;

		if (r.connections->size()) {
			out << "connections:";
			for (auto cts = r.connections->begin(); cts != r.connections->end(); cts++) {
				out << *cts << " ";
			}
			out << std::endl;
		}
		

		if (r.overlappingRegions->size()) {
			out << "overlappingRegions:";
			for (auto olrs = r.overlappingRegions->begin(); olrs != r.overlappingRegions->end(); olrs++) {
				out << *olrs << " ";
			}
			out << std::endl;
		}

		return out;
	}
private:

};

#endif // !_REGION_H_
