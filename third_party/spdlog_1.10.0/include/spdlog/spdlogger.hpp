/*!
 * @file debug_logger.h
 * @brief 调试日志
 * @details
 * @author chenwei/zhangpeng
 * @date 2020/3/23 19:59
 */
#ifndef BASE_DEBUG_LOGGER_H
#define BASE_DEBUG_LOGGER_H

#include <map>
#include <memory>
#include <iostream>
#include "spdlog.h"
// #include "spdlog/async.h"
#include "sinks/rotating_file_sink.h"
#include "sinks/stdout_color_sinks.h"

#ifdef WIN32
#include "direct.h"
#else
#include <sys/stat.h>
#include <sys/types.h> 
#endif // WIN32

#ifndef IN
#define IN
#define OUT
#define INOUT
#endif // !IN

#define slog slogger::MyLogger::GetInstance()

namespace slogger
{
using level = spdlog::level::level_enum;
using VecString = std::vector<std::string>;

// 可以输出的消息类型：时间、模块名称、等级、消息内容
enum class msg_type
{
    time,
    module,
    level,
    msg
};

/*!
 * @class Cfg
 * @brief 日志配置类
 * @author chenwei
 * @date 2020/4/16 18:07
 */
class Cfg
{
public:
    Cfg() = default;

    Cfg(const std::string& log_path) :
        log_path(log_path)
    {
    }

    // 日志等级 trace=0 debug=1 info=2 warn=3 err=4 critical=5 off=6
    level       log_level     = level::info;                // 本地日志输出等级
    level       console_level = level::err;                 // 控制台输出等级
    uint64_t    max_file_size = 100000;                     // 单个日志文件大小, 单位:KB
    uint64_t    max_files     = 100;                        // 日志文件数量(超出后会覆盖)
    std::string log_path      = "record.log";               // 调试日志文件输出位置
    VecString   modules_registered;                         // 所有已注册的模块名称，若为空则log不输出模块名称，如{"planning","control"}
    VecString   modules_output;                             // 可以输出log的模块名称，若为空则所有模块都可以输出log，若有值则只有已注册的模块可输出，如{"control"}代表planning不会输出
    std::string log_pattern           = "[%n] [%^%l%$] %v"; // 输出日志形式 完整的为：[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v
    bool        is_single_thread_mode = true;               // 是否为单线程使用(单线程无锁竞争更快，若要在多线程使用置为false)

    void SetLogLevel(level log_level_)
    {
        log_level = log_level_;
    }

	void SetMaxFilesSize(uint64_t max_file_size_)
	{
		max_file_size = max_file_size_;
	}

	void SetMaxFilesNum(uint64_t max_files_num)
	{
		max_files = max_files_num;
	}

    void SetConsoleLevel(const level console_level_)
    {
        console_level = console_level_;
    }

    void SetLogPath(const std::string& log_path_)
    {
        log_path = log_path_;
    }

    void SetModulesRegistered(const std::initializer_list<std::string> modules_registered_)
    {
        modules_registered = modules_registered_;
    }

    void SetModulesOutput(const std::initializer_list<std::string> modules_output_)
    {
        modules_output = modules_output_;
    }

    void SetLogPattern(const std::initializer_list<enum msg_type> pattern_members = {msg_type::msg})
    {
        log_pattern         = "";
        const auto* it_find = std::find(pattern_members.begin(), pattern_members.end(), msg_type::time);
        if (it_find != pattern_members.end())
        {
            log_pattern += "[%Y-%m-%d %H:%M:%S.%e] ";
        }
        it_find = std::find(pattern_members.begin(), pattern_members.end(), msg_type::module);
        if (it_find != pattern_members.end())
        {
            log_pattern += "[%n] ";
        }
        it_find = std::find(pattern_members.begin(), pattern_members.end(), msg_type::level);
        if (it_find != pattern_members.end())
        {
            log_pattern += "[%^%l%$] "; // 对应彩色打印
        }
        log_pattern += "%v";
    }

    void SetLogPattern(const std::string& log_pattern_str)
    {
        log_pattern = log_pattern_str;
    }

    void IsSingleThreadMode(const bool is_single_thread_mode_)
    {
        is_single_thread_mode = is_single_thread_mode_;
    }
};

class MyLogger
{
public:
    MyLogger()                           = default;
    ~MyLogger()                          = default;
    MyLogger(const MyLogger&)            = delete;
    MyLogger& operator=(const MyLogger&) = delete;

    static MyLogger& GetInstance()
    {
        static MyLogger instance;
        return instance;
    }

