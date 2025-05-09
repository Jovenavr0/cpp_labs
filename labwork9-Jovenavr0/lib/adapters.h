#pragma once
#include <cstdint>
#include <vector>

namespace adapters{

	template <typename, typename = void>
	constexpr bool is_valid_container{};

	template <typename T>
	constexpr bool is_valid_container<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> = true;

	template<typename T, typename Adapter>
	auto operator | (T container, Adapter adapt) {
		static_assert(is_valid_container<T>, "This container has no pointers to begin and/or end");
		return adapt(container);
	}

	template <typename Func>
	class TransformUnion{
	public:
		TransformUnion (Func lambda) {
			_function = lambda;
		}

		auto operator() (auto& container) {
			auto t(container);
			for (auto& el: t) {
				el = _function(el);
			}
			return t;
		}

	private:
		Func _function;
	};

	template<typename Func>
	TransformUnion<Func> transformation(Func function) {
		return TransformUnion(function);
	}

	class TakeUnion{
	public:
		TakeUnion (uint32_t n) {
			_count = n;
		}

		template<typename T>
		auto operator() (T& container) {
			int counter = 0;
			T take_container;
			for (auto& el: container) {
				take_container.insert(take_container.end(), el);
				counter++;
				if (counter == _count) {
					break;
				}
			}
			return take_container;
		}

	private:
		uint32_t _count;
	};

	auto take (uint32_t n) {
		return TakeUnion(n);
	}

	class DropUnion{
	public:
		DropUnion (uint32_t n) {
			_count = n;
		}

		template<typename T>
		T operator() (T& container) {
			T drop_container;
			int counter = 0;
			for (auto& el: container) {
				counter++;
				if (counter > _count) {
					drop_container.insert(drop_container.end(), el);
				}
			}
			return drop_container;
		}

	private:
		uint32_t _count;
	};

	auto drop (uint32_t n) {
		return DropUnion(n);
	}


	class ReverseUnion{
	public:
		template <typename T>
		T operator() (T& container) {
			T rev_container;
			for (auto& el: container) {
				rev_container.insert(rev_container.begin(), el);
			}
			return rev_container;
		}

	};

	auto reverse () {
		return ReverseUnion();
	}

	template<typename Func>
	class FilterUnion{
	public:
		FilterUnion(Func function) {
			_function = function;
		}

		template<typename T>
		T operator() (T& container) {
			T filter_container;
			for (auto& el: container) {
				if (_function(el)) {
					filter_container.insert(filter_container.end(), el);
				}
			}
			return filter_container;
		}
	private:
		Func _function;
	};

	template<typename Func>
	FilterUnion<Func> filter(Func function) {
		return FilterUnion(function);
	}

	class KeysUnion{
	public:
		template<typename T>
		auto operator() (T& acontainer) {
			std::vector<typename T::key_type> keys_container;
			auto start = acontainer.begin();
			auto end = acontainer.end();
			while (start != end) {
				keys_container.insert(keys_container.end(), (*start).first);
				start++;
			}
			return keys_container;
		}

	};

	KeysUnion keys(){
		return KeysUnion();
	}

	class ValuesUnion{
	public:
		template<typename T>
		auto operator() (T& acontainer) {
			std::vector<typeof((acontainer.begin() -> second))> values_container;
			auto start = acontainer.begin();
			auto end = acontainer.end();
			while (start != end) {
				values_container.insert(values_container.end(), (*start).second);
				start++;
			}
			return values_container;
		}

	};

	ValuesUnion values(){
		return ValuesUnion();
	}
}
