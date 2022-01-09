#pragma once
#ifndef _OPENHEIGHTFIELD_H_
#define _OPENHEIGHTFIELD_H_

#include "BoundedField.h"

/*
��NavMeshϵͳ�У�������3D���������ͬһcell���ڲ�ͬ¥��Ŀɿ�Խƽ�棬ƽ���ÿ��cellͨ��OpenHeightField������
���У���ǰcell��OpenHeightField���ֵ�ǵ�ǰcell��HeightField(ʵ��span)�Ķ���(max)����OpenHeightField�����ֵ��
��ǰspan��һ��span����͵�,�����ǰcell��������һ��span,��Ϊ����ֵ������INT_MAX)��
���⣬��ǰcell���span���²�����OpenHeightField��û���κ�HeightField��cellҲ�������OpenHeightField��
����:JohnKkk
����:2022��1��8��
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
