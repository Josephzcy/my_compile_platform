#include <algorithm>
#include <vector>

#include "caffe/layers/hardtanh_layer.hpp"

namespace caffe {

template <typename Dtype>
__global__ void HardtanhForward(const int n, const Dtype* in, Dtype* out,
    Dtype low_bound, Dtype high_bound) {
  CUDA_KERNEL_LOOP(index, n) {
    out[index] = in[index] > low_bound ? in[index] : low_bound;
    out[index] = out[index] < high_bound ? out[index] : high_bound;
  }
}

template <typename Dtype>
void HardtanhLayer<Dtype>::Forward_gpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->gpu_data();
  Dtype* top_data = top[0]->mutable_gpu_data();
  const int count = bottom[0]->count();
  Dtype low_bound = this->layer_param_.hardtanh_param().low_bound();
  Dtype high_bound = this->layer_param_.hardtanh_param().high_bound();
  // NOLINT_NEXT_LINE(whitespace/operators)
  HardtanhForward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
      count, bottom_data, top_data, low_bound, high_bound);
  CUDA_POST_KERNEL_CHECK;
  // << " count: " << count << " bottom_data: "
  //     << (unsigned long)bottom_data
  //     << " top_data: " << (unsigned long)top_data
  //     << " blocks: " << CAFFE_GET_BLOCKS(count)
  //     << " threads: " << CAFFE_CUDA_NUM_THREADS;
}

template <typename Dtype>
__global__ void HardtanhBackward(const int n, const Dtype* in_diff,
    const Dtype* in_data, Dtype* out_diff, Dtype low_bound, Dtype high_bound) {
  CUDA_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * (in_data[index] > low_bound) * (in_data[index] < high_bound);
  }
}

template <typename Dtype>
void HardtanhLayer<Dtype>::Backward_gpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* bottom_data = bottom[0]->gpu_data();
    const Dtype* top_diff = top[0]->gpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();
    const int count = bottom[0]->count();
	Dtype low_bound = this->layer_param_.hardtanh_param().low_bound();
	Dtype high_bound = this->layer_param_.hardtanh_param().high_bound();
    // NOLINT_NEXT_LINE(whitespace/operators)
    HardtanhBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
        count, top_diff, bottom_data, bottom_diff, low_bound, high_bound);
    CUDA_POST_KERNEL_CHECK;
  }
}


INSTANTIATE_LAYER_GPU_FUNCS(HardtanhLayer);


}  // namespace caffe

