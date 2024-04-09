#include <algorithm>
#include <vector>
#include <cfloat>

#include "caffe/layers/sigmoid_focal_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::LayerSetUp(bottom, top);
  sigmoid_bottom_vec_.clear();
  sigmoid_bottom_vec_.push_back(bottom[0]);
  sigmoid_top_vec_.clear();
  sigmoid_top_vec_.push_back(sigmoid_output_.get());
  sigmoid_layer_->SetUp(sigmoid_bottom_vec_, sigmoid_top_vec_);

  has_ignore_label_ =
    this->layer_param_.loss_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.loss_param().ignore_label();
  }
  if (this->layer_param_.loss_param().has_normalization()) {
    normalization_ = this->layer_param_.loss_param().normalization();
  } else if (this->layer_param_.loss_param().has_normalize()) {
    normalization_ = this->layer_param_.loss_param().normalize() ?
                     LossParameter_NormalizationMode_VALID :
                     LossParameter_NormalizationMode_BATCH_SIZE;
  } else {
    normalization_ = LossParameter_NormalizationMode_BATCH_SIZE;
  }
  // Read class weights if possible
  weight_by_label_freqs_ =
    this->layer_param_.loss_param().weight_by_label_freqs();
  
  if (weight_by_label_freqs_) {
    CHECK_EQ(this->layer_param_.loss_param().class_weighting_size(), 2)
		<< "Number of class weight values should be 2 for binary classification task.";
    vector<int> count_shape(1, this->layer_param_.loss_param().class_weighting_size());
    label_counts_.Reshape(count_shape);
    float* label_count_data = label_counts_.mutable_cpu_data();
    for (int i = 0; i < this->layer_param_.loss_param().class_weighting_size(); i++) {
        label_count_data[i] = this->layer_param_.loss_param().class_weighting(i);
    }
  }
  // Read the hyper-parameter gamma
  gamma_ = this->layer_param_.loss_param().gamma();
}

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::Reshape(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::Reshape(bottom, top);
  outer_num_ = bottom[0]->shape(0);  // batch size
  inner_num_ = bottom[0]->count(1);  // instance size: |output| == |target|
  CHECK_EQ(bottom[0]->count(), bottom[1]->count()) <<
      "SIGMOID_FOCAL_LOSS layer inputs must have the same count.";
  sigmoid_layer_->Reshape(sigmoid_bottom_vec_, sigmoid_top_vec_);
}

// TODO(shelhamer) loss normalization should be pulled up into LossLayer,
// instead of duplicated here and in SoftMaxWithLossLayer
template <typename Dtype>
Dtype SigmoidFocalLossLayer<Dtype>::get_normalizer(
    LossParameter_NormalizationMode normalization_mode, int valid_count) {
  Dtype normalizer;
  switch (normalization_mode) {
    case LossParameter_NormalizationMode_FULL:
      normalizer = Dtype(outer_num_ * inner_num_);
      break;
    case LossParameter_NormalizationMode_VALID:
      if (valid_count == -1) {
        normalizer = Dtype(outer_num_ * inner_num_);
      } else {
        normalizer = Dtype(valid_count);
      }
      break;
    case LossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = Dtype(outer_num_);
      break;
    case LossParameter_NormalizationMode_NONE:
      normalizer = Dtype(1);
      break;
    default:
      LOG(FATAL) << "Unknown normalization mode: "
          << LossParameter_NormalizationMode_Name(normalization_mode);
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return std::max(Dtype(1.0), normalizer);
}

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  // The forward pass computes the sigmoid outputs.
  sigmoid_bottom_vec_[0] = bottom[0];
  sigmoid_layer_->Forward(sigmoid_bottom_vec_, sigmoid_top_vec_);
  const Dtype* sigmoid_output_data = sigmoid_output_->cpu_data();
  // Compute the focal loss (negative log likelihood)
  // Stable version of loss computation from input data
  const Dtype* input_data = bottom[0]->cpu_data();
  const Dtype* target = bottom[1]->cpu_data();
  int valid_count = 0;
  Dtype loss = 0;
  float* label_count_data=NULL;
  if(weight_by_label_freqs_){
    label_count_data = label_counts_.mutable_cpu_data();
  }
  for (int i = 0; i < bottom[0]->count(); ++i) {
    const int target_value = static_cast<int>(target[i]);
    if (has_ignore_label_ && target_value == ignore_label_) {
      continue;
    }
    if(weight_by_label_freqs_){
      if(target[i]==0){
        loss += log(1 + exp(input_data[i])) * pow(sigmoid_output_data[i], gamma_)
        * static_cast<Dtype>(label_count_data[target_value]);
      } else {
        loss -= (input_data[i] - log(exp(input_data[i])+1)) * pow(1 - sigmoid_output_data[i], gamma_)
                * static_cast<Dtype>(label_count_data[target_value]);
      }
    }
    else {
      if(target[i]==0){
        loss += log(1 + exp(input_data[i])) * pow(sigmoid_output_data[i], gamma_);
      } else {
        loss -= (input_data[i] - log(exp(input_data[i])+1)) * pow(1 - sigmoid_output_data[i], gamma_);
      }
    }
    ++valid_count;
  }
  normalizer_ = get_normalizer(normalization_, valid_count);
  top[0]->mutable_cpu_data()[0] = loss / normalizer_;
}

