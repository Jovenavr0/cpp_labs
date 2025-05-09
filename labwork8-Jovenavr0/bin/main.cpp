#include <lib/pattern.h>
#include <vector>

int main(){

	std::vector<int> a = {10, 5, 3, 6, 12, 15, 17};

	BinarySearchTree<int> ex;

	std::cout << ex.empty();

//
//	ex.insert(1);
//
//
//	ex = a;
//
//	ex.PreOrder();
//
//	std::cout << std::endl;
//
//
//
//	std::cout << std::endl;

//	BinarySearchTree<int> first;
//	BinarySearchTree<int> second;
//
//	first.insert(4);
//	second.insert(3);
//
//	first.insert(8);
//	first.insert(7);
//	first.insert(9);
//
//	first.root = first.remove(first.root, 8);
//
//	swap(first, second);
//
//	std::cout << first.root -> value;
//	std::cout << second.root -> value;
//
//
//	std::cout << first.root -> value;
//	std::cout << second.root -> value;
//	std::cout << second.root -> right -> value;
//	std::cout << second.root -> right -> left -> value;
//
//	std::vector <int> a;
//
//	a.push_back(1);
//	a.push_back(2);
//	a.push_back(3);
//
//	std::cout << *(a.begin()++) << '\n';
//
//	std::cout << a.max_size() << '\n';
//
	BinarySearchTree<int> first;

	first.insert(10);
	first.insert(5);
	first.insert(15);
	first.insert(3);
	first.insert(6);
	first.insert(12);
	first.insert(17);


//	auto si = first.Prebegin();
//	auto fi = first.Preend();
//	const std::less<int> compare;

//	BinarySearchTree<int> example(si, fi);

//	example.PostOrder();

	std::cout << std::endl;


	std::cout << "\n";

	BinarySearchTree<int> second;

	second = first;

	std::cout << (second == first);

	BinarySearchTree<int> third;

	second.insert(1);

//	first.PostOrder();
//
//	std::cout << std::endl;
//
//	first.reversePostOrder();

	std::cout << std::endl;

	first.InOrder();

	std::cout << std::endl;

	first.reverseInOrder();

	std::cout << std::endl;

	first.PreOrder();

	std::cout << std::endl;

//	first.reversePreOrder();

	std::cout << std::endl;

//	std::cout << second.begin();

//
//	std::cout << second.root;
//
//	third.insert(1);
//
//	second.merge(third);
//	std::cout << third.count(1);

//	second.insert(3);
//
//	BinarySearchTree<int>::swap(first, second);

	return EXIT_SUCCESS;
}