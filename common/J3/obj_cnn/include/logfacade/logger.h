// Copyright (c) 2022 the Logfacade Authors, All right reserved.

#ifndef LOGFACADE_LOGGER_H_
#define LOGFACADE_LOGGER_H_

#ifndef LOGFACADE_LOGFACADE_H_
#error use "logfacade/logger.h" without "logfacade/logfacade.h".
#endif

// C
#include <cstdarg>
#include <cstdint>
#include <cstring>
// C++
#include <atomic>
#include <functional>
#include <mutex>
#include <sstream>
#include <utility>

namespace logfacade {

// LevelFilter filter log with lower level.
//
// LevelFilter and Level with same name will be same interger if user cast it
// to.
//
// There's no Level::kOff, but there is LevelFilter::kOff.
enum class LevelFilter : size_t {
  kOff = 0,
  kFatal,
  kError,
  kWarn,
  kInfo,
  kDebug,
};

// Level labeled a log message.
enum class Level : size_t {
  kFatal = static_cast<size_t>(LevelFilter::kFatal),
  kError = static_cast<size_t>(LevelFilter::kError),
  kWarn = static_cast<size_t>(LevelFilter::kWarn),
  kInfo = static_cast<size_t>(LevelFilter::kInfo),
  kDebug = static_cast<size_t>(LevelFilter::kDebug),
};

// These level helper is used by macros, kLevelHelper_##LEVEL will expand to a
// logfacade::Level.
constexpr Level kLevelHelper_FATAL = Level::kFatal;
constexpr Level kLevelHelper_ERROR = Level::kError;
constexpr Level kLevelHelper_WARN = Level::kWarn;
constexpr Level kLevelHelper_INFO = Level::kInfo;
constexpr Level kLevelHelper_DEBUG = Level::kDebug;

// Backends may use this if logging message shows log level.
//
// `kLevelFilterName[static_cast<size_t>(level)]` is name of level `level`.
constexpr static char const *kLevelFilterName[] = {
    "OFF", "FATAL", "ERROR", "WARN", "INFO", "DEBUG"};

// MessageMeta contains log data apart from logging message.
struct MessageMeta {
  Level level;
  char const *file;
  size_t line;
  char const *package;

  MessageMeta(Level level_, char const *file_, size_t line_)
      : level(level_), file(file_), line(line_) {
    // *package* holds value of BUSGO_PACKAGE where this MessageMeta contructor
    // is called. This is where a logging macro is expaneded.
    //
    // So this value works only if the code that expaneded a logging macro is
    // code in a library instead of a header file.
    //
    // For example, if package A offers header file, with logfacade logging
    // macro, then package B include this header file, this value will refers
    // to package B instead of package A.
    //
    // To fix this, logger implementions should find out if a logging macro is
    // called in a header file, such as searching substring "include/" in
    // *file*, if packages follows a good project standard.
    //
    // Furthermore, logger backends should handle MessageMeta with empty
    // *package*, so that this backends can works in project without *busgo*
    // support.

#ifdef BUSGO_PACKAGE

#define logfacade_macro_to_cstr_helper(m) (#m)
#define logfacade_macro_to_cstr(m) logfacade_macro_to_cstr_helper(m)

    package = logfacade_macro_to_cstr(BUSGO_PACKAGE);

#undef logfacade_macro_to_cstr
#undef logfacade_macro_to_cstr_helper

#else  // BUSGO_PACKAGE

    package = nullptr;

#endif  // BUSGO_PACKAGE
  }
};

// BaseLogger is the base class for backend using logfacade.
class BaseLogger {
 public:
  // If this logger enable logging with level `level`?
  virtual bool IsOn(Level level) = 0;

  // Do logging.
  //
  // See `LogV`.
  virtual void Log(MessageMeta &&record, char const *format, ...) {
    va_list args;
    va_start(args, format);
    LogV(std::move(record), format, args);
    va_end(args);
  }

  // Do logging, variable argument list version.
  //
  // Implementions may not write log when !IsOn(level) && record.level ==
  // level.
  virtual void LogV(MessageMeta &&, char const *format, va_list) = 0;

  virtual ~BaseLogger() {}
};

// NopLogger don't write anything.
struct NopLogger : public BaseLogger {
  bool IsOn(Level) override { return false; }
  void LogV(MessageMeta &&, char const *, va_list) override {}
  void Log(MessageMeta &&, char const *, ...) override {}
};

// Global logger pointer and mutex to lock it.
extern BaseLogger *glogger;
extern std::mutex gmutex;

// Get logger.
inline BaseLogger *Logger() { return glogger; }

// Set logger, backends should call this function in its prepare step.
inline BaseLogger *SetLogger(BaseLogger *logger) {
  std::lock_guard<std::mutex> _lock(gmutex);
  auto *original = glogger;
  glogger = logger;
  return original;
}

// Global filter to filter log message.
extern std::atomic<size_t> gfilter;

// Set `gfilter`.
inline void SetMaxLevel(LevelFilter level) {
  gfilter.store(static_cast<size_t>(level), std::memory_order_relaxed);
}

// Get `gfilter`
inline LevelFilter MaxLevel() {
  return static_cast<LevelFilter>(gfilter.load(std::memory_order_relaxed));
}

}  // namespace logfacade

#endif  // LOGFACADE_LOGGER_H_
