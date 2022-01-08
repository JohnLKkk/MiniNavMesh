#pragma once
#ifndef _BUILDER_H_

#include "BlockHeightfield.h"
#include <cmath>
#define M_PI 3.14159265358979323846
/*
这个类用于将体素化后的场景信息，其实心voxel(受阻部分)记录为一组span，并根据“可跨越的span”生成最终有效的OpenHeightField。
作者:JohnKkk
日期:2022年1月8日22点13分
*/
class BlockHeightfieldBuilder
{
public:
	BlockHeightfieldBuilder(float cellSize = 1, float cellHeight = 1, int minTraversableHeight = 1, int maxTraversableStep = 1, float maxTraversableSlope = 0.4f, bool clipLedges = false):cellSize(cellSize), cellHeight(cellHeight), minTraversableHeight(minTraversableHeight), maxTraversableStep(maxTraversableStep) {
		maxTraversableSlope = std::fmin(85, std::fmax(0, maxTraversableSlope));


		/*
		导出最小 y 法线。
		基础参考：http://mathworld.wolfram.com/DihedralAngle.html
		通过确保在计算之前对 n1 和 n2 进行归一化，
		参考方程中的分母计算为 1 和
		可以丢弃。所以参考方程简化为...

		cos theta = n1 dot n2

		使用：
		n1 = (0, 1, 0)（表示 (x,z) 平面上的平面。）
		n2 = (x, y, z) 归一化。 （任意平面上的表面。）
		
		简化并求解 y：
		cos theta = 0x + 1y + 0z
		y = cos theta
		
		这里的θ。是maxTraversableSlope转换为弧度后的值。
		所以我们知道行走坡度角的 y 法线是多少。如果多
		边形的 y 法线小于这里计算的 y 法线，那么就可以知道已经超出了行走角度。
		*/
		minNormalY = float(std::cosf(std::fabsf(maxTraversableSlope) / 180 * M_PI));
	};
	~BlockHeightfieldBuilder() {};

	/*
	从提供的源几何数据生成BlockHeightfield,BlockHeightfield将表示源几何数据“受阻”的
	体素化场景。
	*/
	BlockHeightfield* build(const float *const vertices, const int *const indices);
private:

	// 一组配置参数
	//------------------------------------------------------------------
	// 设计说明:
	// 通常，在使用NavMesh的系统中，你会看到可以进行设置一组参数来调整
	// 并生成最终满意的NavMesh数据，这些参数不一定完全与其他NavMesh系统
	// 一致，但是它们很大程度上是命名不同或者使用了更激进的生成算法并
	// 提供了额外的参数。
	// 总的来说，有很大一部分参数应该是共有的，比如cellSize，cellHeight
	// 以及maxTraversableSlope等参数。
	//------------------------------------------------------------------


	const bool clipLedges;
	const int minTraversableHeight;
	const int maxTraversableStep;

	// 表示多边形被视为可遍历表面的最小法线Y值
	// 这个参数由maxTraversableSlope(最大斜坡率)计算得到
	float minNormalY;

	// 用于初始化Heightfield,但是,Heightfield会在生成过程中调整并限制cell的有效范围,因此,该参数
	// 仅仅用于初始化Heightfield阶段
	const float cellSize;

	// 用于初始化Heightfield,但是,Hiehgtfield会在生成过程中调整并限制cell的有效范围,因此,该参数
	// 仅仅用于初始化Heightfield阶段
	const float cellHeight;

};

#endif // !_BUILDER_H_
