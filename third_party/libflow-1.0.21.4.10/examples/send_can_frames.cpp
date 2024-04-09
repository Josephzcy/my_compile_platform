#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "flow.hpp"

static flow::Context g_flow;

static void sleep_ms(int ms) {
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms - ts.tv_sec * 1000) * 1000000;
  nanosleep(&ts, nullptr);
}

void* func_th1(void* data) {
  sleep_ms(2000);

  const char* input_file = (const char*)data;
  FILE* fp = fopen(input_file, "r");
  if (nullptr == fp) {
    fprintf(stderr, "Error: cannot open %s\n", input_file);
    exit(1);
  }

  flow::Sender sender(&g_flow, "CANWS");
  float prev_time = 0.0f;

  for (int i = 0; ; ++i) {
    float time;
    int can_id;
    int data[8];
    int ret = fscanf(fp, "%f 0x%x %d %d %d %d %d %d %d %d",
        &time, &can_id,
        &data[0], &data[1], &data[2], &data[3],
        &data[4], &data[5], &data[6], &data[7]);
    if (EOF == ret) {
      break;
    }

    char buf[8];
    for (int j = 0; j < 8; j += 1) {
      buf[j] = data[j];
    }

    if (can_id == 0x700) {
      sender.send("output.can.0x700", buf, 8);
    } else if (can_id == 0x760) {
      sender.send("output.can.0x760", buf, 8);
    } else {
      fprintf(stderr, "Error: unknown can id: %x\n", can_id);
      continue;
    }

    if (i > 0) {
      float dt = time - prev_time;
      int ms = int(dt * 1000);
      sleep_ms(ms);
    }

    prev_time = time;
  }

  fclose(fp);

  fprintf(stderr, "the thread is done\n");
  return nullptr;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s input_file\n", argv[0]);
    exit(1);
  }

  if (g_flow.start() != 0) {
    exit(1);
  }

  char* input_file = argv[1];

  pthread_t thread;
  int ret = pthread_create(&thread, nullptr, func_th1, input_file);
  if (ret != 0) {
    fprintf(stderr, "Error creating the thread: %d\n", ret);
    return 1;
  }

  ret = pthread_join(thread, nullptr);
  if (ret != 0) {
      fprintf(stderr, "Error joining the thread: %d\n", ret);
      return 1;
  }

  sleep_ms(20000);

  return 0;
}
