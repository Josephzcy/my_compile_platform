#include <sstream>
#include "kalman_filter.h"
#include "glog/logging.h"

namespace apa {

template <typename T, unsigned int XN, unsigned int ZN, unsigned int UN>
void KalmanFilter<T, XN, ZN, UN>::Predict(const Eigen::Matrix<T, UN, 1> &u) {
    CHECK(m_is_initialized);
    m_x = m_F * m_x + m_B * u;
    m_P = m_F * m_P * m_F.transpose() + m_Q;
}

template <typename T, unsigned int XN, unsigned int ZN, unsigned int UN>
void KalmanFilter<T, XN, ZN, UN>::Correct(const Eigen::Matrix<T, ZN, 1> &z) {
    CHECK(m_is_initialized);
    m_y = z - m_H * m_x;
    m_S = static_cast<Eigen::Matrix<T, ZN, ZN>>(m_H * m_P * m_H.transpose() + m_R);
    m_K = static_cast<Eigen::Matrix<T, XN, ZN>>(m_P * m_H.transpose() * PseudoInverse<T, ZN>(m_S));
    m_x = m_x + m_K * m_y;
    m_P = static_cast<Eigen::Matrix<T, XN, XN>>((Eigen::Matrix<T, XN, XN>::Identity() - m_K * m_H) * m_P);
}

template <typename T, unsigned int XN, unsigned int ZN, unsigned int UN>
std::string KalmanFilter<T, XN, ZN, UN>::DebugString() const {
    Eigen::IOFormat clean_fmt(4, 0, ",", " ", "[", "]");
    std::stringstream ss;
    ss << "F = " << m_F.format(clean_fmt) << "\n"
       << "B = " << m_B.format(clean_fmt) << "\n"
       << "H = " << m_H.format(clean_fmt) << "\n"
       << "Q = " << m_Q.format(clean_fmt) << "\n"
       << "R = " << m_R.format(clean_fmt) << "\n"
       << "x = " << m_x.format(clean_fmt) << "\n"
       << "P = " << m_P.format(clean_fmt) << "\n";
    return ss.str();
}

}  // namespace apa