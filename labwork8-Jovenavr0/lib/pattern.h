#pragma once
#include <memory>
#include <iostream>

template <typename T>
struct Node{
	Node<T>* parent;
	Node<T>* left;
	Node<T>* right;
	T value;
};

template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<Node<T>>>
class BinarySearchTree{
public:

	template <class Iter>
	class PostIterator {
	public:
		typedef Iter iterator_type;
		typedef std::input_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;

		iterator_type* value;
	private:
		explicit PostIterator(Iter* p): value(p) { }
	public:

		PostIterator(const PostIterator& other): value(other.value) { }

		bool operator !=(const PostIterator& other) {
			return this -> value != other.value;
		}

		bool operator ==(const PostIterator& other) {
			return this -> value == other.value;
		}

		typename PostIterator::reference operator*() const {
			return *value;
		}

		PostIterator& operator++ (){
			 if (value -> parent == nullptr) {
				 std::cerr << "invalid value";
				 exit(EXIT_FAILURE);
			 }
			 else if (value -> parent -> right == value) {
				value = value -> parent;
			} else {
				value = value -> parent;
				if (value -> right != nullptr) {
					value = value -> right;
					while (value -> left != nullptr || value -> right != nullptr) {
						if (value -> left != nullptr) {
							value = value -> left;
						} else {
							value = value -> right;
						}
					}
				}
			}
			return *this;
		}

		PostIterator operator++ (int){
			PostIterator copy {*this};
			++(*this);
			return copy;
		}

		PostIterator& operator-- (){
			if (value -> right != nullptr) {
				value = value -> right;
			} else if (value -> left != nullptr) {
				value = value -> left;
			} else {
				while (value -> parent -> left == nullptr && value -> parent -> right == value || value -> parent -> left == value) {
					value = value -> parent;
				}
				value = value -> parent -> left;
			}
			return *this;
		}

		PostIterator operator-- (int){
			PostIterator copy {*this};
			--(*this);
			return copy;
		}

	};

	template <class Iter>
	class InIterator {
	private:
		friend class BinarySearchTree;
	public:
		typedef Iter iterator_type;
		typedef std::input_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;

		iterator_type* value;
	private:
		explicit InIterator(Iter* p): value(p) { }
	public:

		InIterator(const InIterator& other): value(other.value) { }

		bool operator !=(const InIterator& other) {
			return this -> value != other.value;
		}

		bool operator ==(const InIterator& other) {
			return this -> value == other.value;
		}

		typename InIterator::reference operator*() const {
			return *value;
		}

		InIterator& operator++ (){
			if (value -> right != nullptr) {
				value = value -> right;
				while (value -> left != nullptr) {
					value = value -> left;
				}
			} else if (value -> parent -> left == value) {
				value = value -> parent;
			} else if (value -> parent -> right == value) {
				while (value -> parent -> left != value) {
					value = value -> parent;
				}
				value = value -> parent;
			}
			return *this;
		}

		InIterator operator++ (int){
			InIterator copy {*this};
			++(*this);
			return copy;
		}

		InIterator& operator-- (){
			if (value -> left != nullptr) {
				value = value -> left;
				while (value -> right != nullptr) {
					value = value -> right;
				}
			} else if (value -> parent -> right == value) {
				value = value -> parent;
			} else if (value -> parent -> left == value) {
				while (value -> parent -> right != value) {
					value = value -> parent;
				}
				value = value -> parent;
			}
			return *this;
		}

		InIterator operator-- (int){
			InIterator copy {*this};
			--(*this);
			return copy;
		}

	};

	template <class Iter>
	class PreIterator {
	private:
		friend class BinarySearchTree;
	public:
		typedef Iter iterator_type;
		typedef std::input_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;

		iterator_type* value;
	private:
		explicit PreIterator(Iter* p): value(p) { }
	public:

		PreIterator(const PreIterator& other): value(other.value) { }

		bool operator !=(const PreIterator& other) {
			return this -> value != other.value;
		}

		bool operator ==(const PreIterator& other) {
			return this -> value == other.value;
		}

		typename PreIterator::reference operator*() const {
			return *value;
		}

