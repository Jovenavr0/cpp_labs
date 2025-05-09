#include <lib/pattern.h>
#include <gtest/gtest.h>
#include <algorithm>

int32_t ninf = std::numeric_limits<int32_t>::min();
int32_t inf = std::numeric_limits<int32_t>::max();

void RecPreOrder (Node<int32_t>* node, std::vector<int32_t>& vec) {
	if (node != nullptr) {
		vec.push_back(node -> value);
		RecPreOrder(node -> left, vec);
		RecPreOrder(node -> right, vec);
	}
}

void RecPostOrder (Node<int32_t>* node, std::vector<int32_t>& first) {
	if (node != nullptr) {
		RecPostOrder(node -> left, first);
		RecPostOrder(node -> right, first);
		first.push_back(node -> value);
	}
}

void RecInOrder (Node<int32_t>* node, std::vector<int32_t>& first) {
	if (node != nullptr) {
		RecInOrder(node -> left, first);
		first.push_back(node -> value);
		RecInOrder(node -> right, first);
	}
}

bool check(Node<int32_t>* node, int32_t min, int32_t max) {
	if (node == nullptr) {
		return true;
	}
	if (node->value <= min || max <= node->value) {
		return false;
	}
	return check(node->left, min, node->value) && check(node->right, node->value, max);
}

bool isBinarySearchTree(Node<int32_t>* temp) {
	return check(temp, ninf, inf);
}

TEST(BST, IsTree){
	int a;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 10; i++) {
		a = (rand() % 100);
		tree.insert(a);
	}
	ASSERT_EQ(isBinarySearchTree(tree.root), 1);
}

TEST(BST, MaxTest){
	int a;
	int ma = std::numeric_limits<int>::min();
	BinarySearchTree<int> tree;
	for (int i = 0; i < 100; i++) {
		a = rand() % 1000000;
		ma = std::max(a, ma);
		tree.insert(a);
	}
	ASSERT_EQ(tree.max(tree.root) -> value, ma);
}

TEST(BST, MinTest){
	int a;
	int mi = std::numeric_limits<int>::max();
	BinarySearchTree<int> tree;
	for (int i = 0; i < 100; i++) {
		a = rand() % 1000000;
		mi = std::min(a, mi);
		tree.insert(a);
	}
	ASSERT_EQ(tree.min(tree.root) -> value, mi);
}

TEST(BST, InOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = rand() % 1000000;
		tree.insert(a);
	}
	RecInOrder(tree.root, first);
	auto it = tree.Inbegin();

	for (int i = 0; i < tree.count_element; i++){
		second.push_back((*it).value);
		if (it != tree.Inend()) {
			++it;
		}
	}
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, ReverseInOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = (rand() % 1000000);
		tree.insert(a);
	}
	RecInOrder(tree.root, first);
	auto it = tree.Inend();
	for (int i = 0; i < tree.count_element; i++) {
		second.push_back((*it).value);
		if (it != tree.Inbegin()) {
			--it;
		}
	}
	std::reverse(first.begin(), first.end());
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, PostOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = rand() % 1000000;
		tree.insert(a);
	}
	RecPostOrder(tree.root, first);
	auto it = tree.Postbegin();
	for (int i = 0; i < (tree.count_element); ++i){
		second.push_back((*it).value);
		if (i + 1 != tree.count_element) {
			++it;
		}
	}
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, ReversePostOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = rand() % 1000000;
		tree.insert(a);
	}
	RecPostOrder(tree.root, first);
	auto it = tree.Postend();
	for (int i = 0; i < tree.count_element; i++){
		second.push_back((*it).value);
		if (it != tree.Postbegin()){
			--it;
		}
	}
	std::reverse(first.begin(), first.end());
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, PreOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = rand() % 1000000;
		tree.insert(a);
	}
	RecPreOrder(tree.root, first);
	auto it = tree.Prebegin();
	for (int i = 0; i < tree.count_element; ++i){
		second.push_back((*it).value);
		if (i + 1 != tree.count_element) {
			++it;
		}
	}
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, ReversePreOrder){
	int a;
	std::vector<int> first;
	std::vector<int> second;
	bool answer = true;
	BinarySearchTree<int> tree;
	for (int i = 0; i < 1000000; i++) {
		a = rand() % 1000000;
		tree.insert(a);
	}
	RecPreOrder(tree.root, first);
	auto it = tree.Preend();
	for (int i = 0; i < tree.count_element; ++i) {
		second.push_back((*it).value);
		if (it != tree.Prebegin()) {
			--it;
		}
	}
	std::reverse(first.begin(), first.end());
	for (int i = 0; i < second.size(); i++) {
		if (second[i] != first[i]) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, InsertWork){
	BinarySearchTree<int> tree;
	int start = (rand() % 1000000);
	int answer = true;
	tree.insert(start);
	int checker = (rand() % 1000000);
	tree.insert(checker);
	if (tree.root -> value > checker) {
		if (tree.root -> left -> value != checker) {
			answer = false;
		}
	} else if (tree.root -> value < checker) {
		if (tree.root -> right -> value != checker) {
			answer = false;
		}
	} else {
		if (tree.root -> right != nullptr || tree.root -> left != nullptr) {
			answer = false;
		}
	}
	ASSERT_EQ(answer, 1);
}

TEST(BST, CountElem){
	long long count = (rand() % 10000000000000000);
	BinarySearchTree<int> tree;
	int a;
	for (int i = 0; i < count; ++i) {
		a = rand() % 10000000;
		tree.insert(a);
	}
	ASSERT_EQ(tree.size(), count);
}

TEST(BST, Clear) {
	long long count = (rand() % 10000000000000000);
	BinarySearchTree<int> tree;
	int a;
	for (int i = 0; i < count; ++i) {
		a = rand() % 10000000;
		tree.insert(a);
	}
	tree.clear();
	ASSERT_EQ(tree.empty(), 1);
}

TEST(BST, Identically){
	long long count = (rand() % 10000000000000000);
	BinarySearchTree<int> first_tree;
	BinarySearchTree<int> second_tree;
	int a;
	for (int i = 0; i < count; ++i) {
		a = rand() % 10000000;
		first_tree.insert(a);
		second_tree.insert(a);
	}
	ASSERT_EQ(first_tree == second_tree, 1);
}

TEST(BST, Assignment){
	long long count = (rand() % 10000000000000000);
	BinarySearchTree<int> first_tree;
	BinarySearchTree<int> second_tree;
	int a;
	for (int i = 0; i < count; ++i) {
		a = rand() % 10000000;
		first_tree.insert(a);
	}
	second_tree = first_tree;
	ASSERT_EQ(first_tree == second_tree, 1);
}

TEST(BST, Prev){
	BinarySearchTree<int> tree;
	int a = rand() % 10000000;
	tree.insert(a);
	tree.insert(a - 1);
	ASSERT_EQ(a - 1 == tree.prev(a) -> value, 1);
}

TEST(BST, Next){
	BinarySearchTree<int> tree;
	int a = rand() % 10000000;
	tree.insert(a);
	tree.insert(a + 1);
	ASSERT_EQ(a + 1 == tree.next(a) -> value, 1);
}