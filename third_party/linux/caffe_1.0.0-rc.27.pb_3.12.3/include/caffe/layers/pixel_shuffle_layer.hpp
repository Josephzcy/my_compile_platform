#ifndef CAFFE_PIXEL_SHUFFLE_LAYERS_HPP_
#define CAFFE_PIXEL_SHUFFLE_LAYERS_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

namespace caffe {
/**
 * @brief for transforming a (r^2*C) x H x W blob into a
 * C x rH x rW blob where r is the upscale factor.
 *  Look at the paper:
 *  "Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convolutional Neural Network"
    by Shi et. al (2016) for more details
 */
template <typename Dtype>
class PixelShuffleLayer : public Layer<Dtype> {
 public:
  explicit PixelShuffleLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "PixelShuffle"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  int input_channels_, output_channels_;
  int input_width_, input_height_;
  int output_width_, output_height_;
  int count_per_input_map_, count_per_output_map_;
  int upscale_factor_, upscale_factor_sq_;
};

}  // namespace caffe

#endif  // CAFFE_PIXEL_SHUFFLE_LAYERS_HPP_
