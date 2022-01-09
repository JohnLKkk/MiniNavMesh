#pragma once
#ifndef _NAVMESHGENERATOR_H_
#define _NAVMESHGENERATOR_H_

/*
用于生成任意几何体(只需要提供Positions与Indices数据即可）的潜在可遍历表面（导航网格数据），
当提供一个源几何场景数据进行构建时，会根据大量配置参数生成最终可靠的导航网格数据。
然后，基于生成的导航网格数据，可在其上应用任何基于图形的算法来应用不同需求。
作者:JohnKkk
日期:2022年1月8日18点17分
*/
class NavmeshGenerator
{
public:
	NavmeshGenerator();
	~NavmeshGenerator();

private:

};

#endif // !_NAVMESHGENERATOR_H_
