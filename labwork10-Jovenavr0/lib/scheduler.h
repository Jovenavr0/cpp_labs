#pragma once
#include <deque>
#include <vector>
#include <cstdint>
#include <iostream>

namespace sched{

	class Task {
	public:

		virtual void MakeTask() {
		}

		virtual uint64_t GetNumberOfTasks() {
		}

	};

	template<typename Func, typename Arg1, typename Arg2>
	class ConcreteTask2Arg: public Task {
	public:

		ConcreteTask2Arg(Func lambda, Arg1 first, Arg2 second, uint64_t value) {
			function_ = lambda;
			number_ = value;
			first_argument_ = first;
			second_argument_ = second;
			for_function2_ = for_function1_ = nullptr;
			checker_ = 1;
		}

		ConcreteTask2Arg(Func lambda, Arg1* first, Arg2 second, uint64_t value) {
			function_ = lambda;
			number_ = value;
			first_argument_ = *first;
			for_function1_ = first;
			for_function2_ = nullptr;
			second_argument_ = second;
			checker_ = 2;
		}

		ConcreteTask2Arg(Func lambda, Arg1 first, Arg2* second, uint64_t value) {
			function_ = lambda;
			number_ = value;
			first_argument_ = first;
			for_function1_ = nullptr;
			second_argument_ = *second;
			for_function2_ = second;
			checker_ = 3;
		}

		ConcreteTask2Arg(Func lambda, Arg1* first, Arg2* second, uint64_t value) {
			function_ = lambda;
			number_ = value;
			first_argument_ = *first;
			for_function1_ = first;
			second_argument_ = *second;
			for_function2_ = second;
			checker_ = 4;
		}

		ConcreteTask2Arg(const ConcreteTask2Arg& other){
			function_ = other.function_;
			number_ = other.number_;
			first_argument_ = other.first_argument_;
			second_argument_ = other.second_argument_;
			for_function1_ = other.for_function1_;
			for_function2_ = other.for_function2_;
			checker_ = other.checker_;
		}

		uint64_t GetNumberOfTasks() override {
			return number_;
		}

		void MakeTask() override {
			if (checker_ == 1) {
				value_ = function_(first_argument_, second_argument_);
			} else if (checker_ == 2) {
				value_ = function_(*for_function1_, second_argument_);
			} else if (checker_ == 3) {
				value_ = function_(first_argument_, *for_function2_);
			} else if (checker_ == 4) {
				value_ = function_(*for_function1_, *for_function2_);
			}
		}

	private:
			Func function_;
			uint64_t number_;
			Arg1 first_argument_;
			Arg2 second_argument_;
			Arg1* for_function1_;
			Arg2* for_function2_;
			uint16_t checker_;
			typedef decltype(function_(first_argument_, second_argument_)) ValueType;
			ValueType value_;
	public:

		ValueType* GetData() {
			return &value_;
		}

	};

	template<typename Func, typename Arg1>
	class ConcreteTask1Arg: public Task{
	public:

		ConcreteTask1Arg(Func lambda, Arg1 first, uint64_t value) {
			function_ = lambda;
			number_ = value;
			for_function_ = nullptr;
			first_argument_ = first;
			checker_ = 1;
		}

		ConcreteTask1Arg(Func lambda, Arg1* first, uint64_t value) {
			function_ = lambda;
			number_ = value;
			first_argument_ = *first;
			for_function_ = first;
			checker_ = 2;
		}

		ConcreteTask1Arg(const ConcreteTask1Arg& other){
			function_ = other.function_;
			number_ = other.number_;
			first_argument_ = other.first_argument_;
			for_function_ = other.for_function_;
			checker_ = other.checker_;
		}

		void MakeTask() override {
			if (checker_ == 1) {
				value_ = function_(first_argument_);
			} else if (checker_ == 2){
				value_ = function_(*for_function_);
			}

		}

		uint64_t GetNumberOfTasks() override {
			return number_;
		}

	private:
		Func function_;
		uint64_t number_;
		Arg1 first_argument_;
		Arg1* for_function_;
		uint16_t checker_;
		typedef decltype(function_(first_argument_)) ValueType;
		ValueType value_;
	public:

		ValueType* GetData() {
			return &value_;
		}

	};

	template<typename Func>
	class ConcreteTask0Arg: public Task{
	public:

		ConcreteTask0Arg(Func lambda, uint64_t value) {
			function_ = lambda;
			number_ = value;
		}

