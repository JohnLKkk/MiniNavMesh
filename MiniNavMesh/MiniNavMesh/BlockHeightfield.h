#pragma once
#ifndef _BLOCKHEIGHTFIELD_H_
#define _BLOCKHEIGHTFIELD_H_


#include "BoundedField.h"
#include "HeightSpan.h"
#include <unordered_map>

using std::unordered_map;

/*
BlockHeightfield���ڱ�ʾ���ػ�����һϵ��cell��ɵ�grid,����ÿ��cell����һ��heightspan(heightspan��ʾ����Դ����mesh������span)��
����:JohnKkk
����:2022��1��9��10��20��
����:2022��1��9��13��13��
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
	��span��ӵ�heightfield�У��µ�spanҪô�ϲ������е�span�ϣ�Ҫô����һ���µ�span��
	�������²���:
	1.widthIndex��depthIndex�ı߽���ԡ�
	2.heightIndex�����ޡ�
	3.height min <= max��
	4.���������߶��Ƿ�����Ч��Χ�ڡ�
	��ǵĸ�������:
	1.��������ݵ�maxiMum��һ�����е�span��maxiMumһ�£���ϲ�flag��
	2.�����span��maxiMum��ʾ�µ����ֵ��������Ϊ��span��flag
	3.���򣬺�����span��flag
	�����֮��ֻ��span�Ķ�����flag������Ҫ��(��Ϊ���տɱ����ı������span�Ķ���(Ҳ����floor))
	@param widthIndex ��span��cell�������
	@param depthIndex ��span��cell�������
	@param heightIndexMin ��span�ĸ߶ȱ߽����ֵ
	@param heightIndexMax ��span�ĸ߶ȱ߽����ֵ
	@param flags �µ�flags
	@return �ɹ���ӷ���true���������false��Ψһ�Ŀ�����ĳЩ�������µ���Ч����
	*/
	bool addData(int widthIndex, int depthIndex, int heightIndexMin, int heightIndexMax, int flags);


	
private:

	// ����Heightfield��spans
	// key:ͨ��gridIndex()��ȡgrid��Index
	// value:��ǰgridIndex�����λ�õ�span,�����ǰcellû��span,��Ϊnullptr
	unordered_map<int, HeightSpan*> spans;


	class Iterator
	{
	public:
		Iterator(BlockHeightfield & blockHeightfield) :blockHeightfield(&blockHeightfield), lastDepth(-1), lastWidth(-1), next(nullptr), nextWidth(0), nextDepth(0) {
			toNext();
		};
		~Iterator() {};

		/*
		�������һ�����ʵ�span��depthIndex��
		*/
		int depthIndex()const { return lastDepth; }

		bool allow()const { return next; }

		HeightSpan& getNext() {
			// ���赱ǰnext!=nullptr

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
		���������ʵ�span��widthIndex��
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
					// ��ǰcell������span
					// �ƶ���grid����һ��cell
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

				// ˵��û������span��
				next = nullptr;
				nextDepth = -1;
				nextWidth = -1;
			}
		}
	};

public:
	/*
	���ڱ���heightfiled���ݵĵ�����
	*/
	BlockHeightfield::Iterator* data() { return new BlockHeightfield::Iterator(*this); }

	/*
	����grid��ָ��(widthIndex,depthIndex)����cell���׸�span��
	*/
	HeightSpan* getData(int widhtIndex, int depthIndex) { return spans[gridIndex(widhtIndex, depthIndex)]; };

	/*
	�ж�Heightfield����Ч״̬��
	*/
	bool hasSpans() { return spans.size() > 0; }

};

#endif // !_BLOCKHEIGHTFIELD_H_
