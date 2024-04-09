// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_SIGMOID_MAP_H_
#define INCLUDE_OBJ_CNN_SIGMOID_MAP_H_

#include <cstddef>
#include <cmath>

#include <vector>

namespace obj_cnn {

struct SigmoidMapOption {
  // If x > bound, treat sigmoid(x) as 1, if x < -bound, treat sigmoid(x)
  // as 0.
  size_t bound;

  // Calculate scale_size points in every float range 1.
  size_t scale_size;

  // sigmoid(7)  = 0.9990889488055994
  // sigmoid(-7) = 0.0009110511944006454
  SigmoidMapOption() : bound(7), scale_size(1024) {}
};

// def sigmoid(x):
//    return 1.0 / (1.0 + np.exp(-x))
class SigmoidMap {
 public:
  explicit SigmoidMap(SigmoidMapOption option = SigmoidMapOption{})
      : option_(option), sigmoid_map_(option.bound * option.scale_size * 2) {
    float scale_size = option_.scale_size;
    half_ = option_.bound * option_.scale_size;
    int n = sigmoid_map_.size();

    for (int i = 0; i < n; i++) {
      float nx = (i - half_) / scale_size;  // -x

      sigmoid_map_[i] = 1.0f / (1.0f + std::exp(nx));
    }
  }

  size_t chunk_size() const { return sigmoid_map_.size(); }

  float FastSigmoid(float x) const {
    float scale_size = option_.scale_size;

    auto i = static_cast<int>(std::lround(-x * scale_size)) + half_;
    if (i < 0) return 1.F;
    if (i >= static_cast<int>(sigmoid_map_.size())) return 0.F;
    return sigmoid_map_[i];
  }

 private:
  SigmoidMapOption option_;
  std::vector<float> sigmoid_map_;
  int half_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_SIGMOID_MAP_H_
