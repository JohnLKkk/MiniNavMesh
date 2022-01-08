#pragma once
#ifndef _CONTOURSET_H_
#include "BoundedField.h"
#include "Contour.h"

#include <vector>
using std::vector;

/*
����һ��������
����:JohnKkk
����:2022��1��8��11��02��
*/
class ContourSet : public BoundedField
{
public:
	ContourSet(const float *const gridBoundsMin, const float *const gridBoundsMax, float cellSize, float cellHeight, int initSize);
	~ContourSet() {
		delete contours;
	};

	/*
	���һ��������
	*/
	void add(Contour &contour) {
		contours->push_back(contour);
	}

	/*
	����ָ����������
	*/
	Contour* get(int index)const {
		if (index < 0 || index >= contours->size())
			return nullptr;
		return &(*contours)[index];
	}

	/*
	����������Ŀ��
	*/
	int size()const {
		return contours->size();
	};
private:
	//-----------------------------------------------------------------------
	// ���˵��:
	// ����Ҫɾ������֮ǰ���������Ҫ�Ľӿ�
	// �ο�Recast:rcContourSet in Recast.h
	//-----------------------------------------------------------------------
	vector<Contour> *const contours;
};


#endif // !_CONTOURSET_H_
