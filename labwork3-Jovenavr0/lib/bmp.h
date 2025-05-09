#pragma once
#include "parser.h"

#pragma pack(1)
struct BmpFileHeader {
	uint16_t file_type = 0x4D42;
	uint32_t file_size;
	uint16_t file_reserved1 = 0;
	uint16_t file_reserved2 = 0;
	uint32_t start_bit;
};


struct BmpFileInfoHeader {
	uint32_t size_info_header;
	uint32_t width_size;
	uint32_t height_size;
	uint16_t count_planes;
	uint16_t count_bit_pixel;
	uint32_t type_compression = 0;
	uint32_t bit_size_image;
	uint32_t bit_xpels_per_meter = 0;
	uint32_t bit_ypels_per_meter = 0;
	uint32_t count_color;
	uint32_t count_important_color = 0;
};
#pragma pack()

struct RGB {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
};

void CreateBmp(const MainPointField&, uint64_t now_iter);
