#pragma once
#ifndef _ALGORITHM_H_

#include <vector>
using std::vector;

/*
�����㷨�ӿ�,��ʵ����ֻʵ����һ��,����Ϊ�˺������ܵ���չ��Ľ�,������Ȼ�ṩ�ӿڷ�ʽ���䡣
����:JohnKkk
����:2022��1��8��13��03��
*/
class IContourAlgorithm
{
public:
	virtual ~IContourAlgorithm() {};

	/*
	�������㷨Ӧ��������������ֱ�Ӱ�sourceVerts�洢��resultVerts��������ж���������(x,y,z,ragionID)˳ʱ������
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
