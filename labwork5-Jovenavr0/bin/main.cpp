#include "lib/array.h"
#include <iostream>

using namespace TDArray;

int main(){

	ThreeDimensionalArray array = ThreeDimensionalArray::make_array(3, 4, 5);
	ThreeDimensionalArray array2 = ThreeDimensionalArray::make_array(3, 4, 5);
	ThreeDimensionalArray ans(3, 4, 5);

	std::cin >> array[3][3][3];

	std::cout << int(array[3][3][3]) << ' ';

	array[1][1][1] = 65537;
	std::cout << int(array[1][1][1]) << ' ';
	array2[1][1][1] = 1;

	ans = array - array2;
	std::cout << int(ans[1][1][1]) << ' ';

	ans = array + array2;
	std::cout << int(ans[2][2][2]) << ' ';

	array[2][2][2] = 500;
	ans = array * 10;
	std::cout << int(ans[2][2][2]) << ' ';

	array[1][1][1] = 0;
	std::cout << array;

	return 0;
}