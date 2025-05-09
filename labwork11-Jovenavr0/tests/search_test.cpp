#include <lib/revindex.h>
#include <lib/search.h>
#include <gtest/gtest.h>
#include <valarray>

TEST(Search, TestMakeRevIndex) {

	std::string dir_name("C:\\Users\\vvkar\\CLionProjects\\sem_2\\test");
	names naming = MakeRevIndex(dir_name);

	bool flag = true;
	std::string first_name("DID");
	std::string second_name("TERMS");
	std::string third_name("INDEXMAP");
	int count_files = 3;
	int sred = 5;
	int count_terms = 3;
	if (first_name != naming.files || second_name != naming.terms
	|| third_name != naming.index || count_files != naming.count
	|| sred != naming.avgdl || count_terms != naming.count_TID) {
		flag = false;
	}

	ASSERT_EQ(flag, 1);
}

TEST(Search, TestMethodOfWritingToFile) {

	bool flag = true;
	char block;
	uint8_t a;
	for (int i = 0; i < 256; ++i) {

		std::ofstream file1("1.txt", std::ios::binary);
		file1.put(static_cast<char>(i));
		file1.close();
		std::ifstream file2("1.txt", std::ios::binary);
		file2.get(block);
		a = static_cast<uint8_t>(block);
		if (i != a) {
			flag = false;
		}
		file2.close();

	}
	ASSERT_EQ(flag, true);
}

TEST(Search, TestMethodOfSaveUint64_t) {

	bool flag = true;
	char block;
	uint64_t example = rand() % std::numeric_limits<uint64_t>::max();
	uint64_t save = example;
	uint64_t value;
	std::ofstream file1("1.txt", std::ios::binary);
	for (int i = 0; i < 8; ++i) {
		file1.put(static_cast<char>(example % 256));
		example = (example >> 8);
	}
	file1.close();

	std::ifstream file2("1.txt", std::ios::binary);
	value = 0;
	for (int i = 0; i < 8; ++i) {
		file2.get(block);
		value += static_cast<uint64_t>(static_cast<uint8_t>(block) * std::pow(256, i));
	}
	file1.close();

	if (value != save) {
		flag = false;
	}

	ASSERT_EQ(flag, true);
}

TEST(Search, TestOfSearchWord1) {

	std::string dir_name("C:\\Users\\vvkar\\CLionProjects\\sem_2\\test");
	names naming = MakeRevIndex(dir_name);
	int64_t pointer = Search_world("1", naming.terms, naming.count_TID);
	ASSERT_EQ(pointer, 120);

}

TEST(Search, TestOfSearchWord2) {

	std::string dir_name("C:\\Users\\vvkar\\CLionProjects\\sem_2\\test");
	names naming = MakeRevIndex(dir_name);
	int64_t pointer = Search_world("2", naming.terms, naming.count_TID);
	ASSERT_EQ(pointer, 48);

}

TEST(Search, TestOfSearchWord3) {

	std::string dir_name("C:\\Users\\vvkar\\CLionProjects\\sem_2\\test");
	names naming = MakeRevIndex(dir_name);
	int64_t pointer = Search_world("3", naming.terms, naming.count_TID);
	ASSERT_EQ(pointer, 0);

}