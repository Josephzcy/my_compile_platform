// Copyright (C) 2019 - MiniEye INC
#include <cmath>
#include "pid_controller.h"
#include "glog/logging.h"

namespace apa {

PIDController::PIDController() {
  m_kp = 0.0;
  m_ki = 0.0;
  m_kd = 0.0;
  m_kaw = 0.0;
  m_previous_error = 0.0;
  m_previous_output = 0.0;
  m_integral = 0.0;
  m_integrator_saturation_high = 0.0;
  m_integrator_saturation_low = 0.0;
  m_first_hit = false;
  m_integrator_enabled = false;
  m_integrator_hold = false;
  m_integrator_saturation_status = 0;
  m_output_saturation_high = 0.0;
  m_output_saturation_low = 0.0;
  m_output_saturation_status = 0;
}

void PIDController::Init(const PidConf &pid_conf) {
  m_previous_error = 0.0;
  m_previous_output = 0.0;
  m_integral = 0.0;
  m_first_hit = true;
  m_integrator_enabled = pid_conf.integrator_enable;
  m_integrator_saturation_high =
      std::fabs(pid_conf.integrator_saturation);
  m_integrator_saturation_low =
      -std::fabs(pid_conf.integrator_saturation);
  m_integrator_saturation_status = 0;
  m_integrator_hold = false;
  m_output_saturation_high = std::fabs(pid_conf.output_saturation);
  m_output_saturation_low = -std::fabs(pid_conf.output_saturation);
  m_output_saturation_status = 0;
  SetPID(pid_conf);
}

void PIDController::SetPID(const PidConf &pid_conf) {
  m_kp = pid_conf.kp;
  m_ki = pid_conf.ki;
  m_kd = pid_conf.kd;
  m_kaw = pid_conf.kaw;
}

void PIDController::Reset() {
  m_previous_error = 0.0;
  m_previous_output = 0.0;
  m_integral = 0.0;
  m_first_hit = true;
  m_integrator_saturation_status = 0;
  m_output_saturation_status = 0;
}

double PIDController::Control(const double error, const double dt) {
  if (dt <= 0) {
    LOG(ERROR) << "dt <= 0, will use the last output, dt: " << dt;
    return m_previous_output;
  }
  double diff = 0;
  double output = 0;

  if (m_first_hit) {
    m_first_hit = false;
  } else {
    diff = (error - m_previous_error) / dt;
  }
  // integral hold
  if (!m_integrator_enabled) {
    m_integral = 0;
  } else if (!m_integrator_hold) {
    m_integral += error * dt * m_ki;
    // apply Ki before integrating to avoid steps when change Ki at steady state
    if (m_integral > m_integrator_saturation_high) {
      m_integral = m_integrator_saturation_high;
      m_integrator_saturation_status = 1;
    } else if (m_integral < m_integrator_saturation_low) {
      m_integral = m_integrator_saturation_low;
      m_integrator_saturation_status = -1;
    } else {
      m_integrator_saturation_status = 0;
    }
  }
  m_previous_error = error;
  output = error * m_kp + m_integral + diff * m_kd;  // Ki already applied
  m_previous_output = output;
  return output;
}

int PIDController::IntegratorSaturationStatus() const {
  return m_integrator_saturation_status;
}

bool PIDController::IntegratorHold() const { return m_integrator_hold; }

void PIDController::SetIntegratorHold(bool hold) { m_integrator_hold = hold; }

}  // namespace apa
