#pragma once
#ifndef _BLOCKHEIGHTFIELD_H_
#define _BLOCKHEIGHTFIELD_H_


#include "BoundedField.h"
#include "HeightSpan.h"
#include <unordered_map>

using std::unordered_map;

/*
BlockHeightfield用于表示体素化后由一系列cell组成的grid,其中每个cell包含一组heightspan(heightspan表示包含源场景mesh的受阻span)。
作者:JohnKkk
日期:2022年1月9日10点20分
更新:2022年1月9日13点13分
*/
class BlockHeightfield : public BoundedField
{
public:
	BlockHeightfield(float cellSize = 1, float cellHeight = 1) :BoundedField(cellSize, cellHeight) {};

	~BlockHeightfield() {
		/*for (auto data = spans.begin(); data != spans.end(); data++) {
			if (data->second) {
				delete data->second;
			}
		}*/
	};

	/*
	将span添加到heightfield中，新的span要么合并到已有的span上，要么创建一个新的span。
	仅作以下测试:
	1.widthIndex和depthIndex的边界测试。
	2.heightIndex的下限。
	3.height min <= max。
	4.不处理最大高度是否在有效范围内。
	标记的更新如下:
	1.如果新数据的maxiMum与一个现有的span的maxiMum一致，则合并flag，
	2.如果新span的maxiMum表示新的最大值，则设置为新span的flag
	3.否则，忽略新span的flag
	简而言之，只有span的顶部的flag是最重要的(因为最终可遍历的表面就是span的顶部(也就是floor))
	@param widthIndex 新span的cell宽度索引
	@param depthIndex 新span的cell深度索引
	@param heightIndexMin 新span的高度边界最低值
	@param heightIndexMax 新span的高度边界最高值
	@param flags 新的flags
	@return 成功添加返回true，如果返回false，唯一的可能是某些参数导致的无效数据
	*/
	bool addData(int widthIndex, int depthIndex, int heightIndexMin, int heightIndexMax, int flags);


	
private:

	// 包含Heightfield的spans
	// key:通过gridIndex()获取grid的Index
	// value:当前gridIndex的最低位置的span,如果当前cell没有span,则为nullptr
	unordered_map<int, HeightSpan*> spans;


	class Iterator
	{
	public:
		Iterator(BlockHeightfield & blockHeightfield) :blockHeightfield(&blockHeightfield), lastDepth(-1), lastWidth(-1), next(nullptr), nextWidth(0), nextDepth(0) {
			toNext();
		};
		~Iterator() {};

		/*
		返回最后一个访问的span的depthIndex。
		*/
		int depthIndex()const { return lastDepth; }

		bool allow()const { return next; }

		HeightSpan& getNext() {
			// 假设当前next!=nullptr

			HeightSpan * next = this->next;
			lastWidth = nextWidth;
			lastDepth = nextDepth;
			toNext();
			return *next;
		}

		void reset() {
			nextWidth = nextDepth = lastWidth = lastDepth = 0;
			next = nullptr;
			toNext();
		}

		/*
		返回最后访问的span的widthIndex。
		*/
		int getWidthIndex() { return lastWidth; }

	private:
		BlockHeightfield *blockHeightfield;

		int nextWidth;
		int nextDepth;
		HeightSpan * next;

		int lastWidth;
		int lastDepth;

		void toNext() {
			if (next) {
				if (next->getNext()) {
					next = next->getNext();
					return;
				}
				else
					// 当前cell不再有span
					// 移动到grid的下一个cell
					nextWidth++;

				HeightSpan * span = nullptr;
				for (int depthIndex = nextDepth; depthIndex < blockHeightfield->getDepth(); depthIndex++) {
					for (int widthIndex = nextWidth; widthIndex < blockHeightfield->getWidth(); widthIndex++) {
						span = blockHeightfield->spans[blockHeightfield->gridIndex(widthIndex, depthIndex)];
						if (span) {
							next = span;
							nextWidth = widthIndex;
							nextDepth = depthIndex;
							return;
						}
					}
					nextWidth = 0;
				}

				// 说明没有其他span了
				next = nullptr;
				nextDepth = -1;
				nextWidth = -1;
			}
		}
	};

public:
	/*
	用于遍历heightfiled数据的迭代器
	*/
	BlockHeightfield::Iterator* data() { return new BlockHeightfield::Iterator(*this); }

	/*
	返回grid中指定(widthIndex,depthIndex)处的cell的首个span。
	*/
	HeightSpan* getData(int widhtIndex, int depthIndex) { return spans[gridIndex(widhtIndex, depthIndex)]; };

	/*
	判断Heightfield的有效状态。
	*/
	bool hasSpans() { return spans.size() > 0; }

};

#endif // !_BLOCKHEIGHTFIELD_H_
