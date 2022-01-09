#pragma once
#ifndef _REGION_H_
#define _REGION_H_

#include <vector>
#include <iostream>

using std::vector;

/*
���������ʹ�������(Region)�Ľṹ��<br/>
����:JohnKkk
����:2022��1��8��09��10��
*/
class Region
{
public:

	// ����ID
	int id;

	// �������Span��Ŀ
	int spanCount;

	// �Ƿ���Ҫ��������ID
	bool remap;

	// ����
	// ��ʾ��ǰ����������������������ӱ���������������һ�����ӣ���ʹ���ӵ�������
	// һ��������ڶ�����ӵ���������ĵ㣬����:1,2,3,1����ʾ��ǰ�������и������ӵ�����1��
	// ֮���Դ洢������ӵ㣬����Ϊ�ڽ�������ϲ�ʱ����Ҫ�����Ƿ�������Ч����Ρ�
	vector<int> *const connections;

	// �뵱ǰ�����ص��ķǿ�����
	// 1.�����ǰ�����span�ڸ߶ȷ��������һ�������span
	// ����Ϊ��Щ�����ص�
	// 2.�����ǰ������������ص���span����Ҳ�������(��ʾ�����ص�)
	vector<int> *const overlappingRegions;


	Region(int id = 0) :id(id), spanCount(0), remap(false), connections(new vector<int>()), overlappingRegions(new vector<int>()) {};

	~Region() {
		delete connections;
		delete overlappingRegions;
	};

	/*
	���õ�ǰ����
	*/
	void reset(int id) {
		this->id = id;
		spanCount = 0;
		connections->clear();
		overlappingRegions->clear();
	}

	/*
	���ڲ��ԡ�
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
