#include "Builder.h"
#include "BlockHeightfield.h"
#include <cmath>
#include <algorithm>
#include <memory>

BlockHeightfield * BlockHeightfieldBuilder::build(int vertNums, const float * const vertices, int indNums, const int * const indices)
{
	if (!vertices || !indices)return nullptr;

	// 初始化 heightfield
	BlockHeightfield *const result = new BlockHeightfield(cellSize, cellHeight);

	const float inverseCellSize = 1 / result->getCellSize();
	const float inverseCellHeight = 1 / result->getCellHeight();

	// 计算场景包围盒
	float xmin = vertices[0];
	float ymin = vertices[1];
	float zmin = vertices[2];
	float xmax = vertices[0];
	float ymax = vertices[1];
	float zmax = vertices[2];
	for (int i = 3; i < vertNums; i += 3)
	{
		xmax = std::fmax(vertices[i], xmax);
		ymax = std::fmax(vertices[i + 1], ymax);
		zmax = std::fmax(vertices[i + 2], zmax);

		xmin = std::fmin(vertices[i], xmin);
		ymin = std::fmin(vertices[i + 1], ymin);
		zmin = std::fmin(vertices[i + 2], zmin);
	}
	result->setBounds(xmin, ymin, zmin, xmax, ymax, zmax);
	
	// 检测源网格中哪些多边形的斜率足够低，可以被认为是可遍历的。 （Agent 可以在斜坡上上下行走。）
	int* const polyFlags = genMeshWalkableFlags(vertNums, vertices, indNums, indices);

	// 体素化场景,并将其作为block span添加到BlockHeightfield中
	const int polyCount = indNums / 3;
	for (int iPoly = 0; iPoly < polyCount; iPoly++) {
		voxelizePolygon(iPoly, vertices, indices, polyFlags[iPoly], inverseCellSize, inverseCellHeight, *result);
	}

	// 初步剔除那些无法遍历的“可跨越span”
	genLowHeightSpans(*result);

	// 进一步剔除不可遍历的“可跨越span”
	if (clipLedges) {
		genLedgeSpans(*result);
	}

	return result;
}

int * BlockHeightfieldBuilder::genMeshWalkableFlags(int vertNums, const float * const vertices, int indNums, const int * const indices)
{
	int* const flags = new int[indNums / 3]();

	// 一些临时遍历
	float diffAB[3]{ 0 };
	float diffAC[3]{ 0 };
	float crossDiff[3]{ 0 };

	// 遍历所有多边形
	int polyCount = indNums / 3;
	for (int iPoly = 0; iPoly < polyCount; iPoly++) {
		//---------------------------------
		// (x,y,z),(x,y,z),(x,y,z)
		// 每个三角形3个顶点,每个顶点3个数
		//---------------------------------
		int pVertA = indices[iPoly * 3] * 3;
		int pVertB = indices[iPoly * 3 + 1] * 3;
		int pVertC = indices[iPoly * 3 + 2] * 3;
		
		// 确定Polygon(三角形)的法线Y值
		float normalY = getNormalY(
			cross(subtract(pVertB, pVertA, vertices, diffAB)
				, subtract(pVertC, pVertA, vertices, diffAC)
				, crossDiff));

		// 表面当前Polygon表示的表面可以行走
		if (normalY > minNormalY) {
			flags[iPoly] = WALKABLE;
		}
	}
	return flags;
}

float BlockHeightfieldBuilder::getNormalY(float * const v)
{
	// 这只是归一化vector3的逻辑
	// 去掉了x,z的归一化,因为只需要返回y
	const float epsilon = 0.0001f;

	float length = (float)std::sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
	if (length <= epsilon)
		length = 1;

	float y = v[1] / length;

	if (std::fabs(y) < epsilon)
		y = 0;

	return y;
}

float *const BlockHeightfieldBuilder::cross(const float * const u, const float * const v, float * const out)
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = -u[0] * v[2] + u[2] * v[0];
	out[2] = u[0] * v[1] - u[1] * v[0];
	return out;
}

float * const BlockHeightfieldBuilder::subtract(int pVertA, int pVertB, const float * const vertices, float * const out)
{
	out[0] = vertices[pVertA] - vertices[pVertB];
	out[1] = vertices[pVertA + 1] - vertices[pVertB + 1];
	out[2] = vertices[pVertA + 2] - vertices[pVertB + 2];
	return out;
}

