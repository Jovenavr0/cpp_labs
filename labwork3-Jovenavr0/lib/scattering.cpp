#include "scattering.h"
#include "bmp.h"
#include <cstring>

int KCountBitOnByte = 8;
int KBorderCout = 4;

void SaveYourMemory(MainPointField field) {

	for (int i = 0; i < field.y; i++){
		delete[] field.mas[i];
	}

	delete[] field.mas;

}

MainPointField ExpansionRight(MainPointField field) {
	field.x += 1;
	uint64_t** mas = new uint64_t*[field.y];

	for (int i = 0; i < field.y; i++) {
		mas[i] = new uint64_t[field.x];
		memset(mas[i], 0, field.x * KCountBitOnByte);
		for (int j = 0; j < (field.x - 1); j++) {
			mas[i][j] = field.mas[i][j];
		}
		delete[] field.mas[i];
	}
	delete[] field.mas;
	field.mas = mas;

	return field;
}

MainPointField ExpansionLeft(MainPointField field) {
	field.x += 1;
	uint64_t** mas = new uint64_t*[field.y];

	for (int i = 0; i < field.y; i++) {
		mas[i] = new uint64_t[field.x];
		memset(mas[i], 0, field.x * KCountBitOnByte);
		for (int j = 0; j < (field.x - 1); j++) {
			mas[i][j + 1] = field.mas[i][j];
		}
		delete[] field.mas[i];
	}
	delete[] field.mas;
	field.mas = mas;

	return field;
}

MainPointField ExpansionTop(MainPointField field) {
	field.y += 1;
	uint64_t** mas = new uint64_t*[field.y];

	mas[0] = new uint64_t[field.x];
	memset(mas[0], 0, field.x * KCountBitOnByte);
	for (int i = 1; i < field.y; i++) {
		mas[i] = new uint64_t[field.x];
		memset(mas[i], 0, field.x * KCountBitOnByte);
		for (int j = 0; j < field.x; j++) {
			mas[i][j] = field.mas[i - 1][j];
		}
		delete[] field.mas[i - 1];
	}
	delete[] field.mas;

	field.mas = mas;

	return field;
}

MainPointField ExpansionBottom(MainPointField field) {
	field.y += 1;
	uint64_t** mas = new uint64_t*[field.y];

	for (int i = 0; i < (field.y - 1); i++) {
		mas[i] = new uint64_t[field.x];
		memset(mas[i], 0, field.x * KCountBitOnByte);
		for (int j = 0; j < field.x; j++) {
			mas[i][j] = field.mas[i][j];
		}
		delete[] field.mas[i];
	}
	delete[] field.mas;
	mas[field.y - 1] = new uint64_t[field.x];
	memset(mas[field.y - 1], 0, field.x * KCountBitOnByte);

	field.mas = mas;

	return field;
}

void MakeScattering(MainPointField field) {

	MainPointField save_field = field;
	bool is_stable;
	int16_t now_x;
	int16_t now_y;
	int32_t count_x_left;
	int32_t count_y_up;

	for (uint64_t count = 1; count <= field.max_iter; count++) {

		count_x_left = 0;
		count_y_up = 0;
		save_field.x = field.x;
		save_field.y = field.y;
		save_field.mas = new uint64_t*[field.y];

		for (int i = 0; i < field.y; i++) {
			save_field.mas[i] = new uint64_t[field.x];
			memcpy(save_field.mas[i], field.mas[i], field.x * KCountBitOnByte);
		}

		is_stable = true;
		now_x = field.x;
		now_y = field.y;

		for (int i = 0; i < (now_y + count_y_up); i++) {
			for (int j = 0; j < (now_x + count_x_left); j++) {
				if (save_field.mas[i][j] < KBorderCout) {
					continue;
				}
				is_stable = false;
				field.mas[i][j] -= KBorderCout;
				if (i == 0) {
					field = ExpansionTop(field);
					save_field = ExpansionTop(save_field);
					i++;
					count_y_up++;
				}
				field.mas[i - 1][j] += 1;
				if (i == (field.y - 1)) {
					field = ExpansionBottom(field);
					save_field = ExpansionBottom(save_field);
				}
				field.mas[i + 1][j] += 1;
				if (j == 0) {
					field = ExpansionLeft(field);
					save_field = ExpansionLeft(save_field);
					j++;
					count_x_left++;
				}
				field.mas[i][j - 1] += 1;
				if (j == (field.x - 1)) {
					field = ExpansionRight(field);
					save_field = ExpansionRight(save_field);
				}
				field.mas[i][j + 1] += 1;
			}
		}

		if (is_stable) {
			break;
		}

		if (field.frequency && !(count % field.frequency)) {
			CreateBmp(field, count);
		}

		SaveYourMemory(save_field);

	}

	if (!field.frequency) {
		CreateBmp(field, 0);
	}
}