# common_repo
==================================================
### base
--------------------------------------------------
##### 该目录主要是一些基础的数据结构和为了跨平台的一些适配性的文件。
        any.h -> 任意对象（可以把任意对象付给any对象）
        array_size.h -> 数组size
        check_error.h -> 对error的一些处理(支持win32和linux)
        closure.h -> 函数闭包
        common_windows.h -> windows环境依赖的头文件
        cxx11.h -> 定义是否enable cxx11
        log_level.h -> 定义了glog的一些level： CORE_LOG (核心log) SUBMODULE_LOG (子模块log) DEBUG_LOG(调试LOG)
        platform_feature.h -> 平台相关的一些特性 
        preprocess.h -> 定义了一下预处理常用的宏
        ref_counted.h -> 引用计数
        scoped_ptr.h -> 作用域指针
        singleton.h -> 单例模式
        static_assert.h -> debug 和 release模式下都可以使用的assert
        stdint.h -> 跨平台整形类型头文件
        uncopyable.h -> 申明不可拷贝类型 
### concurrency
-----------------------------------------------------
##### 并发相关
        base_thread -> 基础线程类 （win32 和 linux通用）
        condition_variable -> 条件变量
        mutex -> 互斥锁
        parrallel -> 并发类
        rwlock -> 读写锁
        scoped_locker -> 作用域锁
        spinlock -> 自旋锁
        thread -> 线程类
        thread_pool -> 线程池
        work_thread -> 基于thread封装的工作线程（处理对象是一个个的函数闭包）
### encoding
----------------------------------------------------------
##### 编码相关

### glimshow
-----------------------------------------------------------
##### 利用OpenGL show image

### gps
-----------------------------------------------------------
##### 从gps模块读gps信息
        Windows下，读取的是COM3的端口，调试的时候注意下，如果不对就换下USB插口，或者更改下代码再编译
        m_serial = new serial::Serial("COM3", 38400, serial::Timeout::simpleTimeout(10000));

### image_io
-----------------------------------------------------------
##### image的读取（包括从capture device 和本地读取两种模式）

### memory
-----------------------------------------------------------
##### 内存相关 （目前只有内存池）

### string
-----------------------------------------------------------
##### 所有字符串相关的操作
        algorithm -> 字符串是不是以xxxx打头或者结尾 \ 
                     移除xxx后缀 \
                     剔除连续空格 \
                     合并字符串 \
                     分割字符串 \
                     trim字符串 \
                     移除子串 \
                     将二进制buffer转化成十六进制串 \
                     转大/小写 \ 
                     etc
        string_number -> 字符串转数字 、数字转字符串
        string_piece -> 可以当string使用，需要自己维护内存，提高运行效率
### time
----------------------------------------------------------------------
##### 所有跟时间相关的封装
       stopwatch -> 用以统计时间 (win / linux 通用)
       time_utils -> 当前时间(毫秒、微秒、GMT时间) 
                     
### ui
----------------------------------------------------------------------
##### 所有跟UI相关的操作
       drive_ui -> 机动车检测的UI
       lane_ui -> 车道检测的UI
                 
### can
----------------------------------------------------------------------
##### CAN总线通信相关
       can_dbc -> dbc parser 文件读取
       can_dbc_test -> can_dbc的样例程序
       can_io -> can总线分析仪通讯的sdk
       can_io_tool -> can总线分析仪通讯demo

### config
----------------------------------------------------------------------
##### 配置相关
       xml_configer -> 解析xml文件 

### math
----------------------------------------------------------------------
##### 所有跟数学相关的运算 
       polyfit -> 曲线拟合

### timer
----------------------------------------------------------------------
##### 定时触发相关 
       timer_manager -> 提供定时触发接口，可提供一次性(周期性)触发任务