		PreIterator& operator++ (){
			if (value -> left != nullptr) {
				value = value -> left;
			} else if (value -> right != nullptr) {
				value = value -> right;
			} else if (value -> parent != nullptr){
				auto parent = value -> parent;

				while (parent != nullptr && (parent -> right == nullptr || parent -> right == value)) {
					value = parent;
					parent = value -> parent;
				}

				if (parent != nullptr) {
					value = parent -> right;
				} else {
					value = nullptr;
				}

			} else {
				value = nullptr;
			}
			return *this;
		}

		PreIterator operator++ (int){
			PreIterator copy {*this};
			++(*this);
			return copy;
		}

		PreIterator& operator-- (){
			if (value -> parent == nullptr) {
				std::cerr << "invalid value";
				exit(EXIT_FAILURE);
			}
			else if (value -> parent -> left == value) {
				value = value -> parent;
			} else {
				value = value -> parent;
				if (value -> left != nullptr) {
					value = value -> left;
					while (value -> right != nullptr || value -> left != nullptr) {
						if (value -> right != nullptr) {
							value = value -> right;
						} else {
							value = value -> left;
						}
					}
				}
			}
			return *this;
		}

		PreIterator operator-- (int){
			PreIterator copy {*this};
			--(*this);
			return copy;
		}

	};

	template <class Iter>
	class ReverseIterator {
	private:
		friend class BinarySearchTree;
	public:
		typedef Iter iterator_type;
		typedef std::input_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;

		iterator_type* value;
	private:
		explicit ReverseIterator(Iter* p): value(p) { }
	public:

		ReverseIterator(const ReverseIterator& other): value(other.value) { }

		bool operator !=(const ReverseIterator& other) {
			return this -> value != other.value;
		}

		bool operator ==(const ReverseIterator& other) {
			return this -> value == other.value;
		}

		typename ReverseIterator::reference operator*() const {
			return *value;
		}

		ReverseIterator& operator++ (){
			Iter temp(value);
			return --temp;
		}

		ReverseIterator operator++ (int){
			ReverseIterator copy {*this};
			++(*this);
			return copy;
		}

		ReverseIterator& operator-- (){
			Iter temp(value);
			return --temp;
		}

		ReverseIterator operator-- (int){
			ReverseIterator copy {*this};
			--(*this);
			return copy;
		}


	};

	typedef T value_type;
	Node<value_type>* root;
	Alloc alloc;
	uint64_t count_element;
	typedef InIterator<Node<value_type >> Initerator;
	typedef InIterator<const Node<value_type >> const_Initerator;
	typedef ReverseIterator<InIterator<Node<value_type >>> reverse_Initerator;
	typedef ReverseIterator<const InIterator<const Node<value_type >>> reverse_const_Initerator;
	typedef PreIterator<Node<value_type >> Preiterator;
	typedef PreIterator<const Node<value_type >> const_Preiterator;
	typedef ReverseIterator<PreIterator<Node<value_type >>> reverse_Preiterator;
	typedef ReverseIterator<const PreIterator<const Node<value_type >>> reverse_const_Preiterator;
	typedef PostIterator<Node<value_type >> Postiterator;
	typedef PostIterator<const Node<value_type >> const_Postiterator;
	typedef ReverseIterator<PostIterator<Node<value_type >>> reverse_Postiterator;
	typedef ReverseIterator<const PostIterator<const Node<value_type >>> reverse_const_Postiterator;

	BinarySearchTree (const Compare& compare = Compare{}) {
		this -> root = nullptr;
		this -> count_element = 0;
		this -> comp_ = compare;
	}

	BinarySearchTree (const BinarySearchTree& other, const Compare& compare = Compare{}) {
		this -> root = CopyNode(other.root);
		this -> count_element = other.count_element;
		this -> comp_ = compare;
	}

	BinarySearchTree (auto& i, auto& j, const Compare& compare = Compare{}) {
		this -> root = nullptr;
		this -> count_element = 0;
		this -> comp_ = compare;
		for (i; i != j; ++i) {
			this -> insert((*i).value);
		}

	}

	BinarySearchTree (const auto& il, const Compare& compare = Compare{}) {
		this -> root = nullptr;
		this -> count_element = 0;
		this -> comp_ = compare;
		for (auto el = il.begin(); el != il.end(); ++el) {
			this -> insert(*el);
		}
	}

	~BinarySearchTree () {
		clear();
	}

	size_t max_size() {
		return std::allocator_traits{alloc}.max_size();
	}