void BlockHeightfieldBuilder::voxelizePolygon(int polyIndex, const float * const vertices, const int * const indices, int polyFlags, float inverseCellSize, float inverseCellHeight, BlockHeightfield & inoutField)
{
	const int pPoly = polyIndex * 3;

	float triVerts[] = {
		vertices[indices[pPoly] * 3]    // VertA
		, vertices[indices[pPoly] * 3 + 1]
		, vertices[indices[pPoly] * 3 + 2]
		, vertices[indices[pPoly + 1] * 3]    // VertB
		, vertices[indices[pPoly + 1] * 3 + 1]
		, vertices[indices[pPoly + 1] * 3 + 2]
		, vertices[indices[pPoly + 2] * 3]    // VertC
		, vertices[indices[pPoly + 2] * 3 + 1]
		, vertices[indices[pPoly + 2] * 3 + 2]
	};

	// 计算AABB包围盒
	float triBoundsMin[] = { triVerts[0], triVerts[1], triVerts[2] };
	float triBoundsMax[] = { triVerts[0], triVerts[1], triVerts[2] };

	for (int vertPointer = 3; vertPointer < 9; vertPointer += 3)
	{
		triBoundsMin[0] = std::fmin(triBoundsMin[0],
			triVerts[vertPointer]);
		triBoundsMin[1] = std::fmin(triBoundsMin[1],
			triVerts[vertPointer + 1]);
		triBoundsMin[2] = std::fmin(triBoundsMin[2],
			triVerts[vertPointer + 2]);
		triBoundsMax[0] = std::fmax(triBoundsMax[0],
			triVerts[vertPointer]);
		triBoundsMax[1] = std::fmax(triBoundsMax[1],
			triVerts[vertPointer + 1]);
		triBoundsMax[2] = std::fmax(triBoundsMax[2],
			triVerts[vertPointer + 2]);
	}


	// 如果三角形不与heightfield相交,则直接跳过
	if (!inoutField.isOverlaps(triBoundsMin, triBoundsMax))return;


	//-------------------------------------------------------------------------
	// 计算Triangle在heightfield的体素
	//
	// 备注:
	// heightfield是一个整型grid,原点在(boundsMin[0], boundsMin[2]),因为我们从
	// 深度和宽度定义xz平面,所以这里将Triangle的xz平面映射到heightfield。
	//-------------------------------------------------------------------------
	int triWidthMin = (int)((triBoundsMin[0] - inoutField.getBoundsMin()[0])
		* inverseCellSize);
	int triDepthMin = (int)((triBoundsMin[2] - inoutField.getBoundsMin()[2])
		* inverseCellSize);
	int triWidthMax = (int)((triBoundsMax[0] - inoutField.getBoundsMin()[0])
		* inverseCellSize);
	int triDepthMax = (int)((triBoundsMax[2] - inoutField.getBoundsMin()[2])
		* inverseCellSize);

	// 将其边界规范到heightfield中
	triWidthMin = clamp(triWidthMin, 0, inoutField.getWidth() - 1);
	triDepthMin = clamp(triDepthMin, 0, inoutField.getDepth() - 1);
	triWidthMax = clamp(triWidthMax, 0, inoutField.getWidth() - 1);
	triDepthMax = clamp(triDepthMax, 0, inoutField.getDepth() - 1);

	// "in" 将包含最终数据。
	// "out" 和 "inrow" 用于中间数据。
	// "in" 最初是源输入顶点数据
	// 数组的大小为 3 * 7。是为了保证存储6个顶点表示的正方形,并且多余的空间用于临时缓存
	float in[21]{ 0 };
	float out[21]{ 0 };
	float inrow[21]{ 0 };

	// 获取heightfield的高度分布范围
	const float fieldHeight = inoutField.getBoundsMax()[1] - inoutField.getBoundsMin()[1];


	for (int depthIndex = triDepthMin; depthIndex <= triDepthMax; ++depthIndex) {
		memcpy(triVerts, in, sizeof(triVerts));

		// 中间顶点数目
		int interVertNums = 3;
		float rowWorldZ = inoutField.getBoundsMin()[2] + (depthIndex * inoutField.getCellSize());

		interVertNums = clipPoly(in, interVertNums, out, 0, 1, -rowWorldZ);
		if (interVertNums < 3)
			continue;

		interVertNums = clipPoly(out, interVertNums, inrow, 0, -1, rowWorldZ + inoutField.getCellSize());
		if (interVertNums < 3)
			continue;

		for (int widthIndex = triWidthMin; widthIndex <= triWidthMax; widthIndex++ ) {
			int vertCount = interVertNums;
			const float colWorldX = inoutField.getBoundsMin()[0] + (widthIndex * inoutField.getCellSize());
			vertCount = clipPoly(inrow, vertCount, out, 1, 0, -colWorldX);
			if (vertCount < 3)
				continue;
			vertCount = clipPoly(out, vertCount, in, -1, 0, colWorldX + inoutField.getCellSize());
			if (vertCount < 3)
				continue;

			// 如果到达这里，则“in”包含一个多边形的定义，该多边形表示输入三角形与grid中重叠的cell部分。
			// 查找此cell位置的高度（y 轴）范围。
			float heightMin = in[1];
			float heightMax = in[1];
			for (int i = 1; i < vertCount; ++i)
			{
				heightMin = std::fmin(heightMin, in[i * 3 + 1]);
				heightMax = std::fmax(heightMax, in[i * 3 + 1]);
			}

			// 转换为在heightfield上的floor高度
			heightMin -= inoutField.getBoundsMin()[1];
			heightMax -= inoutField.getBoundsMin()[1];
			if (heightMax < 0.0f || heightMin > fieldHeight)
				// 跳过无效的span
				continue;

			// 确保(heightMin,heightMax)在heightfield范围内
			if (heightMin < 0.0f)
				heightMin = inoutField.getBoundsMin()[1];
			if (heightMax > fieldHeight)
				heightMax = inoutField.getBoundsMax()[1];

			// 计算span [heightMin,heightMax]
			int heightIndexMin = clamp((int)std::floor(heightMin * inverseCellHeight), 0, SHRT_MAX);
			int heightIndexMax = clamp((int)ceil(heightMax * inverseCellHeight), 0, SHRT_MAX);

			// 将span添加到heightfield
			inoutField.addData(widthIndex, depthIndex, heightIndexMin, heightIndexMax, polyFlags);
		}
	}
}

