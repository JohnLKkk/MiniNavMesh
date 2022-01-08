#pragma once
#ifndef _HEIGHTSPANE_H_
/*
Spans表示一个或多个连续体素信息，而HeightSpan表示当前Cell(以柱状方式去理解,所以称为HeightSpan)内的Span。
HeightField是由一系列HeightSpan组成的。
作者:JohnKkk
日期:2022年1月8日12点51分
*/
class HeightSpan
{
public:
	HeightSpan(int min, int max, int flags) :miniMum(min), maxiMum(max), flags(flags), next(nullptr){
		// 注意这里的潜在问题,不应该传入min > max的参数,这是无效的
		// 但是这里不做任何多余处理
	};

	~HeightSpan() {
		delete next;
		next = nullptr;
	};

	/*
	Span的标记。
	*/
	int getFlags()const { return flags; }

	int max()const { return maxiMum; };

	int min()const { return miniMum; };

	/*
	cell的下一个Span,通常高于当前Span。
	*/
	HeightSpan* getNext()const { return next; };

	void setFlags(int value) { flags = value; };

	/*
	设置Span的最大边界,自动匹配到有效范围内。
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
	设置Span的最小边界，自动匹配到有效范围内。
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
	设置下一个Spane。
	nullptr是一个有效值。
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
