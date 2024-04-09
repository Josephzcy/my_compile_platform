//
// Created by mini on 2022/7/14.
//

#ifndef AIPLORER_WRAPPERDIAG_H
#define AIPLORER_WRAPPERDIAG_H

#include <cstdint>

class CWrapperDiag {
public:
  static CWrapperDiag &instance();
  ~CWrapperDiag();

  static bool is_camera_front_valid();
  static bool is_camera_rear_valid();
  static bool is_camera_left_valid();
  static bool is_camera_right_valid();

  /*
  @brief: 上报错误
  @param component：故障发生组件
  @param function：故障发生功能
  @param error_code：故障码
  */
  static int report_error(uint32_t component, uint32_t function, uint32_t error_code);

  /*
  @brief: 错误恢复
  @param component：故障发生组件
  @param function：故障发生功能
  @param error_code：故障码
  */
  static int error_recover(uint32_t component, uint32_t function, uint32_t error_code);

private:
  CWrapperDiag();
  CWrapperDiag(const CWrapperDiag &) = delete;
  const CWrapperDiag &operator=(const CWrapperDiag &) = delete;
};

#endif // AIPLORER_WRAPPERDIAG_H
