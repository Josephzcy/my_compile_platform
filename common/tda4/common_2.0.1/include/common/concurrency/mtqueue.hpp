/// @file mtqueue.hpp
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-02-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef COMMON_CONCURRENCY_MTQUEUE_H_
#define COMMON_CONCURRENCY_MTQUEUE_H_

#if _pragma_once_support
#pragma once
#endif

#include <deque>
#include <mutex>
#include <exception>

template <typename T>
class Queue {
 public:
  Queue() : m_capacity(16), m_drop_which(0) {}

  // capacity: max size
  // drop_which: 0: head, 1: tail
  Queue(size_t capacity, int drop_which) :
    m_capacity(capacity), m_drop_which(drop_which) {}

  // Returns the queue's size (current element count).
  virtual size_t size() {
    return m_deque.size();
  }

  // Check if the queue is empty.
  virtual bool empty() {
    return m_deque.size() == 0;
  }

  // Check if the queue is full.
  virtual bool full() {
    return m_deque.size() >= m_capacity;
  }

  // Push an element into the queue.
  // If the queue is full (size() == capacity):
  //   if drop_which == 0 (head):
  //     remove the first element from the queue;
  //     append the new element to the end of the queue;
  //     return true.
  //   else if drop_which == 1 (tail):
  //     drop the new element;
  //     the queue is left unchanged;
  //     return false.
  // else:
  //   append the new element to the end of the queue;
  //   return true.
  virtual bool push(const T& val) {
    if (m_deque.size() >= m_capacity) {
      if (m_drop_which == 0) {
        m_deque.pop_front();
        m_deque.push_back(val);
        return true;
      } else {
        return false;
      }
    } else {
      m_deque.push_back(val);
      return true;
    }
  }

  // If the queue is empty (size() == 0):
  //   throw an out_of_range exception.
  // else:
  //   remove the first element from the queue;
  //   return a copy of the first element.
  virtual T pop() {
    if (m_deque.empty()) {
      throw std::out_of_range("pop from an empty Queue");
    } else {
      T val = m_deque.front();
      m_deque.pop_front();
      return val;
    }
  }

 protected:
  size_t m_capacity;
  int m_drop_which;
  std::deque<T> m_deque;
};

template <typename T>
class MTQueue : public Queue<T> {
 public:
  MTQueue() : Queue<T>() {}

  MTQueue(size_t capacity, int drop_which) :
    Queue<T>(capacity, drop_which) {}

  virtual size_t size() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return Queue<T>::size();
  }

  virtual bool empty() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return Queue<T>::empty();
  }

  virtual bool full() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return Queue<T>::full();
  }

  virtual bool push(const T& val) {
    std::lock_guard<std::mutex> guard(m_mutex);
    return Queue<T>::push(val);
  }

  virtual T pop() {
    std::lock_guard<std::mutex> guard(m_mutex);
    return Queue<T>::pop();
  }

 protected:
  std::mutex m_mutex;
};

#endif  // COMMON_CONCURRENCY_MTQUEUE_H_