int BlockHeightfieldBuilder::clipPoly(const float * const in, int inputVertCount, float * const out, float pnx, float pnz, float pd)
{
	// pnx和pnz是法线分量
	float * d = new float[inputVertCount]();
	for (int vertIndex = 0; vertIndex < inputVertCount; ++vertIndex) {
		d[vertIndex] = (pnx * in[vertIndex * 3]) + (pnz * in[vertIndex * 3 + 2]) + pd;
	}

	int m = 0;
	for (int current = 0, previous = inputVertCount - 1; current < inputVertCount; previous = current, ++current){
		bool ina = d[previous] >= 0;
		bool inb = d[current] >= 0;
		if (ina != inb)
		{
			float s = d[previous] / (d[previous] - d[current]);
			out[m * 3 + 0] =
				in[previous * 3 + 0] + (in[current * 3 + 0] - in[previous * 3 + 0])*s;
			out[m * 3 + 1] =
				in[previous * 3 + 1] + (in[current * 3 + 1] - in[previous * 3 + 1])*s;
			out[m * 3 + 2] =
				in[previous * 3 + 2] + (in[current * 3 + 2] - in[previous * 3 + 2])*s;
			m++;
		}
		if (inb)
		{
			out[m * 3 + 0] = in[current * 3 + 0];
			out[m * 3 + 1] = in[current * 3 + 1];
			out[m * 3 + 2] = in[current * 3 + 2];
			m++;
		}
	}

	return m;
}

