#pragma once
#ifndef _CONTOUR_H_
#include <vector>
using std::vector;

/*
同时描述Detailed或Simplified两种形式，一个轮廓用一个Polygon表达(凸包或凹包)。
作者:JohnKkk
日期:2022年1月8日10点35分
*/
class Contour
{
public:
	Contour(int regionID, vector<int> rawVerts, vector<int> verts);
	~Contour() {
		delete[] rawVerts;
		delete[] verts;
		rawVerts = nullptr;
		verts = nullptr;
	};


	// 当前轮廓关联的区域ID
	const int regionID;

	// 表示轮廓的原始顶点数据
	// 顶点数据以(x,y,z,regionID)顺时针打包,其中regionID表示顶点被连接到的区域ID
	int* rawVerts;

	// 记录原始顶点数目
	int rawVertCount;

	// 表示轮廓的简化顶点数据
	// 顶点数据以(x,y,z,regionID)顺时针打包,其中regionID表示顶点被连接到的区域ID
	int* verts;

	// 记录简化顶点数目
	int vertCount;
private:

};

#endif // !_CONTOUR_H_
