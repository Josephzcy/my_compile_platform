// Copyright [2022] - MiniEye INC.
#ifndef BEV_SRC_WARPPER_DIAG_H_
#define BEV_SRC_WARPPER_DIAG_H_

#include <cstdint>

#include "diag_code.h"

class CWrapperDiag {
 public:
  static CWrapperDiag &instance();

  ~CWrapperDiag();

  /**
@brief: 上报错误
@param component：故障发生组件
@param function：故障发生功能
@param error_code：故障码
*/
  static int ReportError(uint32_t component, uint32_t function, uint32_t error_code);

  /**
@brief: 错误恢复
@param component：故障发生组件
@param function：故障发生功能
@param error_code：故障码
*/
  static int ErrorRecover(uint32_t component, uint32_t function, uint32_t error_code);

 private:
  static CWrapperDiag instance_;
  CWrapperDiag();
  CWrapperDiag(const CWrapperDiag &) = delete;
  const CWrapperDiag &operator=(const CWrapperDiag &) = delete;
};

#endif  // BEV_SRC_WARPPER_DIAG_H_
