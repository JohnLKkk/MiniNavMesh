#pragma once
#ifndef _HEIGHTSPANE_H_
/*
Spans��ʾһ����������������Ϣ����HeightSpan��ʾ��ǰCell(����״��ʽȥ���,���Գ�ΪHeightSpan)�ڵ�Span��
HeightField����һϵ��HeightSpan��ɵġ�
����:JohnKkk
����:2022��1��8��12��51��
*/
class HeightSpan
{
public:
	HeightSpan(int min, int max, int flags) :miniMum(min), maxiMum(max), flags(flags), next(nullptr){
		// ע�������Ǳ������,��Ӧ�ô���min > max�Ĳ���,������Ч��
		// �������ﲻ���κζ��ദ��
	};

	~HeightSpan() {
		delete next;
		next = nullptr;
	};

	/*
	Span�ı�ǡ�
	*/
	int getFlags()const { return flags; }

	int max()const { return maxiMum; };

	int min()const { return miniMum; };

	/*
	cell����һ��Span,ͨ�����ڵ�ǰSpan��
	*/
	HeightSpan* getNext()const { return next; };

	void setFlags(int value) { flags = value; };

	/*
	����Span�����߽�,�Զ�ƥ�䵽��Ч��Χ�ڡ�
	*/
	void setMax(int value) {
		if (value <= miniMum) {
			maxiMum = miniMum + 1;
		}
		else {
			maxiMum = value;
		}
	}

	/*
	����Span����С�߽磬�Զ�ƥ�䵽��Ч��Χ�ڡ�
	*/
	void setMin(int value) {
		if (value >= maxiMum) {
			miniMum = maxiMum - 1;
		}
		else {
			miniMum = value;
		}
	}

	/*
	������һ��Spane��
	nullptr��һ����Чֵ��
	*/
	void setNext(HeightSpan *value) {
		next = value;
	}

private:
	int miniMum;
	int maxiMum;
	int flags;
	HeightSpan *next;
};

#endif // !_HEIGHTSPANE_H_
