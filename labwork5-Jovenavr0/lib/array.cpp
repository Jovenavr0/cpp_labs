#include <iostream>
#include "array.h"

int KMax17BitNumber = 65536;
int KCountBitIn16BitNumber = 16;
int KMaxIndexOfBitIn16BitNumber = 15;

TDArray::ThreeDimensionalArray TDArray::ThreeDimensionalArray::make_array(const int& x_axis, const int& y_axis, const int& z_axis) {
	ThreeDimensionalArray array(x_axis, y_axis, z_axis);
	return array;
}

TDArray::ThreeDimensionalArray::TwoDimensionalArray TDArray::ThreeDimensionalArray::operator[](const int &a) {
	TwoDimensionalArray now_value;
	now_value.now_array = this;
	now_value.two_dimensional_index = (a - 1);
	return now_value;
}

TDArray::ThreeDimensionalArray& TDArray::ThreeDimensionalArray::operator=(const int& value) {
	field_[now_index_value_] = value % KMax17BitNumber;
	mas_of_first_bit_[now_index_value_ / KCountBitIn16BitNumber] += (((value / KMax17BitNumber) % 2) << (KMaxIndexOfBitIn16BitNumber - (now_index_value_ % KCountBitIn16BitNumber)));
	return *this;
}

std::istream &TDArray::operator>>(std::istream &stream, TDArray::ThreeDimensionalArray &array) {
	stream >> array.field_[array.now_index_value_];
	return stream;
}

std::ostream& TDArray::operator<<(std::ostream& stream, const TDArray::ThreeDimensionalArray& array) {

	int value;
	for (int index = 0; index < array.size_of_field_; ++index) {
		value = array.field_[index];
		value += ((array.mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2 * KMax17BitNumber);
		stream << value;
		stream << ' ';
	}

	return stream;
}

TDArray::ThreeDimensionalArray TDArray::ThreeDimensionalArray::operator+(const TDArray::ThreeDimensionalArray &rhs) {
	TDArray::ThreeDimensionalArray answer(x_axis_, y_axis_, z_axis_);
	for (int index = 0; index < size_of_field_; index++) {
		answer.field_[index] = (field_[index] + rhs.field_[index]) % KMax17BitNumber;
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber] += ((((field_[index] + rhs.field_[index]) / KMax17BitNumber) % 2) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber)));
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber] += ((mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber));
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber] += ((rhs.mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber));
	}

	return answer;
}

TDArray::ThreeDimensionalArray TDArray::ThreeDimensionalArray::operator*(const int &multiple) {
	TDArray::ThreeDimensionalArray answer(x_axis_, y_axis_, z_axis_);
	for (int index = 0; index < size_of_field_; index++) {
		answer.field_[index] = (field_[index] * multiple) % KMax17BitNumber;
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber] += ((((field_[index] * multiple) / KMax17BitNumber) % 2) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber)));
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber]+= ((mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2 * multiple) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber));
	}

	return answer;
}

TDArray::ThreeDimensionalArray TDArray::ThreeDimensionalArray::operator-(const TDArray::ThreeDimensionalArray &rhs) {
	TDArray::ThreeDimensionalArray answer(x_axis_, y_axis_, z_axis_);
	int value_lhs;
	int value_rhs;
	int value_answer;
	for (int index = 0; index < size_of_field_; ++index) {
		value_lhs = field_[index] + ((mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2 * KMax17BitNumber);
		value_rhs = rhs.field_[index] + ((rhs.mas_of_first_bit_[index / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber))) % 2 * KMax17BitNumber);
		value_answer = value_lhs - value_rhs;
		answer.field_[index] = value_answer % KMax17BitNumber;
		answer.mas_of_first_bit_[index / KCountBitIn16BitNumber] += (((value_answer / KMax17BitNumber) % 2) << (KMaxIndexOfBitIn16BitNumber - (index % KCountBitIn16BitNumber)));
	}

	return answer;
}

TDArray::ThreeDimensionalArray::operator int() {
	int value = field_[now_index_value_];
	value += ((mas_of_first_bit_[now_index_value_ / KCountBitIn16BitNumber] >> (KMaxIndexOfBitIn16BitNumber - (now_index_value_ % KCountBitIn16BitNumber))) % 2 * KMax17BitNumber);
	return value;
}

TDArray::ThreeDimensionalArray::OneDimensionalArray TDArray::ThreeDimensionalArray::TwoDimensionalArray::operator[](const int &b) {
	OneDimensionalArray mas;
	mas.now_array = this -> now_array;
	mas.one_dimensional_index = this -> two_dimensional_index + (this -> now_array -> x_axis_) * (b - 1);
	return mas;
}

TDArray::ThreeDimensionalArray& TDArray::ThreeDimensionalArray::OneDimensionalArray::operator[](const int &c) {
	this -> one_dimensional_index += ((this -> now_array -> x_axis_) * (this -> now_array -> y_axis_) * (c - 1));
	this -> now_array -> now_index_value_  = this -> one_dimensional_index;
	return *this -> now_array;
}
