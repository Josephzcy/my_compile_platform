/// Copyright (C) 2019 - MiniEye INC.

#include <iostream>
#include <memory>
#include <msgpack.hpp>
#include "./client1.h"

void decode_server_message(const char* data, size_t size) {
  std::shared_ptr<msgpack::object_handle> oh(new msgpack::object_handle);

  *oh = msgpack::unpack(data, size);  // copy?
  msgpack::object deserialized = oh->get();

  std::map<std::string, msgpack::object> dst;
  deserialized.convert(dst);  // copy?

  const std::string& source = dst["source"].convert();
  const std::string& topic = dst["topic"].convert();
  int64_t time = dst["time"].convert();

  std::cout << "source: " << source << std::endl;
  std::cout << "topic: " << topic << std::endl;
  std::cout << "time: " << time << std::endl;

  if (topic != "image") {
    return;
  }

  ImageVec.Push(oh, nullptr, nullptr, nullptr);
}
