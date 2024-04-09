#include "caffe/layers/pixel_shuffle_layer.hpp"

namespace caffe {

// TODO: Implement GPU version of pixel shuffle.
template <typename Dtype>
void PixelShuffleLayer<Dtype>::Forward_gpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  Forward_cpu(bottom, top);
}

template <typename Dtype>
void PixelShuffleLayer<Dtype>::Backward_gpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  Backward_cpu(top, propagate_down, bottom);
}

INSTANTIATE_LAYER_GPU_FUNCS(PixelShuffleLayer);

}  // namespace caffe
