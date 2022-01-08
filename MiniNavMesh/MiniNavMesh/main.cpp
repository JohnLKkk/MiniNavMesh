#include <iostream>
#include "Region.h"

int main()
{
	Region *region = new Region(2);
	std::cout << *region << std::endl;
	system("pause");
	return 0;
}