#include <lib/adapters.h>
#include <gtest/gtest.h>
#include <list>

TEST(Adapters, Take) {
	std::vector<int> vec;
	for (int i = 0; i < 1000000; ++i) {
		vec.push_back(rand() % 1000);
	}
	int count = rand() % 100;
	bool flag = true;
	int i = 0;
	for (auto& el: vec | adapters::take(count)) {
		if (el != vec[i++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Drop) {
	std::vector<int> vec;
	for (int i = 0; i < 1000000; ++i) {
		vec.push_back(rand() % 1000);
	}
	int count = rand() % 100;
	bool flag = true;
	int i = count;
	for (auto& el: vec | adapters::drop(count)) {
		if (el != vec[i++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Transform) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.push_back(a + 1);
	}
	int index = 0;
	for (auto& el: vec | adapters::transformation([](int elem) {return elem + 1;})) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Filter) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		if (a % 2 == 0) {
			check.push_back(a);
		}
	}
	int index = 0;
	for (auto& el: vec | adapters::filter([](int elem) {return elem % 2 == 0; })) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Reverse) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 100000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.insert(check.begin(), a);
	}
	int index = 0;
	for (auto& el: vec | adapters::reverse() ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Keys) {
	std::map<int, int> cont;
	std::vector<int> keys;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		int b = rand() % 1000;
		if (!cont.contains(a)) {
			cont.insert(std::pair<int, int>(a, b));
			keys.push_back(a);
		}
	}
	std::sort(keys.begin(), keys.end());
	bool flag = true;
	int index = 0;
	for (auto& el: cont | adapters::keys()) {
		if (el != keys[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Values) {
	std::map<int, int> cont;
	std::vector<int> values;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		int b = rand() % 1000;
		cont.insert(std::pair<int, int>(a, b));
	}
	for (auto& el: cont) {
		values.push_back(el.second);
	}
	bool flag = true;
	int index = 0;
	for (auto& el: cont | adapters::values()) {
		if (el != values[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Filter_Drop) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		if (a % 2 == 0) {
			check.push_back(a);
		}
	}
	int count = rand() % 1000;
	int index = count;
	for (auto& el: vec | adapters::filter([](int elem){return elem % 2 == 0; }) | adapters::drop(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Transform_Drop) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.push_back(a * a);
	}
	int count = rand() % 1000;
	int index = count;
	for (auto& el: vec | adapters::transformation([](int elem){return elem * elem; }) | adapters::drop(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Filter_Take) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		if (a % 2 == 0) {
			check.push_back(a);
		}
	}
	int count = rand() % 1000;
	int index = 0;
	for (auto& el: vec | adapters::filter([](int elem){return elem % 2 == 0; }) | adapters::take(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Transform_Take) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 1000000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.push_back(a * a);
	}
	int count = rand() % 1000;
	int index = 0;
	for (auto& el: vec | adapters::transformation([](int elem){return elem * elem; }) | adapters::take(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Reverse_Drop) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 100000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.insert(check.begin(), a);
	}
	int count = rand() % 1000;
	int index = count;
	for (auto& el: vec | adapters::reverse() | adapters::drop(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Reverse_Take) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 100000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.insert(check.begin(), a);
	}
	int count = rand() % 1000;
	int index = 0;
	for (auto& el: vec | adapters::reverse() | adapters::take(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Drop_Take) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 100000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.push_back(a);
	}
	int count = rand() % 1000;
	int index = count;
	for (auto& el: vec | adapters::drop(count) | adapters::take(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, Take_Drop) {
	std::vector<int> vec;
	std::vector<int> check;
	bool flag = true;
	for (int i = 0; i < 100000; ++i) {
		int a = rand() % 1000;
		vec.push_back(a);
		check.push_back(a);
	}
	int count = rand() % 1000;
	int index = 0;
	for (auto& el: vec | adapters::take(count) | adapters::drop(count) ) {
		if (el != check[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, NotVector_Take) {
	std::list<int> a;
	std::vector<int> value;
	for (int i = 0; i < 100000; ++i) {
		int elem = rand() % 10000;
		a.push_back(elem);
		value.push_back(elem);
	}
	bool flag = true;
	int count = rand() % 100;
	int index = 0;
	for (auto& el: a | adapters::take(count)) {
		if (el != value[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, NotVector_Drop) {
	std::list<int> a;
	std::vector<int> value;
	for (int i = 0; i < 100000; ++i) {
		int elem = rand() % 10000;
		a.push_back(elem);
		value.push_back(elem);
	}
	bool flag = true;
	int count = rand() % 100;
	int index = count;
	for (auto& el: a | adapters::drop(count)) {
		if (el != value[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, NotVector_Reverse) {
	std::list<int> a;
	std::vector<int> value;
	for (int i = 0; i < 100000; ++i) {
		int elem = rand() % 10000;
		a.push_back(elem);
		value.insert(value.begin(), elem);
	}
	bool flag = true;
	int index = 0;
	for (auto& el: a | adapters::reverse()) {
		if (el != value[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, NotVector_Filter) {
	std::list<int> a;
	std::vector<int> value;
	for (int i = 0; i < 100000; ++i) {
		int elem = rand() % 10000;
		a.push_back(elem);
		if (elem % 2 == 0) {
			value.push_back(elem);
		}

	}
	bool flag = true;
	int index = 0;
	for (auto& el: a | adapters::filter([](int elem) {return elem % 2 == 0;})) {
		if (el != value[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}

TEST(Adapters, NotVector_Transform) {
	std::list<int> a;
	std::vector<int> value;
	for (int i = 0; i < 100000; ++i) {
		int elem = rand() % 10000;
		a.push_back(elem);
		value.push_back(elem * elem);
	}
	bool flag = true;
	int index = 0;
	for (auto& el: a | adapters::transformation([](int elem) {return elem * elem;})) {
		if (el != value[index++]) {
			flag = false;
			break;
		}
	}
	ASSERT_EQ(flag, true);
}