    /**
     * @brief 初始化日志
     * @details 采用rotating_logger_mt，即当超出规定大小时，会删除当前日志内容，重新开始写入
     * @param [IN] const LoggerConfig& config, 日志配置
     * @return void
     */
    void initial(const Cfg& cfg)
    {
        // 创建日志文件夹
        // mkdirs(cfg.log_path.c_str()); // 最新版已经支持自动创建路径

        if (cfg.is_single_thread_mode)
        {
            // 输出到屏幕
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
            console_sink->set_level(cfg.console_level);
            g_sinks.push_back(console_sink);

            // 输出到文件
			if (cfg.log_level < level::off)
			{
				auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(
					cfg.log_path, cfg.max_file_size * 1024, cfg.max_files, true);
				file_sink->set_level(cfg.log_level);
				g_sinks.push_back(file_sink);
			}
        }
        else
        {
            // 输出到屏幕
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(cfg.console_level);
            g_sinks.push_back(console_sink);

            // 输出到文件
			if (cfg.log_level < level::off)
			{
				auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
					cfg.log_path, cfg.max_file_size * 1024, cfg.max_files, true);
				file_sink->set_level(cfg.log_level);
				g_sinks.push_back(file_sink);
			}
        }

        // 若模块为空，代表并不使用模块
        if (cfg.modules_registered.empty())
        {
            RegisterLogWithoutModule(cfg.log_pattern);
            return;
        }

        // 注册有效的模块
        for (auto& module_name : cfg.modules_registered)
        {
            RegisterLog(module_name, cfg.log_pattern);
        }

        // 要输出的模块名称，若为空则所有均可输出
        if (cfg.modules_output.empty())
        {
            g_module_names = cfg.modules_registered;
        }
        else //非空则只有指定模块可以输出
        {
            for (const auto& module_name : cfg.modules_output)
            {
                if (std::find(cfg.modules_registered.begin(), cfg.modules_registered.end(), module_name) != cfg.modules_registered.end())
                {
                    g_module_names.emplace_back(module_name);
                }
            }
        }
    }

    template <typename... Args>
    void out(IN const std::string& module_name, const level& log_level, IN const char* fmt, IN Args&&...args)
    {
        if (!IsRegisterd(module_name))
        {
            return;
        }
        const auto logger_ptr = GetLogger(module_name);
        if (logger_ptr)
        {
            switch (log_level)
            {
                case level::trace:
                    logger_ptr->trace(fmt, std::forward<Args>(args)...);
                    break;
                case level::debug:
                    logger_ptr->debug(fmt, std::forward<Args>(args)...);
                    break;
                case level::info:
                    logger_ptr->info(fmt, std::forward<Args>(args)...);
                    break;
                case level::warn:
                    logger_ptr->warn(fmt, std::forward<Args>(args)...);
                    break;
                case level::err:
                    logger_ptr->error(fmt, std::forward<Args>(args)...);
                    break;
                case level::critical:
                    logger_ptr->critical(fmt, std::forward<Args>(args)...);
                    break;
                // case level::off:
                //     break;
                default:
                    break;
            }
        }
        else
        {
            std::cout << fmt::format(fmt, args...) << std::endl;
        }
    }

    template <typename... Args>
    void out(IN const level& log_level, IN const char* fmt, IN Args&&...args)
    {
        if (!g_logger_without_module)
        {
            std::cout << "check \"spdlogger.hpp\": [out] function: no spdlogger registered!" << std::endl;
            return;
        }
        switch (log_level)
        {
            case level::trace:
                g_logger_without_module->trace(fmt, std::forward<Args>(args)...);
                break;
            case level::debug:
                g_logger_without_module->debug(fmt, std::forward<Args>(args)...);
                break;
            case level::info:
                g_logger_without_module->info(fmt, std::forward<Args>(args)...);
                break;
            case level::warn:
                g_logger_without_module->warn(fmt, std::forward<Args>(args)...);
                break;
            case level::err:
                g_logger_without_module->error(fmt, std::forward<Args>(args)...);
                break;
            case level::critical:
                g_logger_without_module->critical(fmt, std::forward<Args>(args)...);
                break;
            // case level::off:
            //     break;
            default:
                break;
        }
    }

private:
    // 无模块时使用的logger
    std::shared_ptr<spdlog::logger> g_logger_without_module;
    // 多模块时使用的logger, 每个模块有一个logger, 这些logger共享一个sink
    std::map<std::string, std::shared_ptr<spdlog::logger>> g_loggers;

    // 控制台/文件输出
    std::vector<spdlog::sink_ptr> g_sinks;

    // 可以输出日志的模块名
    std::vector<std::string> g_module_names;

    /**
     * @brief 注册日志(多模块)
     * @details
     * @param [IN] const std::string& module_name, 模块名称
     * @return void
     */
    void RegisterLog(IN const std::string& module_name, IN const std::string& pattern)
    {
        if (g_loggers.find(module_name) == g_loggers.end())
        {
            auto logger = std::make_shared<spdlog::logger>(module_name, begin(g_sinks), end(g_sinks));
            spdlog::register_logger(logger);
            logger->set_level(slogger::level::trace); // 设定有效的阈值
            logger->flush_on(slogger::level::trace);  // 设定触发写入的消息阈值
            // 只要error或者更严重的日志被记录时就会触发flush
            if (pattern.empty())
            {
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v"); //默认打印完整时间信息 
            }
            else
            {
                logger->set_pattern(pattern);
            }
            g_loggers.emplace(module_name, logger);
        }
    }

