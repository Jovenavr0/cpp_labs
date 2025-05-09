#include <lib/array.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace TDArray;

TEST(ArrayTest, SingleTest) {
	ThreeDimensionalArray array(1, 1, 1);

	ASSERT_EQ(int(array[1][1][1]), 0);
}

TEST(ArrayTest, CorrectSizeTest) {
	ThreeDimensionalArray array(1, 2, 3);
	array[1][1][1] = 1;
	array[1][1][2] = 2;
	array[1][1][3] = 3;
	array[1][2][1] = 4;
	array[1][2][2] = 5;
	array[1][2][3] = 6;
	int index = 1;
	for (int x = 1; x <= 1; x++) {
		for (int y = 1; y <= 2; y++) {
			for (int z = 1; z <= 3; z++) {
				ASSERT_EQ(int(array[x][y][z]), index++);
			}
		}
	}
}

TEST(ArrayTest, Max17BitValueTest) {
	ThreeDimensionalArray array(1, 1, 1);
	array[1][1][1] = 131071;

	ASSERT_EQ(int(array[1][1][1]), 131071);
}

TEST(ArrayTest, SumTo17BitValueTest) {
	ThreeDimensionalArray array1(1, 1, 1);
	ThreeDimensionalArray array2(1, 1, 1);
	array1[1][1][1] = 65536;
	array2[1][1][1] = 65535;
	array1 = array1 + array2;

	ASSERT_EQ(int(array1[1][1][1]), 131071);
}


TEST(ArrayTest, BigSingleTest) {
	ThreeDimensionalArray array(10, 10, 10);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array[x][y][z]), 0);
			}
		}
	}

}

TEST(ArrayTest, ValueTest) {
	ThreeDimensionalArray array(1, 1, 1);
	array[1][1][1] = 100500;

	ASSERT_EQ(int(array[1][1][1]), 100500);
}

TEST(ArrayTest, SumSingleTest) {
	ThreeDimensionalArray array1(1, 1, 1);
	ThreeDimensionalArray array2(1, 1, 1);
	array1[1][1][1] = 100499;
	array2[1][1][1] = 1;
	array1 = array1 + array2;

	ASSERT_EQ(int(array1[1][1][1]), 100500);
}

TEST(ArrayTest, BigSumTest) {
	ThreeDimensionalArray array1(10, 10, 10);
	ThreeDimensionalArray array2(10, 10, 10);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				array1[x][y][z] = 1;
				array2[x][y][z] = 2;
			}
		}
	}

	array1 = array1 + array2;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array1[x][y][z]), 3);
			}
		}
	}
}

TEST(ArrayTest, MultiSingleTest) {
	ThreeDimensionalArray array(1, 1, 1);
	array[1][1][1] = 25;
	array = array * 5;


	ASSERT_EQ(int(array[1][1][1]), 125);
}

TEST(ArrayTest, BigMultiTest) {
	ThreeDimensionalArray array(10, 10, 10);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				array[x][y][z] = 1000;
			}
		}
	}

	array = array * 5;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array[x][y][z]), 5000);
			}
		}
	}
}

TEST(ArrayTest, MultiTest) {
	ThreeDimensionalArray array(1, 2, 1);
	array[1][1][1] = 25;
	array[1][2][1] = 50;
	array = array * 5;


	ASSERT_EQ(int(array[1][1][1]), 125);
	ASSERT_EQ(int(array[1][2][1]), 250);
}

TEST(ArrayTest, SubtractionTest) {
	ThreeDimensionalArray array1(1, 1, 1);
	ThreeDimensionalArray array2(1, 1, 1);
	array1[1][1][1] = 50;
	array2[1][1][1] = 25;
	array1 = array1 - array2;


	ASSERT_EQ(int(array1[1][1][1]), 25);
}

TEST(ArrayTest, BigSubtractionTest) {
	ThreeDimensionalArray array1(10, 10, 10);
	ThreeDimensionalArray array2(10, 10, 10);
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				array1[x][y][z] = 100567;
				array2[x][y][z] = 67;
			}
		}
	}

	array1 = array1 - array2;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array1[x][y][z]), 100500);
			}
		}
	}
}

TEST(ArrayTest, SubtractionAndMultTest) {
	ThreeDimensionalArray array1(10, 10, 10);
	ThreeDimensionalArray array2(10, 10, 10);
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				array1[x][y][z] = 167;
				array2[x][y][z] = 67;
			}
		}
	}

	array1 = (array1 - array2) * 25;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array1[x][y][z]), 2500);
			}
		}
	}
}

TEST(ArrayTest, SumAndMultiTest) {
	ThreeDimensionalArray array1(10, 10, 10);
	ThreeDimensionalArray array2(10, 10, 10);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				array1[x][y][z] = 15;
				array2[x][y][z] = 25;
			}
		}
	}

	array1 = (array1 + array2) * 40;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			for (int z = 1; z <= 10; z++) {
				ASSERT_EQ(int(array1[x][y][z]), 1600);
			}
		}
	}

}