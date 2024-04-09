// Copyright (c) 2022 the Logfacade Authors, All right reserved.

#ifndef LOGFACADE_LOGFACADE_H_
#define LOGFACADE_LOGFACADE_H_

#ifdef LOGFACADE_OFF

#define LOGF(...)
#define LOGF_ON(...) (false)

#else  // LOGFACADE_OFF

#include <logfacade/logger.h>

/// Do logging.
///
/// level: logfacade::Level
///   When level is less then MaxLevel(), this macro expand to a integer compare
///   and a jump.
///
/// Logging with level LEVEL.
///
/// @param LEVEL: One of kLevelHelper_
#define LOGF(LEVEL, ...)                                            \
  do {                                                              \
    if (static_cast<size_t>(::logfacade::kLevelHelper_##LEVEL) >    \
        static_cast<size_t>(::logfacade::MaxLevel()))               \
      break;                                                        \
    ::logfacade::Logger()->Log(                                     \
        ::logfacade::MessageMeta{                                   \
            ::logfacade::kLevelHelper_##LEVEL, __FILE__, __LINE__}, \
        __VA_ARGS__);                                               \
  } while (false)

/// Check if level LEVEL is on.
///
/// @param LEVEL: One of kLevelHelper_
///   Logging level LEVEL is enabled if logfacade don't filter it, and the real
///   logger enable it.
#define LOGF_ON(LEVEL)                                        \
  ((static_cast<size_t>(::logfacade::kLevelHelper_##LEVEL) <= \
    static_cast<size_t>(::logfacade::MaxLevel())) &&          \
   ::logfacade::Logger()->IsOn(::logfacade::kLevelHelper_##LEVEL))

#endif  // LOGFACADE_OFF

#endif  // LOGFACADE_LOGFACADE_H_
