#pragma once
#ifndef _BOUNDEDFIELD_H_

/*
������������Grid��������Χ��(AABB)��
ͨ����cellSize������width��depth������width��x�᷽��depth��z�᷽����ˣ��䶨����xzƽ���ϡ�
ÿ��cell��λ��ͨ��gridIndex()��ȡ��
����:JohnKkk
����:2022��1��8��9��45��
*/
class BoundedField
{
public:
	BoundedField();
	BoundedField(float cellSize, float cellHeight);
	BoundedField(const float *const gridBoundsMin, const float *const gridBoundsMax, float cellSize, float cellHeight);
	virtual ~BoundedField() {};

	/*
	���ؿ�ȡ�
	*/
	int getWidth()const { return width; };

	/*
	������ȡ�
	*/
	int getDepth()const { return depth; };

	/*
	���ر߽�max��
	*/
	float* getBoundsMax() { return boundsMax; };

	/*
	���ر߽�min��
	*/
	float* getBoundsMin() { return boundsMin; };

	/*
	����CellSize��
	*/
	float getCellSize()const { return cellSize; };

	/*
	����CellHeight��
	*/
	float getCellHeight()const { return cellHeight; };

	/*
	�ж��Ƿ���Bounds�ڡ�
	*/
	bool inBounds(int widthIndex, int depthIndex)const {
		return widthIndex >= 0 && depthIndex >= 0 && widthIndex < width && depthIndex < depth;
	}

	/*
	�ж��Ƿ��ص���
	*/
	bool isOverlaps(const float *const boundsMin, const float *const boundsMax)const;

	static int getDirOffsetDepth(int dir);
	static int getDirOffsetWidth(int dir);
protected:
	/*
	����grid������
	*/
	int gridIndex(int widthIndex, int depthIndex)const;

	/*
	����BoundsΪmin(0,0,0)->max(1,1,1)��
	*/
	void resetBounds();

	/*
	����BoundsΪָ���߽硣
	*/
	void setBounds(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

	/*
	����BoundsΪָ���߽硣
	*/
	void setBounds(const float *const mins, const float *const maxs);

	/*
	����Bounds��Max�߽硣
	*/
	void setBoundsMax(const float *const values);

	/*
	����Bounds��Min�߽硣
	*/
	void setBoundsMin(const float *const values);

	/*
	����CellSize��CellHeightΪ0.1��
	*/
	void resetCellInfo();

	/*
	����cellSize,��СΪFLT_MIN
	*/
	void setCellSize(float value);

	/*
	����cellHeight,��СΪFLT_MIN
	*/
	void setCellHeight(float value);
private:
	//--------------------------------------------------------------------------------------------------
	// ��������˵��:
	// �ھ����������£��������ڰ�ȫ����ˣ��󲿷�getters�ӿڶ���ֱ�ӷ��������Ա�ֱ���޸�ԭ���ݡ�
	// ��һ����Ҳ�ɲο�Recast����ơ�
	//--------------------------------------------------------------------------------------------------
	int width;
	int depth;
	float boundsMin[3];
	float boundsMax[3];
	float cellSize;
	float cellHeight;

	/*
	���ݵ�ǰBounds��Cell��С����width��depth��
	*/
	void calculateWidthDepth();
};



#endif // !_BOUNDEDFIELD_H_