		ConcreteTask0Arg(const ConcreteTask0Arg& other){
			function_ = other.function_;
			number_ = other.number_;
		}

		void MakeTask() override {
			value_ = function_();
		}

		uint64_t GetNumberOfTasks() override {
			return number_;
		}

	private:
		Func function_;
		uint64_t number_;
		typedef decltype(function_()) ValueType;
		ValueType value_;
	public:

		ValueType* GetData() {
			return &value_;
		}
	};

	class TTaskScheduler{
	public:

		TTaskScheduler() {
			count_ = 0;
			count_called_ = 0;
		}

		template<typename TypeAnswer, typename TypeTask>
		auto getResult(TypeTask& task){
			return static_cast<TypeAnswer>(*(&task) -> GetData());
		}

		template< typename TypeTask>
		auto getResult(TypeTask& task){
			return *(&task) -> GetData();
		}

		template<typename TypeAnswer, typename TypeTask>
		auto getFutureResult(TypeTask& task) {
			count_called_++;
			called_buffer_.push_back((&task) -> GetNumberOfTasks());
			return static_cast<TypeAnswer*>((&task) -> GetData());
		}

		template<typename TypeTask>
		auto getFutureResult(TypeTask& task) {
			count_called_++;
			called_buffer_.push_back((&task) -> GetNumberOfTasks());
			return (&task) -> GetData();
		}

		template<typename Func, typename Arg1, typename Arg2>
		auto add(Func lambda, Arg1 first, Arg2 second) {
			Task* task;
			ConcreteTask2Arg now_task(lambda, first, second, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func, typename Arg1, typename Arg2>
		auto add(Func lambda, Arg1* first, Arg2 second) {
			Task* task;
			ConcreteTask2Arg now_task(lambda, first, second, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func, typename Arg1, typename Arg2>
		auto add(Func lambda, Arg1 first, Arg2* second) {
			Task* task;
			ConcreteTask2Arg now_task(lambda, first, second, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func, typename Arg1, typename Arg2>
		auto add(Func lambda, Arg1* first, Arg2* second) {
			Task* task;
			ConcreteTask2Arg now_task(lambda, first, second, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func, typename Arg1>
		auto add(Func lambda, Arg1 first) {
			Task* task;
			ConcreteTask1Arg now_task(lambda, first, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func, typename Arg1>
		auto add(Func lambda, Arg1* first) {
			Task* task;
			ConcreteTask1Arg now_task(lambda, first, count_);
			task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		template<typename Func>
		auto add(Func lambda) {
			ConcreteTask0Arg now_task(lambda, count_);
			Task* task = &now_task;
			all_tasks_.push_back(task);
			graph_.emplace_back();
			MakeGraph();
			count_ += 1;
			return now_task;
		}

		void executeAll() {
			std::deque<uint32_t> priority = TopSort();
			uint32_t now_elem;
			while (!priority.empty()) {
				now_elem = priority.back();
				priority.pop_back();
				DoTask(now_elem);
			}
		}

	private:

		void MakeGraph(){
			if (count_called_ != 0) {
				for (auto& el: called_buffer_) {
					graph_[count_].push_back(el);
				}
				called_buffer_.clear();
				count_called_ = 0;
			}
		}

		void DoTask(uint32_t index) {
			Task* task = all_tasks_[index];
			task -> MakeTask();
		}

		std::deque<uint32_t> TopSort() {
			std::vector<uint32_t> visited(count_, 0);
			std::deque<uint32_t> priority;

			for (uint32_t index = 0; index < count_; ++index) {
				if (!visited[index]) {
					dfs(index, visited, graph_, priority);
				}
			}

			return priority;
		}

		void dfs(uint32_t index, std::vector<uint32_t>& visited, std::vector<std::vector<uint32_t>>& graph, std::deque<uint32_t>& priority) {
			visited[index] = 1;
			uint32_t size = graph[index].size();
			for (uint32_t i = 0; i < size; ++i){
				if (!visited[graph[index][i]]) {
					dfs(graph[index][i], visited, graph, priority);
				} else if (visited[graph[index][i]] == 1) {
					std::cerr << "You have cycle in your task. Please fix it";
					exit(EXIT_FAILURE);
				}
			}
			visited[index] = 2;
			priority.push_front(index);
		}

		std::vector<Task*> all_tasks_;
		uint32_t count_;
		uint16_t count_called_;
		std::vector<uint64_t> called_buffer_;
		std::vector<std::vector<uint32_t>> graph_;

	};

}
