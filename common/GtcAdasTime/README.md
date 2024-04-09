# GtcAdasTime

新增工控机平台数据面时间戳接口lib库 gtctimer，包括如下接口，
详见 GtcAdasTime.h
使用范例参考 ./test/test.cc
需要包含头文件 #include "GtcAdasTime.h"
链接时链接此lib库 gtctimer


/**  获取数据面时间，单位us
    @param[in]  void
    @param[out] none
    @return     uint64_t 数据面时间，单位us
    @warning    无
    @note       数据面时间：数据面时间系统依赖板载晶振自运行。不存在时间跳变，用于多传感器数据融合、底盘控制等场景；
*/
uint64_t GetAdsTimeInUsecs(void);

/**  将数据面时间戳tick_us转换为管理面时间戳，单位us
    @param[in]  tick_us 数据面时间戳，单位us
    @param[out] none
    @return     uint64_t 管理面时间，单位us
    @warning    无
    @note       无
*/
uint64_t tick_to_utc_time_in_us(uint64_t tick_us);

/**  获取管理面时间，单位us
    @param[in]  void
    @param[out] none
    @return     uint64_t 管理面时间，单位us
    @warning    无
    @note       管理面时间：管理面时间系统对齐UTC时间，会存在时间跳变的情况，一般用于时间显示和记录
*/
uint64_t get_utc_time_in_us(void);