	Compare value_comp () const {
		return comp_;
	}

	void PostOrder(){
		auto it = this -> Postbegin();
		for (int i = 0; i < (this -> count_element); ++i){
			std::cout << (*(it)).value << ' ';
			if (i + 1 != this -> count_element) {
				++it;
			}
		}
	}

	void reversePostOrder(){
		auto a = this -> Postend();

		for (int i = 0; i < this -> count_element; i++){
			std::cout << (*(a)).value << ' ';
			if (a != this -> Postbegin()){
				--a;
			}
		}
	}

	void InOrder(){
		auto it = this -> Inbegin();

		for (int i = 0; i < this -> count_element; i++){
			std::cout << (*it).value << ' ';
			if (it != this -> Inend()) {
				++it;
			}
		}
	}

	void reverseInOrder(){
		auto it = this -> Inend();

		for (int i = 0; i < this -> count_element; i++) {
			std::cout << (*it).value << ' ';
			if (it != this -> Inbegin()) {
				--it;
			}
		}
	}

	void PreOrder(){
		auto it = this -> Prebegin();

		for (int i = 0; i < this -> count_element; ++i){
			std::cout << (*it).value << ' ';
			if (i + 1 != this -> count_element) {
				++it;
			}
		}
	}

	void reversePreOrder(){
		auto it = this -> Preend();
		for (int i = 0; i < this -> count_element; ++i) {
			std::cout << (*it).value << ' ';
			if (it != this -> Prebegin()) {
				--it;
			}
		}
	}

	Node<value_type >* create(value_type key) {
		Node<value_type>* element = alloc.allocate(1);
		element -> value = key;
		element -> parent = nullptr;
		element -> right = nullptr;
		element -> left = nullptr;
		return element;
	}

	void insert (value_type key) {
		Node<value_type >* element = create(key);
		if (root == nullptr) {
			root = element;
			++count_element;
			return;
		}
		Node<value_type>* now_el = root;
		while (now_el != nullptr) {
			if (comp_(now_el -> value, element -> value)) {
				if (now_el -> right != nullptr) {
					now_el = now_el -> right;
				} else {
					element -> parent = now_el;
					now_el -> right = element;
					++count_element;
					break;
				}
			} else if (comp_(element -> value, now_el -> value)) {
				if (now_el -> left != nullptr) {
					now_el = now_el -> left;
				} else {
					element -> parent = now_el;
					now_el -> left = element;
					++count_element;
					break;
				}
			} else {
				break;
			}
		}
	}

	std::pair <Node<value_type>*, bool> insert (value_type key, int32_t, int32_t) {
		Node<value_type >* element = create(key);
		std::pair <Node<value_type >*, bool> answer;
		answer.first = nullptr;
		answer.second = false;
		if (root == nullptr) {
			root = element;
			answer.first = element;
			answer.second = true;
			++count_element;
			return answer;
		}
		Node<value_type >* now_el = root;
		while (now_el != nullptr) {
			if (comp_(now_el -> value, element -> value)) {
				if (now_el -> right != nullptr) {
					now_el = now_el -> right;
				} else {
					element -> parent = now_el;
					now_el -> right = element;
					answer.first = element;
					answer.second = true;
					++count_element;
					break;
				}
			} else if (comp_(element -> value, now_el -> value)) {
				if (now_el -> left != nullptr) {
					now_el = now_el -> left;
				} else {
					element -> parent = now_el;
					now_el -> left = element;
					answer.first = element;
					answer.second = true;
					++count_element;
					break;
				}
			} else {
				break;
			}
		}

		return answer;
	}

	void insert (value_type start, value_type end) {
		for (int i = start; i < end; ++i) {
			this -> insert(i);
		}
	}

	void insert (const auto& il) {
		for (auto el = il.begin(); el != il.end(); ++el) {
			this -> insert(*el);
		}
	}

	Node<value_type >* max (Node<value_type >* element){
		if (element == nullptr) {
			return nullptr;
		}

		if (element -> right == nullptr) {
			return element;
		}

		return max(element->right);
	}

	Node<value_type >* min (Node<value_type>* element){
		if (element == nullptr) {
			return nullptr;
		}

		if (element -> left == nullptr) {
			return element;
		}

		return min(element -> left);
	}

