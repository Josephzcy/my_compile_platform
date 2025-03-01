#include <vector>

#include "caffe/layers/conv_layer.hpp"
#include "caffe/util/im2col.hpp"
#include "ristretto/base_ristretto_layer.hpp"

namespace caffe {

template <typename Dtype>
void ConvolutionRistrettoLayer<Dtype>::Forward_gpu(
      const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  //std::cout<<"begin conv"<<std::endl;
  // Trim layer input
  //if (this->phase_ == TEST) {
    for (int i = 0; i < bottom.size(); ++i) {
      this->QuantizeLayerInputs_gpu(bottom[i]->mutable_gpu_data(),
          bottom[i]->count());
    }
  //}

  // Trim weights for quantization
  caffe_copy(this->blobs_[0]->count(), this->blobs_[0]->gpu_data(),
      this->weights_quantized_[0]->mutable_gpu_data());
  if (this->bias_term_) {
    caffe_copy(this->blobs_[1]->count(), this->blobs_[1]->gpu_data(),
        this->weights_quantized_[1]->mutable_gpu_data());
  }
  //cut smaller weights to zeros for sparsification
  this->CutWeights_cpu(this->weights_quantized_, this->sparse_ratio_);
  int rounding = this->phase_ == TEST ? this->rounding_ :
      QuantizationParameter_Rounding_STOCHASTIC;
  this->QuantizeWeights_gpu(this->weights_quantized_, rounding,
      this->bias_term_);
  // Do forward propagation
  const Dtype* weight = this->weights_quantized_[0]->gpu_data();
  for (int i = 0; i < bottom.size(); ++i) {
    const Dtype* bottom_data = bottom[i]->gpu_data();
    Dtype* top_data = top[i]->mutable_gpu_data();
    for (int n = 0; n < this->num_; ++n) {
      this->forward_gpu_gemm(bottom_data + n * this->bottom_dim_, weight,
          top_data + n * this->top_dim_);
      if (this->bias_term_) {
        const Dtype* bias = this->weights_quantized_[1]->gpu_data();
        this->forward_gpu_bias(top_data + n * this->top_dim_, bias);
      }
    }
    // Trim layer output
    //if (this->phase_ == TEST) {
      this->QuantizeLayerOutputs_gpu(top_data, top[i]->count());
    //}
  }
  //std::cout<<"end conv"<<std::endl;
}

template <typename Dtype>
void ConvolutionRistrettoLayer<Dtype>::Backward_gpu(
      const vector<Blob<Dtype>*>& top, const vector<bool>& propagate_down,
      const vector<Blob<Dtype>*>& bottom) {
  const Dtype* weight = this->weights_quantized_[0]->gpu_data();
  Dtype* weight_diff = this->blobs_[0]->mutable_gpu_diff();
  for (int i = 0; i < top.size(); ++i) {
    const Dtype* top_diff = top[i]->gpu_diff();
    // Bias gradient, if necessary.
    if (this->bias_term_ && this->param_propagate_down_[1]) {
      Dtype* bias_diff = this->blobs_[1]->mutable_gpu_diff();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_gpu_bias(bias_diff, top_diff + n * this->top_dim_);
      }
    }
    if (this->param_propagate_down_[0] || propagate_down[i]) {
      const Dtype* bottom_data = bottom[i]->gpu_data();
      Dtype* bottom_diff = bottom[i]->mutable_gpu_diff();
      for (int n = 0; n < this->num_; ++n) {
        // gradient w.r.t. weight. Note that we will accumulate diffs.
        if (this->param_propagate_down_[0]) {
          this->weight_gpu_gemm(bottom_data + n * this->bottom_dim_,
              top_diff + n * this->top_dim_, weight_diff);
        }
        // gradient w.r.t. bottom data, if necessary.
        if (propagate_down[i]) {
          this->backward_gpu_gemm(top_diff + n * this->top_dim_, weight,
              bottom_diff + n * this->bottom_dim_);
        }
      }
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(ConvolutionRistrettoLayer);

}  // namespace caffe
