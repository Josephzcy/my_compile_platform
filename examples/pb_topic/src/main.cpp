#include <signal.h>

#include <iostream>
#include <string>
#include <thread>

#include "dds_api.h"
#include "dds_common.h"

using namespace minieye::DDS;

static volatile int g_iRunning = 1;
void signalHandler(int iSigno) { g_iRunning = 0; }

void ThreadWriter(minieye::DDS::Writer writer) {
  for (int i = 0; g_iRunning; i++) {
    // 向 queue 写入数据
    int iRet = writer.Write(&i, sizeof(i));
    if (iRet != DDSSuccess) {
      printf("dds_write failed, iRet = %d\n", iRet);
    }

    printf("writer data i = %d\n", i);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

// @param topic: topic
// @param index: 内部数组的下标， array[index]
// @param ptr: 是指向当前元素的指针
// @param size: 当前元素的大小（字节）
// @param user: 用户数据
void onData(const char* topic, size_t index, void* ptr, size_t size,
            void* user) {
  printf("[topic: %s] array[%lu] = %d\n", topic, index, *(int*)ptr);
}

int main(int argc, char** argv) {
  signal(SIGTERM, signalHandler);
  signal(SIGHUP, signalHandler);
  signal(SIGINT, signalHandler);

  std::string config_file = "topic.json";
  minieye::DDS::Context ctx(config_file, true);

  const char* topic = "camera30";
  Context wctx("topic.json");
  Writer writer(&wctx, topic);

  Context rctx("topic.json", true);
  Reader reader(&rctx, topic, onData);

  std::thread t1(ThreadWriter, writer);
  t1.join();

  return 0;
}
