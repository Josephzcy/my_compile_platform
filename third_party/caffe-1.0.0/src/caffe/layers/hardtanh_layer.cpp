#include <algorithm>
#include <vector>

#include "caffe/layers/hardtanh_layer.hpp"

namespace caffe {

template <typename Dtype>
void HardtanhLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  Dtype low_bound = this->layer_param_.hardtanh_param().low_bound();
  Dtype high_bound = this->layer_param_.hardtanh_param().high_bound();
  for (int i = 0; i < count; ++i) {
    top_data[i] =std::min(std::max(bottom_data[i], low_bound),high_bound);
  }
}

template <typename Dtype>
void HardtanhLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const int count = bottom[0]->count();
	Dtype low_bound = this->layer_param_.hardtanh_param().low_bound();
	Dtype high_bound = this->layer_param_.hardtanh_param().high_bound();
    for (int i = 0; i < count; ++i) {
      bottom_diff[i] = (top_diff[i] * (bottom_data[i] > low_bound)*(bottom_data[i] < high_bound));
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(HardtanhLayer);
#endif

INSTANTIATE_CLASS(HardtanhLayer);
REGISTER_LAYER_CLASS(Hardtanh);
}  // namespace caffe

