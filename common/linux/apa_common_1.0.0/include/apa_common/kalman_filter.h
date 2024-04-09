// Copyright [2019] - MiniEye INC.
#pragma once

#include <string>
#include "Eigen/Dense"
#include "matrix_operations.h"

namespace apa {

/**
 * @class KalmanFilter
 * 
 * @brief Implements a discrete-time Kalman filter.
 * 
 * @param XN dimension of state
 * 
 * @param ZN dimension of observation
 * 
 * @param UN dimension of controls
 */

template <typename T, unsigned int XN, unsigned int ZN, unsigned int UN>
class KalmanFilter {
 public:
    /**
     * @brief Constructor which defers initialization until the initial state
     * distribution parameters are set( with SetStateEstimate)
     * typically on the first observation
     */
    KalmanFilter() {
       m_x.setZero();
       m_P.setZero();
       m_F.setIdentity();
       m_Q.setZero();
       m_H.setIdentity();
       m_R.setZero();
       m_B.setZero();
       m_y.setZero();
       m_S.setZero();
       m_K.setZero();
       m_is_initialized = false;
    }

    /**
     * @brief Sets the initial state belief distribution.
     * @param x mean of the state belief distribution.
     * @param p mean covariance of the state belief distribution.
     */
    void SetStateEstimate(const Eigen::Matrix<T, XN, 1> &x, const Eigen::Matrix<T, XN, XN> &p) {
       m_x = x;
       m_P = p;
       m_is_initialized = true;
    }

    KalmanFilter(const Eigen::Matrix<T, XN, 1> &x, const Eigen::Matrix<T, XN, XN> &p) : KalmanFilter() {
       SetStateEstimate(x, p);
    }

    virtual ~KalmanFilter();

    /**
     * @brief Changes the system transition function under zero control.
     * @param F new transition matrix.
     */
    void SetTransitionMatrix(const Eigen::Matrix<T, XN, XN> &F) {m_F = F;}

    void SetTransitionNoise(const Eigen::Matrix<T, XN, XN> &Q) {m_Q = Q;}

    void SetObservationMatrix(const Eigen::Matrix<T, ZN, XN> &H) {m_H = H;}

    void SetObservationNoise(const Eigen::Matrix<T, ZN, ZN> &R) {m_R = R;}

    void SetStateCovariance(const Eigen::Matrix<T, XN, XN> &P) {m_P = P;}

    void SetControlMatrix(const Eigen::Matrix<T, XN, UN> &B) {m_B = B;}

    const Eigen::Matrix<T, XN, XN> &GetTransitionMatrix() const {return m_F;}

    const Eigen::Matrix<T, XN, XN> &GetTransitionNoise() const {return m_Q;}

    const Eigen::Matrix<T, ZN, XN> &GetObservationMatrix() const {return m_H;}

    const Eigen::Matrix<T, ZN, ZN> &GetObservationNoise() const {return m_R;}

    const Eigen::Matrix<T, XN, XN> &GetStateCovariance() const {return m_P;}

    const Eigen::Matrix<T, XN, UN> &GetControlMatrix() const {return m_B;}

    const Eigen::Matrix<T, XN, 1> &GetStateEstimate() const {return m_x;}

    bool IsInitialized() const {return m_is_initialized;}

    /**
     * @brief Updates the state belief distribution given the control input u.
     * @param u Control input (by default, zero).
     */
    void Predict(const Eigen::Matrix<T, UN, 1> &u = Eigen::Matrix<T, UN, 1>::zero());

    /**
     * @brief Updates the state belief distribution given an observation z.
     * @param z Observation.
     */
    void Correct(const Eigen::Matrix<T, ZN, 1> &z);

    std::string DebugString() const;

 private:
    //  Mean of current state belief distribution
    Eigen::Matrix<T, XN, 1> m_x;

    //  Covariance of current state belief dist
    Eigen::Matrix<T, XN, XN> m_P;

    //  State transition matrix under zero control
    Eigen::Matrix<T, XN, XN> m_F;

    //  Covariance of state tansition noise
    Eigen::Matrix<T, XN, XN> m_Q;

    //  Observation matrix
    Eigen::Matrix<T, ZN, XN> m_H;

    //  Covariance of observation noise
    Eigen::Matrix<T, ZN, ZN> m_R;

    //  Control matrix in state transition rule
    Eigen::Matrix<T, XN, UN> m_B;

    //  Innovation; marked as menber to prevent memory re-allocation.
    Eigen::Matrix<T, ZN, 1> m_y;

    //  Innovation convariance; marked as menber to prevent memory re-allocation.
    Eigen::Matrix<T, ZN, ZN> m_S;

    //  Kalman gain; marked as menber to prevent memory re-allocation.
    Eigen::Matrix<T, XN, ZN> m_K;

    //  true iff SetStateEstimate has been called.
    bool m_is_initialized;

};

}  // namespace apa