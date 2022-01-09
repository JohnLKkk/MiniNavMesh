#include "BlockHeightfield.h"

bool BlockHeightfield::addData(int widthIndex, int depthIndex, int heightIndexMin, int heightIndexMax, int flags)
{
	if (widthIndex < 0 || widthIndex >= getWidth() || depthIndex < 0 || depthIndex >= getDepth()) {
		// ��grid����Ч�߽�
		return false;
	}

	if (heightIndexMin < 0 || heightIndexMax < 0 || heightIndexMin > heightIndexMax) {
		// ��Ч��span
		return false;
	}

	// ����Ҫ��ӵ�span��grid�е�λ��,����ȡ��ǰ�Ѵ��ڵ�span
	int _gridIndex = gridIndex(widthIndex, depthIndex);
	HeightSpan * currentSpan = spans[_gridIndex];

	if (!currentSpan) {
		// ��ǰcell�ϵĵ�һ��span
		spans[_gridIndex] = new HeightSpan(heightIndexMin, heightIndexMax, flags);
		return true;
	}

	// ��ǰcell����span����,������ǰcell���ĸ�span���԰������ϲ���span
	// ���򴴽�һ����span
	HeightSpan *previousSpan = nullptr;
	while (currentSpan) {
		if (currentSpan->min() > heightIndexMax + 1) {
			// ��span���ڵ�ǰspan,���Ҳ�����,�������Ϊ��span
			HeightSpan * newSpan = new HeightSpan(heightIndexMin, heightIndexMax, flags);
			// ��newSpan���뵽currentSpan����
			newSpan->setNext(currentSpan);

			if (previousSpan) {
				// newSpan��previousSpan��currentSpan֮��
				// �������ӵ�previousSpan֮��
				previousSpan->setNext(newSpan);
			}
			else {
				// newSpan�ǵ�ǰcell���׸�span
				// ������뵱ǰcell�ĵײ�
				spans[_gridIndex] = newSpan;
			}
		}
		else if (currentSpan->max() < heightIndexMin - 1) {
			// currentSpan����newSpan
			// ��currentSpanû����������span
			if (!currentSpan->getNext()) {
				currentSpan->setNext(new HeightSpan(heightIndexMin, heightIndexMax, flags));
				return true;
			}

			// ������һ��,ֱ���ҵ����ʵ�λ��
			previousSpan = currentSpan;
			currentSpan = currentSpan->getNext();
		}
		else {
			// currentSpan��newSpan����ĳ���غ�(���ڻ��ص�)
			// ���кϲ�������

			//----------------------------------------------------------------
			// �����˼·��������:
			// ���кϲ�ʱ,ֻ���Ǵ��ϲ������յ�min,max
			//----------------------------------------------------------------


			if (heightIndexMin < currentSpan->min()) {
				currentSpan->setMin(heightIndexMin);
			}

			if (heightIndexMax == currentSpan->max()) {
				// ���߽�һ��ʱ,�ϲ�flags������
				currentSpan->setFlags(char(currentSpan->getFlags() | flags));
				return true;
			}

			if (currentSpan->max() > heightIndexMax) {
				// currentSpan����߽߱����newSpan
				// ���Դ�ʱֱ�Ӿ��ǰ�����newSpan
				// ����Ҫ�ϲ�newSpane �� flags
				return true;
			}

			// currentSpan����newSpan����,���������ʺϺϲ����Ǹ�span
			HeightSpan * nextSpan = currentSpan->getNext();
			while (true) {

				// currentSpan֮��û��span
				// ����nextSpan����newSpan
				// ��ʱ�����������currentSpan����ǰnewSpan����,Ȼ�����ӻ�ϲ���ǰ��nextSpan
				// ��ϲ���currentSpan(���������ǵĳ��������ػ���,��˺ϲ���ֻ����nextSpan���ڶ������ص�)
				if (!nextSpan || nextSpan->min() > heightIndexMax + 1) {
					currentSpan->setMax(heightIndexMax);

					// ��ʱ�൱��currentSpan����չ��(��������չ)
					// ������ǽ�currentSpan����ΪnewSpan��flags
					currentSpan->setFlags(flags);

					if (!nextSpan) {
						// ����������,��currentSpan֮�������span��ʱ�Ѿ���һ��
						// ��չ���currentSpan������
						// ���Դ�ʱ��û����һ��������
						currentSpan->setNext(nullptr);
					}
					else {
						// ��չ��,���ڴ�ʱcurrentSpan��������һ�������ǰnextSpan�Ķ���
						// ���Դ�ʱֱ������������nextSpan
						currentSpan->setNext(nextSpan);
					}

					return true;
				}

				// ��ʱ,nextSpan��newSpan���Ϸ����߿�����nextSpan�غ�
				if (nextSpan->min() == heightIndexMax + 1 || heightIndexMax <= nextSpan->max()) {
					currentSpan->setMax(nextSpan->max());
					currentSpan->setNext(nextSpan->getNext());

					// ��ʱ�Ѿ���չ��newSpan�Ķ���,����currentSpan��ֱ��ΪnextSpan��flags
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
