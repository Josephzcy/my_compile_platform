#include "caffe/layers/pixel_shuffle_layer.hpp"

namespace caffe {

using std::min;
using std::max;

template <typename Dtype>
void PixelShuffleLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
    PixelShuffleParameter pixel_shuffle_param = this->layer_param_.pixel_shuffle_param();
    upscale_factor_ = pixel_shuffle_param.upscale_factor();
    upscale_factor_sq_ = upscale_factor_ * upscale_factor_;
    CHECK(upscale_factor_) << "upscale factor must be specified for pixel shuffle layer!";
    CHECK_GT(upscale_factor_, 0) << "upscale factor must be positive!";
}

template <typename Dtype>
void PixelShuffleLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
    input_channels_ = bottom[0]->channels();
    input_height_ = bottom[0]->height();
    input_width_ = bottom[0]->width();
    CHECK_EQ(0, input_channels_ % upscale_factor_sq_)
            << "The number of input channels for pixel shuffle layer must be multiples "
            << "of the upscale factor!";
    output_channels_ = input_channels_ / upscale_factor_sq_;
    output_width_ = input_width_ * upscale_factor_;
    output_height_ = input_height_ * upscale_factor_;
    count_per_output_map_ = output_width_ * output_height_;
    count_per_input_map_ = input_width_ * input_height_;
    top[0]->Reshape(bottom[0]->num(), output_channels_, output_height_, output_width_);
}

template <typename Dtype>
void PixelShuffleLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    Dtype* top_data = top[0]->mutable_cpu_data();
    for (int n = 0; n < bottom[0]->num(); ++n) {
        for (int c = 0; c < output_channels_; ++c) {
            // for each output feature map
            for (int iy = 0, oy = 0; iy < input_height_; ++iy, oy += upscale_factor_) {
                for (int ix = 0, ox = 0; ix < input_width_; ++ix, ox += upscale_factor_) {
                    int input_channel_offset = 0;
                    for (int ty = 0; ty < upscale_factor_; ++ty) {
                        for (int tx = 0; tx < upscale_factor_; ++tx, input_channel_offset += count_per_input_map_) {
                            top_data[(oy + ty) * output_width_ + ox + tx] =
                                    bottom_data[input_channel_offset + iy * input_width_ + ix];
                        }
                    }
                }
            }
            bottom_data += count_per_output_map_;
            top_data += count_per_output_map_;
        }
    }
}

template <typename Dtype>
void PixelShuffleLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
    if (!propagate_down[0]) {
        return;
    }
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();

    for (int n = 0; n < bottom[0]->num(); ++n) {
        for (int c = 0; c < output_channels_; ++c) {
            for (int iy = 0, oy = 0; iy < input_height_; ++iy, oy += upscale_factor_) {
                for (int ix = 0, ox = 0; ix < input_width_; ++ix, ox += upscale_factor_) {
                    int input_channel_offset = 0;
                    for (int ty = 0; ty < upscale_factor_; ++ty) {
                        for (int tx = 0; tx < upscale_factor_; ++tx, input_channel_offset += count_per_input_map_) {
                            bottom_diff[input_channel_offset + iy * input_width_ + ix] =
                                    top_diff[(oy + ty) * output_width_ + ox + tx];
                        }
                    }
                }
            }
            bottom_diff += count_per_output_map_;
            top_diff += count_per_output_map_;
        }
    }
}

#ifdef CPU_ONLY
STUB_GPU(PixelShuffleLayer);
#endif


INSTANTIATE_CLASS(PixelShuffleLayer);
REGISTER_LAYER_CLASS(PixelShuffle);

}  // namespace caffe
