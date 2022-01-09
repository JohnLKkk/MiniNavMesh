#pragma once
#ifndef _OPENHEIGHTFIELD_H_
#define _OPENHEIGHTFIELD_H_

#include "BoundedField.h"

/*
在NavMesh系统中，由于是3D场景，因此同一cell存在不同楼层的可跨越平面，平面的每个cell通过OpenHeightField描述。
其中，当前cell的OpenHeightField最低值是当前cell的HeightField(实体span)的顶部(max)，而OpenHeightField的最高值是
当前span下一个span的最低点,如果当前cell不存在下一个span,则为无限值（比如INT_MAX)。
另外，当前cell最低span以下不存在OpenHeightField，没有任何HeightField的cell也不会存在OpenHeightField。
作者:JohnKkk
日期:2022年1月8日
*/
class OpenHeightField : public BoundedField
{
public:
	OpenHeightField();
	~OpenHeightField();

	class OpenHeightFieldIterator
	{
	public:
		OpenHeightFieldIterator();
		~OpenHeightFieldIterator();

	private:
		int nextWidth;
		int nextDepth;
	};


private:

};

#endif // !_OPENHEIGHTFIELD_H_
