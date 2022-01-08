#pragma once
#ifndef _CONTOURSET_H_
#include "BoundedField.h"
#include "Contour.h"

#include <vector>
using std::vector;

/*
描述一组轮廓。
作者:JohnKkk
日期:2022年1月8日11点02分
*/
class ContourSet : public BoundedField
{
public:
	ContourSet(const float *const gridBoundsMin, const float *const gridBoundsMax, float cellSize, float cellHeight, int initSize);
	~ContourSet() {
		delete contours;
	};

	/*
	添加一个轮廓。
	*/
	void add(Contour &contour) {
		contours->push_back(contour);
	}

	/*
	返回指定的轮廓。
	*/
	Contour* get(int index)const {
		if (index < 0 || index >= contours->size())
			return nullptr;
		return &(*contours)[index];
	}

	/*
	返回轮廓数目。
	*/
	int size()const {
		return contours->size();
	};
private:
	//-----------------------------------------------------------------------
	// 设计说明:
	// 在需要删除功能之前不添加所需要的接口
	// 参考Recast:rcContourSet in Recast.h
	//-----------------------------------------------------------------------
	vector<Contour> *const contours;
};


#endif // !_CONTOURSET_H_
