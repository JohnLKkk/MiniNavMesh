#pragma once
#ifndef _CONTOUR_H_
#include <vector>
using std::vector;

/*
ͬʱ����Detailed��Simplified������ʽ��һ��������һ��Polygon���(͹���򰼰�)��
����:JohnKkk
����:2022��1��8��10��35��
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


	// ��ǰ��������������ID
	const int regionID;

	// ��ʾ������ԭʼ��������
	// ����������(x,y,z,regionID)˳ʱ����,����regionID��ʾ���㱻���ӵ�������ID
	int* rawVerts;

	// ��¼ԭʼ������Ŀ
	int rawVertCount;

	// ��ʾ�����ļ򻯶�������
	// ����������(x,y,z,regionID)˳ʱ����,����regionID��ʾ���㱻���ӵ�������ID
	int* verts;

	// ��¼�򻯶�����Ŀ
	int vertCount;
private:

};

#endif // !_CONTOUR_H_
