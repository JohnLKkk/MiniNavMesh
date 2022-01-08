#pragma once
#ifndef _BOUNDEDFIELD_H_

/*
用于描述基于Grid的轴对齐包围盒(AABB)。
通常由cellSize定义其width和depth，其中width在x轴方向，depth在z轴方向，因此，其定义在xz平面上。
每个cell的位置通过gridIndex()获取。
作者:JohnKkk
日期:2022年1月8日9点45分
*/
class BoundedField
{
public:
	BoundedField();
	BoundedField(float cellSize, float cellHeight);
	BoundedField(const float *const gridBoundsMin, const float *const gridBoundsMax, float cellSize, float cellHeight);
	virtual ~BoundedField() {};

	/*
	返回宽度。
	*/
	int getWidth()const { return width; };

	/*
	返回深度。
	*/
	int getDepth()const { return depth; };

	/*
	返回边界max。
	*/
	float* getBoundsMax() { return boundsMax; };

	/*
	返回边界min。
	*/
	float* getBoundsMin() { return boundsMin; };

	/*
	返回CellSize。
	*/
	float getCellSize()const { return cellSize; };

	/*
	返回CellHeight。
	*/
	float getCellHeight()const { return cellHeight; };

	/*
	判断是否在Bounds内。
	*/
	bool inBounds(int widthIndex, int depthIndex)const {
		return widthIndex >= 0 && depthIndex >= 0 && widthIndex < width && depthIndex < depth;
	}

	/*
	判断是否重叠。
	*/
	bool isOverlaps(const float *const boundsMin, const float *const boundsMax)const;

	static int getDirOffsetDepth(int dir);
	static int getDirOffsetWidth(int dir);
protected:
	/*
	计算grid索引。
	*/
	int gridIndex(int widthIndex, int depthIndex)const;

	/*
	重置Bounds为min(0,0,0)->max(1,1,1)。
	*/
	void resetBounds();

	/*
	设置Bounds为指定边界。
	*/
	void setBounds(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

	/*
	设置Bounds为指定边界。
	*/
	void setBounds(const float *const mins, const float *const maxs);

	/*
	设置Bounds的Max边界。
	*/
	void setBoundsMax(const float *const values);

	/*
	设置Bounds的Min边界。
	*/
	void setBoundsMin(const float *const values);

	/*
	重置CellSize和CellHeight为0.1。
	*/
	void resetCellInfo();

	/*
	设置cellSize,最小为FLT_MIN
	*/
	void setCellSize(float value);

	/*
	设置cellHeight,最小为FLT_MIN
	*/
	void setCellHeight(float value);
private:
	//--------------------------------------------------------------------------------------------------
	// 这里的设计说明:
	// 在绝大多数情况下，性能优于安全，因此，大部分getters接口都是直接返回引用以便直接修改原数据。
	// 这一部分也可参考Recast的设计。
	//--------------------------------------------------------------------------------------------------
	int width;
	int depth;
	float boundsMin[3];
	float boundsMax[3];
	float cellSize;
	float cellHeight;

	/*
	根据当前Bounds和Cell大小设置width和depth。
	*/
	void calculateWidthDepth();
};



#endif // !_BOUNDEDFIELD_H_
