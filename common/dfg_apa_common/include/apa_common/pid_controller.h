// Copyright (C) 2019 - MiniEye INC

#pragma once
#include "apa_interface.h"
#include "gflags/gflags.h"

namespace apa {

struct PidConf {
    bool integrator_enable;
    double integrator_saturation;
    double kp;
    double ki;
    double kd;
    double kaw;
    double output_saturation;
};

class PIDController {
 public:
    PIDController();
/**
   * @brief initialize pid controller
   * @param pid_conf configuration for pid controller
   */
  void Init(const PidConf &pid_conf);

  /**
   * @brief set pid controller coefficients for the proportional,
   * integral, and derivative
   * @param pid_conf configuration for pid controller
   */
  void SetPID(const PidConf &pid_conf);

  /**
   * @brief reset variables for pid controller
   */
  void Reset();

  /**
   * @brief compute control value based on the error
   * @param error error value, the difference between
   * a desired value and a measured value
   * @param dt sampling time interval
   * @return control value based on PID terms
   */
  double Control(const double error, const double dt);

  ~PIDController() = default;

  /**
   * @brief get saturation status
   * @return saturation status
   */
  int IntegratorSaturationStatus() const;

  /**
   * @brief get status that if integrator is hold
   * @return if integrator is hold return true
   */
  bool IntegratorHold() const;

  /**
   * @brief set whether to hold integrator component at its current value.
   * @param hold
   */
  void SetIntegratorHold(bool hold);

 private:
  double m_kp;
  double m_ki;
  double m_kd;
  double m_kaw;
  double m_previous_error;
  double m_previous_output;
  double m_integral;
  double m_integrator_saturation_high;
  double m_integrator_saturation_low;
  bool m_first_hit;
  bool m_integrator_enabled;
  bool m_integrator_hold;
  int m_integrator_saturation_status;
  double m_output_saturation_high;
  double m_output_saturation_low;
  int m_output_saturation_status;
};

}  // namespace apa
