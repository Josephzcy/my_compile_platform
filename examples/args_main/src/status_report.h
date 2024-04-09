// Copyright [2022] - MiniEye INC.

#ifndef _ARGS_MAIN_STATUS_REPORT_H_
#define _ARGS_MAIN_STATUS_REPORT_H_

#include "glog/logging.h"

namespace examples {
namespace args_main {
    enum class LogLevel {
        kInfo = 0,
        kWarning,
        kError,
        kFatal
    };

    enum class ErrorCode {
        kNoError = 0,
        // TODO: app's specific error code
    };

    extern LogLevel kLogLevel;
    #define MLOG(severity) \
            !(kLogLevel<=LogLevel(google::severity)) ? (void) 0 : google::LogMessageVoidify() & LOG(severity)

    extern ErrorCode kErrorCode;
}  // namespace args_main
}  // namespace examples

#endif  // _ARGS_MAIN_STATUS_REPORT_H_