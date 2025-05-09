#pragma once
#include <cinttypes>
#include <iostream>

namespace TDArray{

class ThreeDimensionalArray{
public:

	friend std::ostream& operator<<(std::ostream& stream, const TDArray::ThreeDimensionalArray& array);
	friend std::istream& operator>>(std::istream& stream, TDArray::ThreeDimensionalArray& array);

	struct OneDimensionalArray{
		TDArray::ThreeDimensionalArray& operator[](const int& c);
		TDArray::ThreeDimensionalArray* now_array;

//		~OneDimensionalArray(){
//			delete now_array;
//		}

		int one_dimensional_index;
	};

	struct TwoDimensionalArray{
		OneDimensionalArray operator[](const int& b);
		TDArray::ThreeDimensionalArray* now_array;

//		~TwoDimensionalArray(){
//			delete now_array;
//		}

		int two_dimensional_index;
	};

	ThreeDimensionalArray(int x_axis, int y_axis, int z_axis): x_axis_(x_axis), y_axis_(y_axis), z_axis_(z_axis), size_of_field_(x_axis * y_axis * z_axis), field_(new uint16_t[x_axis * y_axis * z_axis]{}), mas_of_first_bit_(new uint16_t[(x_axis * y_axis * z_axis - 1) / 16 + 1]{})
	{}

	ThreeDimensionalArray(const TDArray::ThreeDimensionalArray& other): x_axis_(other.x_axis_), y_axis_(other.y_axis_), z_axis_(other.z_axis_), size_of_field_(other.size_of_field_), field_(new uint16_t[other.size_of_field_]{}), mas_of_first_bit_(new uint16_t[(other.size_of_field_ - 1) / 16 + 1]{})
	{}

//	~ThreeDimensionalArray(){
//		delete[] field_;
//		delete[] mas_of_first_bit_;
//	}

	static ThreeDimensionalArray make_array(const int& x_axis, const int& y_axis, const int& z_axis);

	TDArray::ThreeDimensionalArray::TwoDimensionalArray operator[](const int& a);

	TDArray::ThreeDimensionalArray& operator=(const int& value);

//	TDArray::ThreeDimensionalArray operator=(const TDArray::ThreeDimensionalArray& other);

	TDArray::ThreeDimensionalArray operator+(const TDArray::ThreeDimensionalArray& rhs);

	TDArray::ThreeDimensionalArray operator-(const TDArray::ThreeDimensionalArray& rhs);

	TDArray::ThreeDimensionalArray operator*(const int& multiple);

	operator int();


private:
	uint16_t* field_;
	uint16_t* mas_of_first_bit_;
	int now_index_value_ = 0;
	int x_axis_;
	int y_axis_;
	int z_axis_;
	int size_of_field_;
};

std::ostream& operator<<(std::ostream& stream, const TDArray::ThreeDimensionalArray& array);
std::istream& operator>>(std::istream& stream, TDArray::ThreeDimensionalArray& array);

}