#include <vector>
#include <cfloat>
#include <algorithm>

#include "caffe/layers/sigmoid_focal_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {


template <typename Dtype>
__global__ void SigmoidFocalLossForwardGPU(const int nthreads, const Dtype* sigmoid_output_data,
          const Dtype* input_data, const Dtype* target, Dtype* loss,
          const bool has_ignore_label_, const int ignore_label_, const bool weight_by_label_freqs_,
          Dtype* counts, const float* label_count_data, const Dtype gamma_) {
  CUDA_KERNEL_LOOP(i, nthreads) {
    const int target_value = static_cast<int>(target[i]);
    if (has_ignore_label_ && target_value == ignore_label_) {
      loss[i] = 0;
      counts[i] = 0;
    } else {
      if(weight_by_label_freqs_){
        if(target_value==0){
          loss[i] = log(1 + exp(input_data[i])) * pow(sigmoid_output_data[i], gamma_)
                  * static_cast<Dtype>(label_count_data[target_value]);
        } else {
          loss[i] = (log(1 + exp(input_data[i])) - input_data[i]) * pow(1 - sigmoid_output_data[i], gamma_)
                  * static_cast<Dtype>(label_count_data[target_value]);
        }
      } else {
        if(target_value==0){
          loss[i] = log(1 + exp(input_data[i])) * pow(sigmoid_output_data[i], gamma_);
        } else {
          loss[i] = (log(1 + exp(input_data[i])) - input_data[i]) * pow(1 - sigmoid_output_data[i], gamma_);
        }
      }
      counts[i] = 1;
    }
  }
}

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::Forward_gpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  // The forward pass computes the sigmoid outputs.
  sigmoid_bottom_vec_[0] = bottom[0];
  sigmoid_layer_->Forward(sigmoid_bottom_vec_, sigmoid_top_vec_);
  const Dtype* sigmoid_output_data = sigmoid_output_->gpu_data();
  // get the class weight
  float* label_count_data=NULL;
  if(weight_by_label_freqs_){
    label_count_data = label_counts_.mutable_gpu_data();
  }
  // Compute the loss (negative log likelihood)
  const int count = bottom[0]->count();
  // Stable version of loss computation from input data
  const Dtype* input_data = bottom[0]->gpu_data();
  const Dtype* target = bottom[1]->gpu_data();
  // Since this memory is not used for anything until it is overwritten
  // on the backward pass, we use it here to avoid having to allocate new GPU
  // memory to accumulate intermediate results in the kernel.
  Dtype* loss_data = bottom[0]->mutable_gpu_diff();
  Dtype* count_data = bottom[1]->mutable_gpu_diff();
  Dtype valid_count=0;
  // NOLINT_NEXT_LINE(whitespace/operators)
  SigmoidFocalLossForwardGPU<Dtype><<<CAFFE_GET_BLOCKS(count),
      CAFFE_CUDA_NUM_THREADS>>>(count, sigmoid_output_data, input_data, target, loss_data,
      has_ignore_label_, ignore_label_, weight_by_label_freqs_, count_data, label_count_data, gamma_);
  // Only launch another CUDA kernel if we actually need the valid count.
  if (normalization_ == LossParameter_NormalizationMode_VALID &&
      has_ignore_label_) {
    caffe_gpu_asum(count, count_data, &valid_count);
    // std::cout<< "valid count " << valid_count<<std::endl;
  } else {
    valid_count = count;
  }
  Dtype loss;
  caffe_gpu_asum(count, loss_data, &loss);
  normalizer_ = get_normalizer(normalization_, valid_count);
  // std::cout<< "normalizer " << normalizer_<<std::endl;
  top[0]->mutable_cpu_data()[0] = loss / normalizer_;
}

template <typename Dtype>
__global__ void SigmoidFocalLossBackwardGPU(const int nthreads, const Dtype* sigmoid_output_data,
          const Dtype* target, Dtype* bottom_diff, const bool has_ignore_label_, const int ignore_label_,
          const bool weight_by_label_freqs_, const float* label_count_data, const Dtype gamma_) {
  CUDA_KERNEL_LOOP(i, nthreads) {
    const int target_value = static_cast<int>(target[i]);
    if (has_ignore_label_ && target_value == ignore_label_) {
      // Zero out gradient of ignored targets.
      bottom_diff[i] = 0;
      } else { 
        if (target_value==0) {
          if(weight_by_label_freqs_) {
            bottom_diff[i] = -pow(sigmoid_output_data[i], gamma_)
              *(gamma_*(1-sigmoid_output_data[i])*log(max(1-sigmoid_output_data[i], Dtype(FLT_MIN))) - sigmoid_output_data[i])
              * static_cast<Dtype>(label_count_data[target_value]);
          } else {
            bottom_diff[i] = -pow(sigmoid_output_data[i], gamma_)
              *(gamma_*(1-sigmoid_output_data[i])*log(max(1-sigmoid_output_data[i], Dtype(FLT_MIN))) - sigmoid_output_data[i]);
          }
      } else {
        if(weight_by_label_freqs_) {
          bottom_diff[i] = pow(1-sigmoid_output_data[i], gamma_)
            *(gamma_*sigmoid_output_data[i]*log(max(sigmoid_output_data[i], Dtype(FLT_MIN))) + sigmoid_output_data[i]-1)
            * static_cast<Dtype>(label_count_data[target_value]);
        } else {
          bottom_diff[i] = pow(1-sigmoid_output_data[i], gamma_)
            *(gamma_*sigmoid_output_data[i]*log(max(sigmoid_output_data[i], Dtype(FLT_MIN))) + sigmoid_output_data[i]-1);
        }
      }
    }
  }
}

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::Backward_gpu(
    const vector<Blob<Dtype>*>& top, const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    // get the class weight
    float* label_count_data=NULL;
    if(weight_by_label_freqs_){
      label_count_data = label_counts_.mutable_gpu_data();
    }
    // First, compute the diff
    const int count = bottom[0]->count();
    const Dtype* sigmoid_output_data = sigmoid_output_->gpu_data();
    const Dtype* target = bottom[1]->gpu_data();
    Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();
    SigmoidFocalLossBackwardGPU<Dtype><<<CAFFE_GET_BLOCKS(count),
        CAFFE_CUDA_NUM_THREADS>>>(count, sigmoid_output_data, target, bottom_diff,
      has_ignore_label_, ignore_label_, weight_by_label_freqs_, label_count_data, gamma_);
    // Scale down gradient
    Dtype loss_weight = top[0]->cpu_diff()[0] / normalizer_;
    caffe_gpu_scal(count, loss_weight, bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(SigmoidFocalLossLayer);

}  // namespace caffe