    /**
     * @brief 注册日志(无模块)
     * @details
     * @param [IN] const std::string& module_name, 模块名称
     * @return void
     */
    void RegisterLogWithoutModule(IN const std::string& pattern)
    {
        g_logger_without_module = std::make_shared<spdlog::logger>("no_module", begin(g_sinks), end(g_sinks));
        spdlog::register_logger(g_logger_without_module);
        g_logger_without_module->set_level(slogger::level::trace); // 设定有效的阈值
        g_logger_without_module->flush_on(slogger::level::trace);  // 设定触发写入的消息阈值
        if (pattern.empty())
        {
            g_logger_without_module->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v"); //默认打印完整时间信息 
        }
        else
        {
            // 去掉模块相关信息
            auto pattern_temp = pattern;
            auto index_module = pattern.find("[%n] ");
            if (index_module != std::string::npos)
            {
                pattern_temp.erase(index_module, strlen("[%n] "));
            }
            g_logger_without_module->set_pattern(pattern_temp);
        }
    }

    /**
     * @brief 判断是否输出模块日志
     * @details
     * @param [IN] const std::string& module_name, 模块名称
     * @return bool, true:输出, false:不输出
     */
    bool IsRegisterd(IN const std::string& module_name)
    {
        if (g_module_names.empty())
        {
            return true;
        }
        return std::find(g_module_names.begin(), g_module_names.end(), module_name) != g_module_names.end();
    }

    /**
     * @brief 查找指定模块对应的logger
     * @details 如果找不到, 会为指定模块创建一个logger,
     *          采用rotating_logger_mt，即当超出规定大小时，会删除当前日志内容，重新开始写入
     * @param [IN] const std::string& module_name, 模块名字
     * @return std::shared_ptr<spdlog::logger>&
     */
    std::shared_ptr<spdlog::logger> GetLogger(IN const std::string& module_name)
    {
        auto it = g_loggers.find(module_name);

        //if (it == g_loggers.end())
        //{
        //    auto logger = std::make_shared<spdlog::logger>(module_name, begin(g_sinks), end(g_sinks));
        //    logger->set_level(logger::level::trace);
        //    logger->flush_on(logger::level::trace);
        //    g_loggers.emplace(module_name, logger);
        //    return logger;
        //}
        return it->second;
    }

    /**
     * @brief 清理日志
     * @details
     * @return void
     */
    void DestroyLog()
    {
        for (auto iter = g_loggers.begin(); iter != g_loggers.end();)
        {
            iter->second->flush();
            iter = g_loggers.erase(iter);
        }
        g_sinks.clear();
        g_module_names.clear();
    }

    //
    // int mkdirs(const char* Path)
    // {
    //     const int max_path_len = 500;
    //     char      FilePath[max_path_len];
    //     int       k = 0;
    //
    //     char szBuf[max_path_len];
    // #ifdef WIN32
    //     strcpy_s(szBuf, _countof(szBuf), Path);
    // #else
    //     strcpy(szBuf, Path);
    // #endif // WIN32
    //
    //     int i = static_cast<int>(strlen(szBuf));
    //     for (int j = 0; j < i; j++)
    //     {
    //         if (szBuf[j] == '\\' || szBuf[j] == '/')
    //         {
    //             k = j;
    //             // 在linux下，第一个匹配到目录/时，直接进入下一个循环，否则该函数直接返回-1
    //             if (k == 0)
    //             {
    //                 continue;
    //             }
    //             memset(FilePath, 0, max_path_len);
    // #ifdef WIN32
    //             strncpy_s(FilePath, _countof(FilePath), szBuf, k);
    // #else
    //             strncpy(FilePath, szBuf, k);
    // #endif // WIN32
    //             FilePath[k] = '\0';
    //
    // #ifdef WIN32
    //             if ((_access(FilePath, 0)) == -1)
    //             {
    //                 if (_mkdir(FilePath) == -1)
    //                     continue;
    //             }
    // #else
    //             if ((access(FilePath, 0)) == -1)
    //             {
    //                 if (mkdir(FilePath, S_IRUSR | S_IWUSR | S_IXUSR) == -1)
    //                     continue;
    //             }
    // #endif
    //         }
    //     }
    //
    //     if (k == 0)
    //     {
    //         return -1;
    //     }
    //
    //     return 0;
    // }
};

// 根据log等级输出对应描述信息
inline std::string GetLevelDescription(const slogger::level& log_level)
{
    switch (log_level)
    {
        case slogger::level::trace:
            return "0(trace)";
        case slogger::level::debug:
            return "1(debug)";
        case slogger::level::info:
            return "2(info)";
        case slogger::level::warn:
            return "3(warn)";
        case slogger::level::err:
            return "4(err)";
        case slogger::level::critical:
            return "5(critical)";
        case slogger::level::off:
            return "6(off)";
        default:
            return "undefined!";
    }
}

inline std::string GetLevelDescription(const int log_level)
{
    if (log_level >= slogger::level::trace && log_level <= slogger::level::off)
    {
        return GetLevelDescription(static_cast<slogger::level>(log_level));
    }
    return "undefined!";
}
} // namespace base

#endif // BASE_DEBUG_LOGGER_H
