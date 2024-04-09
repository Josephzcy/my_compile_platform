// Copyright (c) 2022-2023 The Simplelog Authors. All rights reserved.

#ifndef SIMPLELOG_SIMPLELOG_H_
#define SIMPLELOG_SIMPLELOG_H_

#include <logfacade/logfacade.h>

#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace simplelog {

using logfacade::BaseLogger;
using logfacade::Level;
using logfacade::LevelFilter;
using logfacade::MessageMeta;

struct Option;

/// 初始化日志。
///
/// 它设置了 logfacade 的 BaseLogger。所有在此之后的 LOGF 宏输出的日志会收到
/// simplelog 配置影响。
///
/// @param option: 日志配置选项。
void Initialize(Option option);

/// 关闭日志。
///
/// 恢复 Initialize 之前的 logfacade::BaseLogger。
void Close();

/// simplelog 的日志选项。
struct Option {
  //
  // Style and format
  //

  /// 终端输出打开颜色输出。
  ///
  /// 在输出不是终端（例如文件）时不会生效。
  bool use_color = true;

  /// 打印 SetThisThreadName 设置的线程名称，前缀为 thrd-。
  bool show_thread_name = false;

  /// 打印输出日志时的系统时间。
  bool show_time = false;

  /// 打印长的日志等级名称，例如 INFO/DEBUG 而不是 I/D。
  bool long_level_name = false;

  /// 打印 std::this_thread::get_id() 返回的线程 ID，前缀 tid-。
  bool show_std_thread_id = false;

  /// 类似 show_std_thread_id，但是将它映射到一个整数，前缀 tidm-。
  bool show_std_thread_id_mapped = false;

  int path_max_len = 32;

  //
  // Control
  //

  /// 日志等级。
  LevelFilter level_filter = LevelFilter::kInfo;

  /// 在输出每条日志之后刷新 IO 的缓冲区。
  ///
  /// 这可以使得日志的输出更及时，但是对性能的影响也更多。
  bool flush = false;

  /// 在 Initialize 时从环境变量 SIMPLELOG_LEVEL 读取日志等级（如果设置了）。
  ///
  /// SIMPLELOG_LEVEL 可以采取 ParseLevelFilter 设计的语法描述日志等级。
  bool use_env = true;

  /// 在打印 FATAL 日志之后退出程序。
  bool terminate_on_fatal = true;

  /// 将产生的日志转发到 dlt 日志库。
  bool use_dlt = false;

  /// dlt 日志库需要设置的 app_name。
  ///
  /// use_dlt 打开时有效。
  std::string dlt_app_name = "logf";

  /// dlt 日志库需要设置的 description。
  ///
  /// use_dlt 打开时有效。
  std::string dlt_description = "logf use dlt";

  //
  // Other sinks
  //

  /// 将日志转发到文件。
  bool use_logging_file = false;

  /// 将日志写到 logging_file 指向的文件。
  ///
  /// 在 use_logging_file 为真时有效。
  std::string logging_file;

  /// 将日志写到标准错误输出。
  bool use_stderr = true;

  // Buffer

  /// 日志缓冲区长度。
  ///
  /// 超过缓冲区长度的日志会被截断。
  size_t buffer_size = 1024;

  /// 用日志等级描述字符串设置日志等级。
  ///
  /// 日志等级描述字符串是以逗号（`,`）分割的字符串。每个部分是：
  ///
  /// - `LEVEL`，取 debug/info/warn/error/fatal，表示设置 level_filter
  ///   为对应的等级。
  /// - `=LEVEL`，设置 level_filter_no_package 为对应的等级。
  /// - `PACKAGE=LEVEL` 设置 `level_filter_no_package[PACKAGE]` 为 LEVEL
  ///   对应的等级。
  ///
  /// 例如：
  ///
  /// ```
  /// debug,=debug
  /// ```
  ///
  /// 设置 `level_filter` 和 `level_filter_no_package` 为 LevelFilter::kDebug。
  ///
  /// 例如：
  ///
  /// ```
  /// error,obj-cnn=debug
  /// ```
  ///
  /// 设置 `level_filter` 为 LevelFilter::kDebug。设置
  /// `level_filter_map["obj-cnn"]` 为 LevelFilter::kDebug。
  ///
  /// @param text: 对日志等级描述字符串。
  /// @return 解析成功时返回 0。
  int ParseLevelFilter(std::string const &text);
};

/// 设置当前线程的线程名。
///
/// 这是线程安全的。
void SetThisThreadName(char const *name);

char const *GetThisThreadName();

// Other options.

// If logger leak queue is logger then gLeakQueueTooLarge. Old loggers will be
// deleted until queue size is less or equal then gLeakQueueSmallEnough.
//
// Before delete loggers, work thread will wait for gDeleteLoggerWaitMillis
// milliseconds to avoid some thread use this logger to writer log.
extern std::atomic<size_t> gLeakQueueTooLarge;
extern std::atomic<size_t> gLeakQueueSmallEnough;
extern std::atomic<size_t> gDeleteLoggerWaitMillis;

/// simplelog 所注册的 logfacade::BaseLogger 实现。
class Logger : public BaseLogger {
 public:
  explicit Logger(Option);
  ~Logger();

  bool IsOn(Level level) override;
  void LogV(MessageMeta &&record, char const *format, va_list args) override;

  class Impl;

 private:
  std::unique_ptr<Impl> impl_;
};

}  // namespace simplelog

#endif  // SIMPLELOG_SIMPLELOG_H_
