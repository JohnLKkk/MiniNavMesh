#pragma once
#ifndef _INTERMEDIATEDATA_H_
#define _INTERMEDIATEDATA_H_

/*
保存生成NavMesh相关的中间数据的类，当此数据与原几何数据（即输入场景数据）相结合时，表示构建最终的NavMesh的过程。
这个类同时也记录各个阶段生成时的时间开销，用于性能分析。
作者:JhonKkk
日期:2022年1月8日
*/
class IntermediateData
{
public:
	IntermediateData();
	~IntermediateData();

	// -1表示未定义状态
	static const long UNDEFINED = -1;

	// 体素化过程的时间开销
	long voxelizationTime;

	// 执行区域生成的时间开销
	long regionGenTime;

	// 执行轮廓生成的时间开销
	long contourGenTime;

	// 执行多边形生成的时间开销
	long polyGenTime;

	// 执行最终三角化的时间开销
	long finalMeshGenTime;
private:

};

#endif // !_INTERMEDIATEDATA_H_
