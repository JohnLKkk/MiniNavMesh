#include "Builder.h"
#include "BlockHeightfield.h"
#include <cmath>
#include <algorithm>
#include <memory>

BlockHeightfield * BlockHeightfieldBuilder::build(int vertNums, const float * const vertices, int indNums, const int * const indices)
{
	if (!vertices || !indices)return nullptr;

	// ��ʼ�� heightfield
	BlockHeightfield *const result = new BlockHeightfield(cellSize, cellHeight);

	const float inverseCellSize = 1 / result->getCellSize();
	const float inverseCellHeight = 1 / result->getCellHeight();

	// ���㳡����Χ��
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
	
	// ���Դ��������Щ����ε�б���㹻�ͣ����Ա���Ϊ�ǿɱ����ġ� ��Agent ������б�����������ߡ���
	int* const polyFlags = genMeshWalkableFlags(vertNums, vertices, indNums, indices);

	// ���ػ�����,��������Ϊblock span��ӵ�BlockHeightfield��
	const int polyCount = indNums / 3;
	for (int iPoly = 0; iPoly < polyCount; iPoly++) {
		voxelizePolygon(iPoly, vertices, indices, polyFlags[iPoly], inverseCellSize, inverseCellHeight, *result);
	}

	// �����޳���Щ�޷������ġ��ɿ�Խspan��
	genLowHeightSpans(*result);

	// ��һ���޳����ɱ����ġ��ɿ�Խspan��
	if (clipLedges) {
		genLedgeSpans(*result);
	}

	return result;
}

int * BlockHeightfieldBuilder::genMeshWalkableFlags(int vertNums, const float * const vertices, int indNums, const int * const indices)
{
	int* const flags = new int[indNums / 3]();

	// һЩ��ʱ����
	float diffAB[3]{ 0 };
	float diffAC[3]{ 0 };
	float crossDiff[3]{ 0 };

	// �������ж����
	int polyCount = indNums / 3;
	for (int iPoly = 0; iPoly < polyCount; iPoly++) {
		//---------------------------------
		// (x,y,z),(x,y,z),(x,y,z)
		// ÿ��������3������,ÿ������3����
		//---------------------------------
		int pVertA = indices[iPoly * 3] * 3;
		int pVertB = indices[iPoly * 3 + 1] * 3;
		int pVertC = indices[iPoly * 3 + 2] * 3;
		
		// ȷ��Polygon(������)�ķ���Yֵ
		float normalY = getNormalY(
			cross(subtract(pVertB, pVertA, vertices, diffAB)
				, subtract(pVertC, pVertA, vertices, diffAC)
				, crossDiff));

		// ���浱ǰPolygon��ʾ�ı����������
		if (normalY > minNormalY) {
			flags[iPoly] = WALKABLE;
		}
	}
	return flags;
}

float BlockHeightfieldBuilder::getNormalY(float * const v)
{
	// ��ֻ�ǹ�һ��vector3���߼�
	// ȥ����x,z�Ĺ�һ��,��Ϊֻ��Ҫ����y
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

	// ����AABB��Χ��
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


	// ��������β���heightfield�ཻ,��ֱ������
	if (!inoutField.isOverlaps(triBoundsMin, triBoundsMax))return;


	//-------------------------------------------------------------------------
	// ����Triangle��heightfield������
	//
	// ��ע:
	// heightfield��һ������grid,ԭ����(boundsMin[0], boundsMin[2]),��Ϊ���Ǵ�
	// ��ȺͿ�ȶ���xzƽ��,�������ｫTriangle��xzƽ��ӳ�䵽heightfield��
	//-------------------------------------------------------------------------
	int triWidthMin = (int)((triBoundsMin[0] - inoutField.getBoundsMin()[0])
		* inverseCellSize);
	int triDepthMin = (int)((triBoundsMin[2] - inoutField.getBoundsMin()[2])
		* inverseCellSize);
	int triWidthMax = (int)((triBoundsMax[0] - inoutField.getBoundsMin()[0])
		* inverseCellSize);
	int triDepthMax = (int)((triBoundsMax[2] - inoutField.getBoundsMin()[2])
		* inverseCellSize);

	// ����߽�淶��heightfield��
	triWidthMin = clamp(triWidthMin, 0, inoutField.getWidth() - 1);
	triDepthMin = clamp(triDepthMin, 0, inoutField.getDepth() - 1);
	triWidthMax = clamp(triWidthMax, 0, inoutField.getWidth() - 1);
	triDepthMax = clamp(triDepthMax, 0, inoutField.getDepth() - 1);

	// "in" �������������ݡ�
	// "out" �� "inrow" �����м����ݡ�
	// "in" �����Դ���붥������
	// ����Ĵ�СΪ 3 * 7����Ϊ�˱�֤�洢6�������ʾ��������,���Ҷ���Ŀռ�������ʱ����
	float in[21]{ 0 };
	float out[21]{ 0 };
	float inrow[21]{ 0 };

	// ��ȡheightfield�ĸ߶ȷֲ���Χ
	const float fieldHeight = inoutField.getBoundsMax()[1] - inoutField.getBoundsMin()[1];


	for (int depthIndex = triDepthMin; depthIndex <= triDepthMax; ++depthIndex) {
		memcpy(triVerts, in, sizeof(triVerts));

		// �м䶥����Ŀ
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

			// ������������in������һ������εĶ��壬�ö���α�ʾ������������grid���ص���cell���֡�
			// ���Ҵ�cellλ�õĸ߶ȣ�y �ᣩ��Χ��
			float heightMin = in[1];
			float heightMax = in[1];
			for (int i = 1; i < vertCount; ++i)
			{
				heightMin = std::fmin(heightMin, in[i * 3 + 1]);
				heightMax = std::fmax(heightMax, in[i * 3 + 1]);
			}

			// ת��Ϊ��heightfield�ϵ�floor�߶�
			heightMin -= inoutField.getBoundsMin()[1];
			heightMax -= inoutField.getBoundsMin()[1];
			if (heightMax < 0.0f || heightMin > fieldHeight)
				// ������Ч��span
				continue;

			// ȷ��(heightMin,heightMax)��heightfield��Χ��
			if (heightMin < 0.0f)
				heightMin = inoutField.getBoundsMin()[1];
			if (heightMax > fieldHeight)
				heightMax = inoutField.getBoundsMax()[1];

			// ����span [heightMin,heightMax]
			int heightIndexMin = clamp((int)std::floor(heightMin * inverseCellHeight), 0, SHRT_MAX);
			int heightIndexMax = clamp((int)ceil(heightMax * inverseCellHeight), 0, SHRT_MAX);

			// ��span��ӵ�heightfield
			inoutField.addData(widthIndex, depthIndex, heightIndexMin, heightIndexMax, polyFlags);
		}
	}
}

