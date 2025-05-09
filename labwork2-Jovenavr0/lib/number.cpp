#include "number.h"
#include <cstring>
#include <cmath>

const int kBasisOurSystem = 256;
const int kMaxCountByte = 253;
const int kMaxCountBit = 2024;
const int kMaxDigitOfNumbers = 609;
const int kAsciiRepresentation0 = 48;
const int kValueOfLastByteWithLastBit = 128;
const int kCountByteInt = 4;
const int kCountBitInByte = 8;

int2023_t from_int(int32_t buff){
    int2023_t result;
    bool is_negative = (buff < 0);    //check for negative

    if (is_negative){
		buff *= -1;
	}

    for (int i = 0; i < kCountByteInt; i++){
        result.mas[i] = buff % kBasisOurSystem;
        buff /= kBasisOurSystem;
    }

    if(is_negative){
        for(int i = 0; i < kMaxCountByte; i++){
            result.mas[i] = ~result.mas[i];
        }
    }

    return result;
}

int2023_t from_string(const char* buff){
    int2023_t result;
    bool sign = false;
    if(buff[0] == '-'){
		sign = true;
	}
    int k = 0;
    char string[kMaxDigitOfNumbers] = {0};
    char bit_of_string_digit[kMaxCountBit] = {0};
    int len_buff = strlen(buff) - sign; //without sign
    int now_len = len_buff;

    for(int i = 0; i < len_buff; i++){
		string[i] = buff[i + sign]; //don't write down the sign
	}

    while(now_len != 0){

        if((static_cast<int>(string[len_buff - now_len] - kAsciiRepresentation0) / 2) == 0) {
			--now_len;
		}

        //
        for(int i = (len_buff - 1); i >= 0; i--){
            if(i == (len_buff - 1)){
				bit_of_string_digit[k++] = static_cast<int>(string[i] - kAsciiRepresentation0) % 2;
			}
            else{
				string[i + 1] += ((static_cast<int>(string[i] - kAsciiRepresentation0) % 2) * 5);
			}
            string[i] = (static_cast<int>(string[i] - kAsciiRepresentation0) / 2) + '0';
        }

    }

    for(int i = 0; i < kMaxCountBit; i++){
        result.mas[i / kCountBitInByte] += (static_cast<int>(bit_of_string_digit[i]) * pow(2, (i % kCountBitInByte)));
    }

    if(sign){
        for(int i = 0; i < kMaxCountByte; i++) result.mas[i] = ~result.mas[i];
    }

    return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs){
    int2023_t result;
    bool flag = false;  //fix -0
    int transition_prev_category = 0;
    int digit_lhs;
    int digit_rhs;
    bool sign_lhs = take_sign(lhs);
    bool sign_rhs = take_sign(rhs);

    if((sign_lhs + sign_rhs) % 2){  //for fix -0

        for(int i = 0; i < kMaxCountByte; i++){
            if(~lhs.mas[i] != rhs.mas[i]){
                flag = true;
                break;
            }
        }

    }

    if(flag){
		transition_prev_category++;    //fix -0
	}

    for (int i = 0; i < kMaxCountByte; i++) {
        digit_lhs = lhs.mas[i];
        digit_rhs = rhs.mas[i];
        result.mas[i] = (transition_prev_category + digit_lhs + digit_rhs) % kBasisOurSystem;
        transition_prev_category = (transition_prev_category + digit_lhs + digit_rhs) / kBasisOurSystem;
    }

    if(flag){
		transition_prev_category--;    //fix -0
	}

    if(transition_prev_category){   //implementation for negative; if there is 1 left after addition which went beyond the number, then add it
        for(int i = 0; i < kMaxCountByte; i++) {
            result.mas[i] = (result.mas[i] + transition_prev_category) % kBasisOurSystem;
            transition_prev_category = (result.mas[i] + transition_prev_category) / kBasisOurSystem;
        }
    }

    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t value_rhs = rhs;

    for(int i = 0; i < kMaxCountByte; i++){
		value_rhs.mas[i] = ~rhs.mas[i];
	}

    result = lhs + value_rhs;

    return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs){
    int2023_t result;
    int2023_t x = lhs;
    int2023_t y = rhs;
    int byte_lhs;
    int byte_rhs;
    int transition_prev_category;
    int actual_value;
    bool sign_lhs = take_sign(lhs);
    bool sign_rhs = take_sign(rhs);

    if(sign_lhs){
        for (int i = 0; i < kMaxCountByte; i++){
			x.mas[i] = ~x.mas[i];
		}
    }

    if(sign_rhs){
        for (int i = 0; i < kMaxCountByte; i++){
			y.mas[i] = ~y.mas[i];
		}
    }

    for(int i = 0; i < kMaxCountByte; i++){
        byte_rhs = y.mas[i];
        transition_prev_category = 0;
        for(int j = 0; j < kMaxCountByte; j++){
            byte_lhs = x.mas[j];
            if ((i + j) > 252) break;
            actual_value = transition_prev_category + byte_lhs * byte_rhs + result.mas[i + j];
            result.mas[i + j] = (actual_value) % kBasisOurSystem;
            transition_prev_category = (actual_value) / kBasisOurSystem;
        }
    }

    if((sign_lhs + sign_rhs) % 2){
        for(int i = 0; i < kMaxCountByte; i++){
			result.mas[i] = ~result.mas[i];
		}
    }

    return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs){
    int2023_t result;
    int2023_t middle_lhs;
    int2023_t value_lhs = lhs;
    int2023_t value_rhs = rhs;
    int result_bit[kMaxCountBit];
    int count_byte_lhs = 0;
    int k = 0;
    bool flag = true;

    for(int i = 0; i < kMaxCountByte; i++){    //for fix ZeroDivisionError
        if(rhs.mas[i] != 0){
			flag = false;
		}
    }

    if(flag){
        std::cerr << "ZeroDivisionError: division by zero";
        exit(-1);
    }

    if(take_sign(lhs)){
        for(int i = 0; i < kMaxCountByte; i++){
			value_lhs.mas[i] = ~value_lhs.mas[i];
		}
    }

    if(take_sign(rhs)){
        for(int i = 0; i < kMaxCountByte; i++){
			value_rhs.mas[i] = ~value_rhs.mas[i];
		}
    }

    for(int i = 0; i < kMaxCountByte; i++){
        if(value_lhs.mas[i]){
			count_byte_lhs = i + 1;
		}
    }

    for(int i = (count_byte_lhs * kCountBitInByte - 1); i >= 0; i--){
        middle_lhs = middle_lhs * from_int(2);
        middle_lhs = middle_lhs + from_int((value_lhs.mas[i / kCountBitInByte] >> (i % kCountBitInByte)) % 2);
        if(middle_lhs >= value_rhs){
            middle_lhs = middle_lhs - value_rhs;
            result_bit[k++] = 1;
        }
        else{
			result_bit[k++] = 0;
		}
    }

    for(int i = (k - 1); i >= 0; i--){
        result.mas[((k - 1) - i) / kCountBitInByte] += (result_bit[i] * pow(2, ((k - 1) - i) % kCountBitInByte));
    }

    if((take_sign(lhs) + take_sign(rhs)) % 2){
        if(result != from_int(0)){
			result = result * from_int(-1);
		}
    }

    return result;
}

