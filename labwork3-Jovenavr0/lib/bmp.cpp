#include "bmp.h"
#include <cstring>


void CreateBmp(const MainPointField& field, uint64_t now_iter) {
	BmpFileHeader header;
	BmpFileInfoHeader info_header;

	RGB colors[16];

	char color[] = {0, 1, 2, 3, 4};

	char color_first;
	char color_second;
	int count_dop_byte;
	unsigned char now_color_in_byte;
	char* now_name;
	char name_value[19];

	// fill header
	header.start_bit = sizeof(header) + sizeof(info_header) + sizeof(colors);
	header.file_size = header.start_bit + field.x * field.y +
					   field.y * (4 - (((field.x + (field.x % 2))/ 2) % 4)) % 4;

	// fill info_header
	info_header.size_info_header = sizeof(info_header);
	info_header.width_size = field.x;
	info_header.height_size = field.y;
	info_header.count_planes = 1;
	info_header.count_bit_pixel = 4;
	info_header.bit_size_image = field.x * field.y;
	info_header.count_color = 16;

	// create now filename
	itoa(now_iter, name_value, 10);
	now_name = new char(strlen(field.output_directory) + std::strlen(name_value) + 1);
	strcpy(now_name, name_value);
	strcat(now_name, field.output_directory);

	FILE* file = fopen(now_name, "wb");

	// write header and info_header
	fwrite(&header,1,sizeof(header), file);
	fwrite(&info_header, 1, sizeof(info_header), file);

	// create colors
	memset (&colors[0], 255, sizeof(RGB));
	memset (&colors[1], 0, sizeof(RGB));
	colors[1].green = 255;
	memset (&colors[2], 255, sizeof(RGB));
	colors[2].blue = 0;
	colors[2].alpha = 0;
	memset (&colors[3], 255, sizeof(RGB));
	colors[3].red = 134;
	colors[3].green = 1;
	colors[3].blue = 175;
	memset (&colors[4], 0, sizeof(RGB));

	// write palette
	fwrite(&colors, 1,sizeof(colors), file);

	//calculate dop_byte for count mod 4 = 0
	count_dop_byte = (4 - (((field.x + (field.x % 2))/ 2) % 4)) % 4;

	// write picture
	for (int i = (field.y - 1); i >= 0; i--) {
		for (int j = 0; j < field.x; j+=2) {
			if (field.mas[i][j] <= 3) {
				color_first = color[field.mas[i][j]];
			}
			else {
				color_first = color[4];
			}
			if (field.mas[i][j + 1] <= 3) {
				color_second = color[field.mas[i][j + 1]];
			}
			else {
				color_second = color[4];
			}
			now_color_in_byte = (color_first << 4) + color_second;
			fwrite(&now_color_in_byte, 1, sizeof(now_color_in_byte), file);
		}
		// padding the width to a multiple of 4
		now_color_in_byte = (color[4] << 4) + color[4];
		fwrite(&now_color_in_byte, 1, sizeof(now_color_in_byte) * count_dop_byte, file);
	}
	fclose(file);
	delete now_name;
}