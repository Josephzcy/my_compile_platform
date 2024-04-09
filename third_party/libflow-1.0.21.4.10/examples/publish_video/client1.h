#pragma once

#include <deque>
#include <mutex>
#include <string>
#include <memory>
#include <msgpack.hpp>
#include "./SimpleMTQueue.h"

extern MTQueue<std::shared_ptr<msgpack::object_handle>> ImageVec;

void RecvThreadFunc(void);

void decode_server_message(const char* data, size_t size);
