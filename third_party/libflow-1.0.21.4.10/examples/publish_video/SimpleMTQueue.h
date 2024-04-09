/// @file SimpleMTQueue.h
/// @brief The SimpleMTQueue extension.
/// @author Zhang Qi (zhangqi@minieye.cc)
/// @date 2018-12-15
/// Copyright (C) 2018 - MiniEye INC.

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <condition_variable>
#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <msgpack.hpp>

template <typename T>
class MTQueue {
 public:
  MTQueue() :
    capacity(1) {}

  explicit MTQueue(int32_t size) :
    capacity(size) {}

  void SetCapacity(int32_t cap) {
    capacity = cap;
    if (static_cast<int>(queue.size()) > capacity) {
      queue.resize(capacity);
    }
  }

  // Params:
  //   qsize: queue size after pushing
  //   dropped: whetcher an element is dropped
  //   orphan: the dropped element
  void Push(const T& val, int* qsize, int* dropped, T* orphan) {
    std::lock_guard<std::mutex> guard(mutex);
    int size = static_cast<int>(queue.size());
    if (size >= capacity) {
      if (qsize) {
        *qsize = size;
      }

      if (dropped) {
        *dropped = 1;
      }

      if (orphan) {
        *orphan = queue[0];
      }

      queue.pop_front();
    } else {
      if (qsize) {
        *qsize = size + 1;
      }

      if (dropped) {
        *dropped = 0;
      }
    }

    queue.push_back(val);
    cond.notify_one();
  }

  int SafelyPull(T* elem) {
    *elem = queue[0];
    queue.pop_front();
    return 0;
  }

  // Return values:
  //   0: success, an element is pulled out of the queue
  //   1: failure, the queue is empty, EAGAIN
  int PullNonblock(T* elem) {
    std::lock_guard<std::mutex> guard(mutex);
    size_t size = queue.size();
    if (size > 0) {
      return SafelyPull(elem);
    } else {
      return 1;
    }
  }

  // Params:
  //   timeout_ms:
  //     <0: infinite block
  //      0: non-block
  //     >0: block for at most timeout_ms
  // Return values:
  //   0: success, an element is pulled out of the queue
  //   1: failure, the queue is empty, EAGAIN
  //   2: failure, the queue is empty, and waiting was timeout
  int Pull(T* elem, int timeout_ms) {
    int ret = PullNonblock(elem);
    if (ret == 0 || timeout_ms == 0) {
      return ret;
    }

    auto pred = [&]{ return queue.size() > 0; };

    if (timeout_ms < 0) {  // infinite block
      std::unique_lock<std::mutex> lock(mutex);
      cond.wait(lock, pred);
      return SafelyPull(elem);
    } else if (timeout_ms > 0) {  // wait for a while
      std::unique_lock<std::mutex> lock(mutex);
      auto duration = std::chrono::milliseconds(timeout_ms);
      int pred_ok = cond.wait_for(lock, duration, pred);
      if (pred_ok) {
        return SafelyPull(elem);
      } else {
        return 2;
      }
    }

    return 2;
  }

 public:
  int capacity;
  std::mutex mutex;
  std::condition_variable cond;
  std::deque<T> queue;
};
