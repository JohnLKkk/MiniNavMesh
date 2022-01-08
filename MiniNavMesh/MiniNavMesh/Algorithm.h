#pragma once
#ifndef _ALGORITHM_H_

#include <vector>
using std::vector;

/*
轮廓算法接口,其实这里只实现了一种,但是为了后续可能的拓展与改进,所以仍然提供接口方式适配。
作者:JohnKkk
日期:2022年1月8日13点03分
*/
class IContourAlgorithm
{
public:
	virtual ~IContourAlgorithm() {};

	/*
	将轮廓算法应用于轮廓，可以直接把sourceVerts存储到resultVerts输出，其中顶点数据以(x,y,z,ragionID)顺时针打包。
	*/
	virtual void apply(const vector<int>& const sourceVerts, vector<int>& resultVerts) = 0;
private:

};


class IOpenHeightFieldAlgorithm
{
public:
	virtual ~IOpenHeightFieldAlgorithm() {};
	virtual void apply() = 0;
private:

};

#endif // !_ALGORITHM_H_
