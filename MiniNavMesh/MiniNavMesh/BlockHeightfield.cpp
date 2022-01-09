#include "BlockHeightfield.h"

bool BlockHeightfield::addData(int widthIndex, int depthIndex, int heightIndexMin, int heightIndexMax, int flags)
{
	if (widthIndex < 0 || widthIndex >= getWidth() || depthIndex < 0 || depthIndex >= getDepth()) {
		// 在grid的无效边界
		return false;
	}

	if (heightIndexMin < 0 || heightIndexMax < 0 || heightIndexMin > heightIndexMax) {
		// 无效的span
		return false;
	}

	// 查找要添加的span在grid中的位置,并获取当前已存在的span
	int _gridIndex = gridIndex(widthIndex, depthIndex);
	HeightSpan * currentSpan = spans[_gridIndex];

	if (!currentSpan) {
		// 当前cell上的第一个span
		spans[_gridIndex] = new HeightSpan(heightIndexMin, heightIndexMax, flags);
		return true;
	}

	// 当前cell存在span数据,搜索当前cell中哪个span可以包含并合并该span
	// 否则创建一个新span
	HeightSpan *previousSpan = nullptr;
	while (currentSpan) {
		if (currentSpan->min() > heightIndexMax + 1) {
			// 新span低于当前span,并且不相邻,所以添加为新span
			HeightSpan * newSpan = new HeightSpan(heightIndexMin, heightIndexMax, flags);
			// 将newSpan插入到currentSpan下面
			newSpan->setNext(currentSpan);

			if (previousSpan) {
				// newSpan在previousSpan与currentSpan之间
				// 将其链接到previousSpan之上
				previousSpan->setNext(newSpan);
			}
			else {
				// newSpan是当前cell的首个span
				// 将其插入当前cell的底部
				spans[_gridIndex] = newSpan;
			}
		}
		else if (currentSpan->max() < heightIndexMin - 1) {
			// currentSpan低于newSpan
			// 且currentSpan没有链接其他span
			if (!currentSpan->getNext()) {
				currentSpan->setNext(new HeightSpan(heightIndexMin, heightIndexMax, flags));
				return true;
			}

			// 查找下一个,直到找到合适的位置
			previousSpan = currentSpan;
			currentSpan = currentSpan->getNext();
		}
		else {
			// currentSpan与newSpan存在某种重合(相邻或重叠)
			// 进行合并并返回

			//----------------------------------------------------------------
			// 这里的思路是这样的:
			// 进行合并时,只考虑待合并的最终的min,max
			//----------------------------------------------------------------


			if (heightIndexMin < currentSpan->min()) {
				currentSpan->setMin(heightIndexMin);
			}

			if (heightIndexMax == currentSpan->max()) {
				// 最大边界一致时,合并flags并返回
				currentSpan->setFlags(char(currentSpan->getFlags() | flags));
				return true;
			}

			if (currentSpan->max() > heightIndexMax) {
				// currentSpan的最高边界大于newSpan
				// 所以此时直接就是包含了newSpan
				// 不需要合并newSpane 的 flags
				return true;
			}

			// currentSpan不与newSpan相邻,查找最终适合合并的那个span
			HeightSpan * nextSpan = currentSpan->getNext();
			while (true) {

				// currentSpan之上没有span
				// 或者nextSpan高于newSpan
				// 此时处理的是拉伸currentSpan到当前newSpan顶部,然后链接或合并当前的nextSpan
				// 则合并到currentSpan(别忘了我们的场景是体素化后,因此合并后只会与nextSpan相邻而不会重叠)
				if (!nextSpan || nextSpan->min() > heightIndexMax + 1) {
					currentSpan->setMax(heightIndexMax);

					// 此时相当于currentSpan被扩展了(往顶部扩展)
					// 因此我们将currentSpan更新为newSpan的flags
					currentSpan->setFlags(flags);

					if (!nextSpan) {
						// 可以想象下,在currentSpan之后的所有span此时已经被一个
						// 扩展后的currentSpan包含了
						// 所以此时就没有下一个链接了
						currentSpan->setNext(nullptr);
					}
					else {
						// 扩展后,由于此时currentSpan到达了下一个最靠近当前nextSpan的顶部
						// 所以此时直接链接最后这个nextSpan
						currentSpan->setNext(nextSpan);
					}

					return true;
				}

				// 此时,nextSpan在newSpan的上方或者可能与nextSpan重合
				if (nextSpan->min() == heightIndexMax + 1 || heightIndexMax <= nextSpan->max()) {
					currentSpan->setMax(nextSpan->max());
					currentSpan->setNext(nextSpan->getNext());

					// 此时已经拓展到newSpan的顶部,所以currentSpan就直接为nextSpan的flags
					currentSpan->setFlags(nextSpan->getFlags());
					if (currentSpan->max() == heightIndexMax) {
						currentSpan->setFlags(currentSpan->getFlags() | flags);
						return true;
					}

					return true;
				}

				nextSpan = nextSpan->getNext();
			}

		}
	}

	return false;
}
