// Copyright [2022] - MiniEye INC.

#include <stdio.h>
#include <unistd.h>

#include "examples/args_main//src/status_report.h"
#include "examples/args_main/config/configuration.h"
#include "examples/args_main/version.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "signal.h"

using namespace examples::args_main;

static bool kStopMain = false;

void SignalHandler(int sig) {
  printf("\nleaving args_main\n");
  kStopMain = true;
}

void PrintUsage() {
  printf("Available command line options:\n");
  printf("-h Print usage\n");
  printf("-V Print version\n");
  printf("-v [log level] You may input log level\n");
  printf("-c [config file] You may input config file\n");
}

void SetLogLevel(int level) {
  if (level >= (int)LogLevel::kInfo && level <= (int)LogLevel::kFatal) {
    kLogLevel = (LogLevel)((int)LogLevel::kFatal - level);
  }
}

void ParseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  // init
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = true;
  FLAGS_colorlogtostderr = true;
  signal(SIGINT, SignalHandler);
  signal(SIGTERM, SignalHandler);
  // parse args
  ParseArgs(argc, argv);
  LOG_IF(INFO, kLogLevel <= LogLevel::kInfo) << "info msg of glog";
  LOG_IF(WARNING, kLogLevel <= LogLevel::kWarning) << "warning msg of glog";
  LOG_IF(ERROR, kLogLevel <= LogLevel::kError) << "error msg of glog";
  MLOG(INFO) << "info msg of mlog";
  MLOG(WARNING) << "warning msg of mlog";
  MLOG(ERROR) << "error msg of mlog";

  // main loop
  while (!kStopMain) {
    // Do something
    usleep(100e3);
  }

  printf("leave args_main\n");
  return 0;
}

void ParseArgs(int argc, char* argv[]) {
  int option;
  // please launch these options before gflags'
  while ((option = getopt(argc, argv, "hVv:c:")) != -1) {
    if (option == 'h') {
      PrintUsage();
      exit(EXIT_SUCCESS);
    } else if (option == 'V') {
      printf("Version: %d.%d.%d\n", _RELEASE_VERSION_, _MAJOR_VERSION_,
             _MINOR_VERSION_);
      exit(EXIT_SUCCESS);
    } else if (option == 'v') {
      int loglevel = atoi(optarg);
      if (loglevel >= (int)LogLevel::kInfo &&
          loglevel <= (int)LogLevel::kFatal) {
        LOG(INFO) << "Set log level to: " << loglevel;
        SetLogLevel(loglevel);
      } else {
        LOG(INFO) << "Invalid log level: " << loglevel;
        SetLogLevel((int)LogLevel::kInfo);
        exit(EXIT_FAILURE);
      }
    } else if (option == 'c') {
      kConfigYaml = optarg;
      // TODO: load and check configuration
      // exit(EXIT_FAILURE);
    } else if (option == '?') {
      LOG(ERROR) << "Unknown option: " << optopt;
      if (optopt == '-') {
        break;
      }
      PrintUsage();
      exit(EXIT_FAILURE);
    } else {
      LOG(ERROR) << "Invalid command option";
      PrintUsage();
      exit(EXIT_FAILURE);
    }
  }
}