int2023_t operator%(const int2023_t& lhs, const int2023_t& rhs){
	int2023_t result = from_int(0);
	int2023_t value_rhs = from_int(1);
	while(lhs >= value_rhs * rhs){
		result = result + from_int(1);
		value_rhs = value_rhs * rhs;
	}

	return result;
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs){
    int2023_t value_lhs = lhs;
    int2023_t value_rhs = rhs;
    bool result = true;

    if(take_sign(lhs) == take_sign(rhs)){

        if(take_sign(lhs)){
            for(int i = 0; i < kMaxCountByte; i++){
				value_lhs.mas[i] = ~value_lhs.mas[i];
			}
        }

        if(take_sign(rhs)){
            for(int i = 0; i < kMaxCountByte; i++){
				value_rhs.mas[i] = ~value_rhs.mas[i];
			}
        }

        for(int i = 0; i < kMaxCountByte; i++){
            if(value_lhs.mas[i] > value_rhs.mas[i]){
                result = true;
            }
            else if(value_lhs.mas[i] < value_rhs.mas[i]){
				result = false;
			}
        }

        if(take_sign(lhs) && !(lhs == rhs)){
			result = !result;
		}
    }
    else{
		result = take_sign(lhs) > take_sign(rhs);
    }

    return result;
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs){
    return (lhs > rhs) || (lhs == rhs);
}

bool take_sign(const int2023_t& value){
     return value.mas[kMaxCountByte - 1] >= kValueOfLastByteWithLastBit;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    bool answer = true;
    for(int i = 0; i < kMaxCountByte; i++){
        if(lhs.mas[i] != rhs.mas[i]){
			answer = false;
		}
    }

    return answer;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value){
    int digit_of_value;
    int k = 0;
    for (int i = (kMaxCountByte - 1); i >= 0; i--){
        digit_of_value = value.mas[i];
        int byte[kCountBitInByte] = {0};
        while (digit_of_value != 0){
            byte[k] = digit_of_value % 2;
            k++;
            digit_of_value /= 2;
        }
        k = 0;
        for(int j = (kCountBitInByte - 1); j >= 0; j--){
            stream << byte[j];
        }
    }

    return stream;
}