template <typename Dtype>
void SigmoidFocalLossLayer<Dtype>::Backward_cpu(
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
      label_count_data = label_counts_.mutable_cpu_data();
    }
    // First, compute the diff
    const int count = bottom[0]->count();
    const Dtype* sigmoid_output_data = sigmoid_output_->cpu_data();
    const Dtype* target = bottom[1]->cpu_data();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    // calculating gradient
    for (int i = 0; i < count; ++i) {
      const int target_value = static_cast<int>(target[i]);
      if (has_ignore_label_ && target_value == ignore_label_) {
        // Zero out gradient of ignored targets.
        bottom_diff[i] = 0;
      } else { 
        if (target_value==0) {
          if(weight_by_label_freqs_) {
            bottom_diff[i] = -pow(sigmoid_output_data[i], gamma_)
              *(gamma_*(1-sigmoid_output_data[i])*log(std::max(1-sigmoid_output_data[i], Dtype(FLT_MIN))) - sigmoid_output_data[i])
              * static_cast<Dtype>(label_count_data[target_value]);
          } else {
            bottom_diff[i] = -pow(sigmoid_output_data[i], gamma_)
              *(gamma_*(1-sigmoid_output_data[i])*log(std::max(1-sigmoid_output_data[i], Dtype(FLT_MIN))) - sigmoid_output_data[i]);
          }
        } else {
          if(weight_by_label_freqs_) {
            bottom_diff[i] = pow(1-sigmoid_output_data[i], gamma_)
              *(gamma_*sigmoid_output_data[i]*log(std::max(sigmoid_output_data[i], Dtype(FLT_MIN))) + sigmoid_output_data[i]-1)
              * static_cast<Dtype>(label_count_data[target_value]);
          } else {
            bottom_diff[i] = pow(1-sigmoid_output_data[i], gamma_)
              *(gamma_*sigmoid_output_data[i]*log(std::max(sigmoid_output_data[i], Dtype(FLT_MIN))) + sigmoid_output_data[i]-1);
          }
        }
      }
    }
    // Scale down gradient
    Dtype loss_weight = top[0]->cpu_diff()[0] / normalizer_;
    caffe_scal(count, loss_weight, bottom_diff);
  }
}

#ifdef CPU_ONLY
STUB_GPU(SigmoidFocalLossLayer);
#endif

INSTANTIATE_CLASS(SigmoidFocalLossLayer);
REGISTER_LAYER_CLASS(SigmoidFocalLoss);

}  // namespace caffe
