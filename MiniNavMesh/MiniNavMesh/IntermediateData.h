#pragma once
#ifndef _INTERMEDIATEDATA_H_
#define _INTERMEDIATEDATA_H_

/*
��������NavMesh��ص��м����ݵ��࣬����������ԭ�������ݣ������볡�����ݣ�����ʱ����ʾ�������յ�NavMesh�Ĺ��̡�
�����ͬʱҲ��¼�����׶�����ʱ��ʱ�俪�����������ܷ�����
����:JhonKkk
����:2022��1��8��
*/
class IntermediateData
{
public:
	IntermediateData();
	~IntermediateData();

	// -1��ʾδ����״̬
	static const long UNDEFINED = -1;

	// ���ػ����̵�ʱ�俪��
	long voxelizationTime;

	// ִ���������ɵ�ʱ�俪��
	long regionGenTime;

	// ִ���������ɵ�ʱ�俪��
	long contourGenTime;

	// ִ�ж�������ɵ�ʱ�俪��
	long polyGenTime;

	// ִ���������ǻ���ʱ�俪��
	long finalMeshGenTime;
private:

};

#endif // !_INTERMEDIATEDATA_H_