int BlockHeightfieldBuilder::clipPoly(const float * const in, int inputVertCount, float * const out, float pnx, float pnz, float pd)
{
	// pnx��pnz�Ƿ��߷���
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
			// ��span�ǲ������ߵ�,ֱ������
			continue;

		int spanFloor = span.max();
		int spanCeiling = (span.getNext()) ? span.getNext()->min() : INT_MAX;

		// ˵���á��ɿ�Խspan���ǲ��ɱ����Ŀռ�,����
		if (spanCeiling - spanFloor <= minTraversableHeight) {
			// ����վ�����floor�ϣ���Ϊceiling̫�ͣ��Ƴ�WALKABLE���
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
			// ����������޷�������span
			continue;
		}

		const int widthIndex = iter->getWidthIndex();
		const int depthIndex = iter->getWidthIndex();

		// ��ȡ��ǰ���ɿ�Խ��span���ġ��ɿ�Խ��Χ��
		// ���ɿ�Խ��Χ��ָ��ͨ���ռ䷶Χ���䶨��Ϊ��ǰspan����(�ذ�)����һ��span�ײ�(�컨��)֮��
		// �Ŀռ�߶�
		const int currFloor = span.max();
		const int currCeilling = (span.getNext()) ? span.getNext()->min() : INT_MAX;

		// �жϵ�ǰspan���ھӵ���С����,�������С������ָ:span��floor�߶ȵ��ھ�span��floor�߶Ȳ
		// ������ʾ����,������ʾ����
		// �������ķ��ȳ����˲���maxTraversableStep������Ϊ�ǲ��ɱ�����span
		// ����ֻ��������ķ��ȵ��޳�
		int minDisToNeighbor = INT_MAX;

		// ֻ�������䵽-maxTraversableStep,��һ����Ҫע�����,�����"��span"(Ҳ���ǳ�����ͱ���)�½�,
		// ��Ϊû����һ��span,���ֱ���½�Ϊ-maxTraversableStep����˲�������˵�"��span"��Ҳ���ǳ�����ͱ���
		// ��������ģ�����ҲӦ������˵ģ����򣬽������ڳ����ر�Ŀɱ���span��

		// ������ǰgrid���������ڵ�cell
		// ����ֻ������Χ4���ھ�
		for (int dir = 0; dir < 4; dir++) {

			const int nWidthIndex = widthIndex
				+ BoundedField::getDirOffsetWidth(dir);
			const int nDepthIndex = depthIndex
				+ BoundedField::getDirOffsetDepth(dir);

			HeightSpan * nSpan = field.getData(nWidthIndex, nDepthIndex);
			if (!nSpan) {
				// �������ھ�,��ʱֱ������Ϊ����������
				minDisToNeighbor = std::min(minDisToNeighbor, -maxTraversableStep - currFloor);
				continue;
			}

			// ����ӵ�ǰfloor���䵽�ھӵ�����������ΪnFloor
			int nFloor = -maxTraversableStep;
			int nCeiling = nSpan->min();

			// ���ȹ���һ�����:��A span��B span(�ھ�)ʱ,����B span��A span��,��ôA��B��������Ҫ����
			// �᲻��ײ��B span(Ҳ�����컨��)
			// Ҳ����˵������˵���A span���ھ�B spanʱ(B span����A span),�᲻��ײ��B span
			if (std::min(nCeiling, currCeilling) - currFloor > minTraversableHeight) {
				// ˵�����Դ�A span��������B span
				minDisToNeighbor = std::min(minDisToNeighbor, (nFloor - currFloor));
			}

			// ���ڼ������ھӵ�����span�����,������С������������
			while (nSpan) {
				nFloor = nSpan->max();
				nCeiling = (nSpan->getNext()) ? nSpan->getNext()->min() : INT_MAX;

				if (std::min(currCeilling, nCeiling) - std::max(currFloor, nFloor) > minTraversableHeight) {
					// �����ڴ�span��������ǰ����ھ�,����������С�߶Ȳ�
					minDisToNeighbor = std::min(minDisToNeighbor, (nFloor - currFloor));
				}

				nSpan = nSpan->getNext();
			}
		}

		// ��ס:�������ʾ�½���
		if (minDisToNeighbor < -maxTraversableStep) {
			// �������½�������maxTraversableStep����ָ����ֵ
			// ���������ʾ��ǰspan�ǲ��ɱ����ı���
			span.setFlags(span.getFlags() & ~WALKABLE);
		}
	}
}
