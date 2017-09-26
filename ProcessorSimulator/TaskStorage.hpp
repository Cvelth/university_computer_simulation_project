#pragma once
#include "Shared.hpp"
#include <deque>
#include <mutex>
#include <functional>

namespace cs {
	namespace Exceptions {
		class EmptyQueue {};
	}
	class Task {
		Color m_color;
		float m_processing_left;
		unsigned int m_was_processed;
	public:
		Task(float color = -1.f);
		Task& operator=(Task const &other) {
			m_color = other.m_color;
			m_processing_left = other.m_processing_left;
			m_was_processed = other.m_was_processed;
			return *this;
		}
		inline Color const& color() const {
			return m_color;
		}
		inline float processing_left() const {
			return m_processing_left;
		}
		inline void set_processing_left(float value) {
			m_processing_left = value;
		}
		inline unsigned int was_processed() const {
			return m_was_processed;
		}
		inline void process() {
			m_was_processed++;
		}
	};
	class Queue : public std::deque<Task> {};
	class TaskStorage {
	protected:
		std::mutex m_mutex;
	public:
		virtual void push(Task *task = nullptr) abstract;
		virtual void repush(Task *task = nullptr) abstract;
		virtual Task pop() abstract;

		virtual void for_each(std::function<void(Task const& task)> const& lambda) abstract;
	};
	class LIFO : public TaskStorage {
		Queue m_queue;
	public:
		LIFO() : m_queue() {}
		virtual void push(Task *task = nullptr) override {
			std::lock_guard<std::mutex> l(m_mutex);
			if (task)
				m_queue.push_back(*task);
			else
				m_queue.push_back(Task());
		}
		virtual void repush(Task *task = nullptr) override {
			push(task);
		}
		virtual Task pop() override {
			Task ret;
			std::lock_guard<std::mutex> l(m_mutex);
			if (m_queue.size()) {
				ret = m_queue.back();
				m_queue.pop_back();
			} else
				throw Exceptions::EmptyQueue();
			return ret;
		}

		virtual void for_each(std::function<void(Task const& task)> const& lambda) override {
			std::lock_guard<std::mutex> l(m_mutex);
			for (auto t = m_queue.rbegin(); t != m_queue.rend(); t++) {
				lambda(*t);
			}
		}
	};

	class PER : public TaskStorage {
		Queue m_queue;
	public:
		PER() : m_queue() {}
		virtual void push(Task *task = nullptr) override {
			std::lock_guard<std::mutex> l(m_mutex);
			if (task)
				m_queue.push_back(*task);
			else
				m_queue.push_back(Task());
		}
		virtual void repush(Task *task = nullptr) override {
			push(task);
		}
		virtual Task pop() override {
			Task ret;
			std::lock_guard<std::mutex> l(m_mutex);
			if (m_queue.size()) {
				ret = m_queue.back();
				m_queue.pop_back();
			} else
				throw Exceptions::EmptyQueue();
			return ret;
		}

		virtual void for_each(std::function<void(Task const& task)> const& lambda) override {
			std::lock_guard<std::mutex> l(m_mutex);
			for (auto t = m_queue.rbegin(); t != m_queue.rend(); t++) {
				lambda(*t);
			}
		}
	};
	/* To correct.
	class PER : public TaskStorage {
		Queue m_initial_queue;
		Queue m_repush_queue;
	public:
		PER() : m_initial_queue(), m_repush_queue() {}
		virtual void push(Task *task = nullptr) override {
			if (task)
				m_initial_queue.push_back(*task);
			else
				m_initial_queue.push_back(Task());
		}
		virtual void repush(Task *task = nullptr) override {
			if (task)
				m_repush_queue.push_back(*task);
			else
				m_repush_queue.push_back(Task());
		}
		virtual Task pop() override {
			Task ret;
			if (m_initial_queue.size())
				ret = m_initial_queue.front();
			else if (m_repush_queue.size())
				ret = m_repush_queue.front();
			else
				throw Exceptions::EmptyQueue();
			return ret; 
		}
	};
	*/
}