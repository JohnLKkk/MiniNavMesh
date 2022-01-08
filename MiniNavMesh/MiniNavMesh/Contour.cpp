#include "Contour.h"

Contour::Contour(int regionID, vector<int> rawVerts, vector<int> verts):regionID(regionID)
{
	// 这里没有做非空判断,假设所有参数都是有效参数
	this->rawVerts = new int[rawVerts.size()];
	for (int i = 0, size = rawVerts.size(); i < size; i++) {
		this->rawVerts[i] = rawVerts[i];
	}
	this->rawVertCount = rawVerts.size() / 4;

	this->verts = new int[verts.size()];
	for (int i = 0, size = verts.size(); i < size; i++) {
		this->verts[i] = verts[i];
	}
	this->vertCount = verts.size() / 4;
}