	bool contains (value_type key) {
		bool answer = false;
		Node<value_type >* x = root;
		while (x != nullptr) {
			if (comp_(key, x -> value)) {
				x = x -> left;
			} else if (comp_(x -> value, key)) {
				x = x -> right;
			} else if (x -> value == key) {
				answer = true;
				break;
			}
		}

		return answer;
	}

	Node<value_type >* find (value_type key) {
		Node<value_type>* x = root;
		while (x != nullptr) {
			if (comp_(key, x -> value)) {
				x = x -> left;
			} else if (comp_(x -> value, key)) {
				x = x -> right;
			} else if (x -> value == key) {
				return x;
			}
		}

		return nullptr;
	}

	Node<value_type >* remove (Node<value_type >* node, value_type key) {
		if (node == nullptr) {
			return node;
		}
		if (comp_(node -> value, key)) {
			node -> left = remove(node -> left, key);
		} else if (comp_(key, node -> value)) {
			node -> right = remove(node -> right, key);
		} else if (node -> left != nullptr && node -> right != nullptr) {
			node -> value = min(node -> right) -> value;
			node -> right = remove(node -> right, node -> value);
		} else {
			if (node -> left != nullptr) {
				node = node -> left;
			} else if (node -> right != nullptr) {
				node = node -> right;
			} else {
				node = nullptr;
			}
			--count_element;
		}

		return node;
	}

	Node<value_type >* extract(value_type key) {
		Node<value_type >* answer;
		Node<value_type >* find = find(key);
		if (find != nullptr) {
			answer = find;
			root = remove(root, key);
		} else {
			answer -> value = value_type{};
		}
		return answer;
	}

	Node<value_type >* extract(Node<value_type >* temp) {
		return extract(temp -> value);
	}

	Node<value_type >* lover_bound(value_type key) {
		return prev(key);
	}

	Node<value_type >* upper_bound(value_type key) {
		return next(key);
	}

	std::pair<Node<value_type >*, Node<value_type >*> equal_range(value_type key) {
		std::pair<Node<value_type >*, Node<value_type>*> answer;
		answer.first = lover_bound(key);
		answer.second = upper_bound(key);

		return answer;
	}

	long long size () {
		return this -> count_element;
	}

	bool empty () {
		if (this -> root == nullptr){
			return true;
		} else {
			return false;
		}
	}

	BinarySearchTree& operator=(const BinarySearchTree& rhs) {
		if (*this != rhs) {
			this -> root = this -> deleteTree(this -> root);
			this -> count_element = rhs.count_element;
			root = CopyNode(rhs.root);
		}

		return *this;
	}

	BinarySearchTree& operator=(const auto& il) {
		this -> clear();
		for (auto el = il.begin(); el != il.end(); ++el) {
			this -> insert(*el);
		}

		return *this;
	}

	Node<value_type >* CopyNode (Node<value_type >* element) {
		if (element == nullptr) {
			return nullptr;
		}
		Node<value_type >* node = create(element -> value);
		node -> parent = element -> parent;
		node -> left = CopyNode(element -> left);
		node -> right = CopyNode(element -> right);

		return node;
	}

	bool operator== (const BinarySearchTree& rhs) {
		if ((this -> count_element == rhs.count_element) && IsIdentically(this->root, rhs.root)) {
			return true;
		} else {
			return false;
		}
	}

	bool IsIdentically (const Node<value_type>* lhs, const Node<value_type >* rhs){
		if (lhs == nullptr || rhs == nullptr) {
			if (lhs == nullptr && rhs == nullptr) {
				return true;
			} else {
				return false;
			}
 		}

		if (lhs -> value == rhs -> value) {
			return IsIdentically(lhs->left, rhs->left) && (IsIdentically(lhs->right, rhs->right));
		} else {
			return false;
		}

	}

	bool operator!= (const BinarySearchTree& rhs) {
		return !operator==(rhs);
	}

	Node<value_type >* next (value_type key){
		Node<value_type >* current = root, * successor = nullptr;

		while (current != nullptr) {
			if (comp_(key, current -> value)) {
				successor = current;
				current = current -> left;
			} else {
				current = current -> right;
			}
		}

		return successor;
	}

	Node<value_type >* prev (value_type key) {
		Node<value_type>* current = root;
		Node<value_type>* successor = nullptr;

		while (current != nullptr) {
			if (!comp_(current -> value, key)) {
				current = current -> left;
			} else {
				successor = current;
				current = current -> right;
			}
		}

		return successor;
	}

