/// @file platform.cpp
/// @brief Platform-dependent code.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2020-11-04
/// Copyright (C) 2020 - MiniEye INC.

#include "platform.h"  // NOLINT(build/include_subdir)

#include <string.h>
#include <mongoose.h>

#ifdef _WIN32
#include <windows.h>

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <fnmatch.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include "utils.hpp"

bool wildcard_match(const char *pattern, const char *str) {
#ifdef _WIN32
  return PathMatchSpecA(str, pattern) == TRUE;
#else
  // int fnmatch(const char *pattern, const char *string, int flags);
  // fnmatch returns 0 if string matches pattern, FNM_NOMATCH if there
  // is no match or another nonzero value if there is an error.
  return fnmatch(pattern, str, 0) == 0;
#endif
}

int thread_setname(const char* name) {
#if defined(_WIN32)
  return 0;
#elif defined(__APPLE__)
  return pthread_setname_np(name);
#elif defined(__FreeBSD__)
  return pthread_set_name_np(pthread_self(), name);
#else
  return pthread_setname_np(pthread_self(), name);
#endif
}

void close_socket(sock_t* sock) {
  if (!sock) {
    return;
  }

#if defined(_WIN32)
  closesocket(*sock);
#else
  while (true) {
    int rc = close(*sock);
    if (rc == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EIO) {
        errlog("close(%d) error: %s", *sock, strerror(errno));
      } else {  // EBADF
        // ignore
      }
    }

    *sock = -1;  // invalid fd
    break;
  }
#endif
}

double get_time_in_secs_d() {
  return mg_time();
}

int64_t get_time_in_msecs_i64() {
  return static_cast<int64_t>(mg_time() * 1e3);
}

int64_t get_time_in_usecs_i64() {
  return static_cast<int64_t>(mg_time() * 1e6);
}

// Return value: 0=OK, others=Error
int init_windows() {
#ifdef _WIN32
  WORD sockVersion = MAKEWORD(2, 2);
  WSADATA wsaData;
  int ret = WSAStartup(sockVersion, &wsaData);
  if (ret != 0) {
    fprintf(stderr, "Error: WSAStartup failed (code %d)\n", ret);
    return 1;
  }

  return 0;
#else
  return 0;
#endif
}

int clean_windows() {
#ifdef _WIN32
  WSACleanup();
  return 0;
#else
  return 0;
#endif
}