void BlockHeightfieldBuilder::genLowHeightSpans(BlockHeightfield & field)
{
	BlockHeightfield::Iterator *iter = field.data();
	while (iter->allow()) {
		HeightSpan &span = iter->getNext();

		if ((span.getFlags() & WALKABLE) == 0)
			// 该span是不可行走的,直接跳过
			continue;

		int spanFloor = span.max();
		int spanCeiling = (span.getNext()) ? span.getNext()->min() : INT_MAX;

		// 说明该“可跨越span”是不可遍历的空间,跳过
		if (spanCeiling - spanFloor <= minTraversableHeight) {
			// 不能站在这个floor上，因为ceiling太低，移除WALKABLE标记
			span.setFlags(span.getFlags() & ~WALKABLE);
		}
	}
}

void BlockHeightfieldBuilder::genLedgeSpans(BlockHeightfield & field)
{
	BlockHeightfield::Iterator *iter = field.data();
	while (iter->allow()) {
		HeightSpan &span = iter->getNext();

		if ((span.getFlags() & WALKABLE) == 0) {
			// 跳过本身就无法遍历的span
			continue;
		}

		const int widthIndex = iter->getWidthIndex();
		const int depthIndex = iter->getWidthIndex();

		// 获取当前“可跨越的span”的“可跨越范围”
		// “可跨越范围”指可通过空间范围，其定义为当前span顶部(地板)到下一个span底部(天花板)之间
		// 的空间高度
		const int currFloor = span.max();
		const int currCeilling = (span.getNext()) ? span.getNext()->min() : INT_MAX;

		// 判断当前span到邻居的最小距离,这里的最小距离是指:span的floor高度到邻居span的floor高度差。
		// 正数表示上爬,负数表示下落
		// 如果下落的幅度超过了参数maxTraversableStep，则被认为是不可遍历的span
		// 这里只处理下落的幅度的剔除
		int minDisToNeighbor = INT_MAX;

		// 只允许下落到-maxTraversableStep,有一点需要注意的是,如果从"零span"(也就是场景最低表面)下降,
		// 因为没有下一个span,因此直接下降为-maxTraversableStep，因此并不会过滤掉"零span"，也就是场景最低表面
		// 这是允许的，并且也应该是如此的，否则，将不存在场景地表的可遍历span。

		// 遍历当前grid中所有相邻的cell
		// 这里只遍历周围4个邻居
		for (int dir = 0; dir < 4; dir++) {

			const int nWidthIndex = widthIndex
				+ BoundedField::getDirOffsetWidth(dir);
			const int nDepthIndex = depthIndex
				+ BoundedField::getDirOffsetDepth(dir);

			HeightSpan * nSpan = field.getData(nWidthIndex, nDepthIndex);
			if (!nSpan) {
				// 不存在邻居,此时直接设置为最大下落幅度
				minDisToNeighbor = std::min(minDisToNeighbor, -maxTraversableStep - currFloor);
				continue;
			}

			// 允许从当前floor下落到邻居的最大下落幅度为nFloor
			int nFloor = -maxTraversableStep;
			int nCeiling = nSpan->min();

			// 首先过滤一种情况:从A span到B span(邻居)时,假设B span比A span高,那么A到B的走向需要考虑
			// 会不会撞到B span(也就是天花板)
			// 也就是说这里过滤的是A span到邻居B span时(B span高于A span),会不会撞到B span
			if (std::min(nCeiling, currCeilling) - currFloor > minTraversableHeight) {
				// 说明可以从A span正常走向B span
				minDisToNeighbor = std::min(minDisToNeighbor, (nFloor - currFloor));
			}

			// 现在检测这个邻居的余下span的情况,计算最小允许的下落幅度
			while (nSpan) {
				nFloor = nSpan->max();
				nCeiling = (nSpan->getNext()) ? nSpan->getNext()->min() : INT_MAX;

				if (std::min(currCeilling, nCeiling) - std::max(currFloor, nFloor) > minTraversableHeight) {
					// 允许在从span遍历到当前这个邻居,继续更新最小高度差
					minDisToNeighbor = std::min(minDisToNeighbor, (nFloor - currFloor));
				}

				nSpan = nSpan->getNext();
			}
		}

		// 记住:负距离表示下降。
		if (minDisToNeighbor < -maxTraversableStep) {
			// 仅允许下降不超过maxTraversableStep参数指定的值
			// 所以这里表示当前span是不可遍历的表面
			span.setFlags(span.getFlags() & ~WALKABLE);
		}
	}
}