	void clear () {
		this -> root = deleteTree(this -> root);
		count_element = 0;
	}

	Node<value_type >* deleteTree (Node<value_type >* node) {
		if (node == nullptr) {
			return nullptr;
		}
		deleteTree(node -> left);
		deleteTree(node -> right);
		alloc.deallocate(node, 1);

		return nullptr;
	}

	Alloc get_allocator(){
		return alloc;
	}

	void swap (BinarySearchTree& lhs) {
		BinarySearchTree<value_type > temp;
		temp = *this;
		*this = lhs;
		lhs = temp;
	}

	static void swap (BinarySearchTree& rhs, BinarySearchTree& lhs) {
		rhs.swap(lhs);
	}

	void merge (const BinarySearchTree& rhs) {
		circumvent(rhs.root);
	}

	void circumvent (Node<value_type >* node) {
		if (node == nullptr) {
			return;
		}
		insert(node -> value);
		circumvent(node -> left);
		circumvent(node -> right);
	}

	int32_t count (value_type key) {
		if (contains(key)) {
			return 1;
		} else {
			return 0;
		}
	}

	Node<value_type >* find_small_leaf(Node<value_type >* node){
		while (node -> left != nullptr || node -> right != nullptr) {
			if (node -> left != nullptr) {
				node = node -> left;
			} else {
				node = node -> right;
			}
		}

		return node;
	}

	reverse_Initerator ReverseInbegin () {
		return reverse_Initerator(Inend());
	}

	reverse_Initerator ReverseInend () {
		return reverse_Initerator(Inbegin());
	}

	reverse_const_Initerator ReverseIncbegin () {
		return reverse_const_Initerator(Incend());
	}

	reverse_const_Initerator ReverseIncend () {
		return reverse_const_Initerator(Incbegin ());
	}

	reverse_Preiterator ReversePrebegin () {
		return reverse_Preiterator(Preend());
	}

	reverse_Preiterator ReversePreend () {
		return reverse_Preiterator(Prebegin());
	}

	reverse_const_Preiterator ReversePrecbegin () {
		return reverse_const_Preiterator(Precend());
	}

	reverse_const_Preiterator ReversePrecend () {
		return reverse_const_Preiterator(Precbegin ());
	}

	reverse_Postiterator ReversePostbegin () {
		return reverse_Postiterator(Postend());
	}

	reverse_Postiterator ReversePostend () {
		return reverse_Postiterator(Postbegin());
	}

	reverse_const_Postiterator ReversePostcbegin () {
		return reverse_const_Postiterator(Postcend());
	}

	reverse_const_Postiterator ReversePostcend () {
		return reverse_const_Postiterator(Postcbegin ());
	}

	Initerator Inbegin () {
		Node<value_type>* node = min(root);
		return Initerator(node);
	}

	Initerator Inend () {
		Node<value_type >* node = max(root);
		return Initerator(node);
	}

	const_Initerator Incbegin () const {
		Node<value_type >* node = min(root);
		return const_Initerator(node);
	}

	const_Initerator Incend () const {
		Node<value_type >* node = max(root);
		return const_Initerator(node);
	}

	Preiterator Prebegin () {
		return Preiterator(root);
	}

	Preiterator Preend () {
		Node<value_type >* node = max(root);
		while (node -> left != nullptr) {
			node = max(node -> left);
		}
		return PreIterator(node);
	}

	const_Preiterator Precbegin () const {
		return const_Preiterator(root);
	}

	const_Preiterator Precend () const {
		Node<value_type >* node = max(root);
		while (node -> left != nullptr) {
			node = max(node -> left);
		}
		return const_PreIterator(node);
	}

	Postiterator Postbegin () {
		Node<value_type >* node = find_small_leaf(root);
		return Postiterator(node);
	}

	Postiterator Postend () {
		return Postiterator(root);
	}

	const_Postiterator Postcbegin () const {
		Node<value_type>* node = min(root);
		return const_Postiterator(node);
	}

	const_Postiterator Postcend () const {
		return const_Postiterator(root);
	}

	friend std::ostream& operator<<(std::ostream& os, const Node<value_type >& n){
		return os << n.value;
	}

private:
	Compare comp_;

};