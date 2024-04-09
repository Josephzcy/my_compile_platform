/// @file simple_notifier.cpp
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-02-11
/// Copyright (C) 2018 - MiniEye INC.

#ifndef COMMON_CONCURRENCY_SIMPLE_NOTIFIER_HPP_
#define COMMON_CONCURRENCY_SIMPLE_NOTIFIER_HPP_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <array>
#include <string>

class SimpleNotifier {
 public:
  SimpleNotifier() {
    int rc = pipe(m_pipe.data());
    if (rc == -1) {
      throw_errno("pipe error");
    }

    set_nonblocking(m_pipe.front());
  }

  ~SimpleNotifier() {
    close_fd(m_pipe.front());
    close_fd(m_pipe.back());
  }

  void notify() {
    int rc = 0;
    char c = '1';

    do {
      rc = write(m_pipe.back(), &c, 1);
    } while (rc == -1 && errno == EINTR);

    if (rc == -1) {
      print_errno("write error");
    }
  }

  // Wait for notification.
  // timeout_msecs:
  //   -1: blocks indefinitely
  //    0: return without blocking
  //   >0: wait for timeout_msecs
  // Returns:
  //   -1: error
  //    0: not notified
  //    1: notified
  int wait(int timeout_msecs) {
    std::array<struct pollfd, 1> fds = {
      m_pipe.front(), POLLIN, 0,
    };

    int rc = poll(fds.data(), fds.size(), timeout_msecs);
    if (rc == -1) {
      print_errno("poll error");
      return -1;
    } else if (rc == 0) {  // timeout
      return 0;
    } else {
      int readable = fds.at(0).revents & POLLIN;
      if (!readable) {
        return 0;
      }

      while (true) {
        std::array<char, 64> buf;
        int rc = read(m_pipe.front(), buf.data(), buf.size());
        if (rc == -1) {
          print_errno("read error");
          return 0;
        } else if (rc == 0) {  // eof
          return 0;
        } else {
          return 1;
        }
      }
    }
  }

 private:
  static std::string format_errno(const char* what) {
    std::string str = strerror(errno);
    return std::string(what) + ": " + str;
  }

  static void print_errno(const char* what) {
    std::cerr << format_errno(what) << std::endl;
  }

  void throw_errno(const char* what) {
    throw std::runtime_error(format_errno(what));
  }

  void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
      throw_errno("fcntl error");
    }

    flags = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (flags == -1) {
      throw_errno("fcntl error");
    }
  }

  void close_fd(int fd) {
    int rc = 0;

    do {
      rc = close(fd);
    } while (rc == -1 && errno == EINTR);

    if (rc == -1) {
      print_errno("close error");
    }
  }

  std::array<int, 2> m_pipe;
};

#endif  // COMMON_CONCURRENCY_SIMPLE_NOTIFIER_HPP_
