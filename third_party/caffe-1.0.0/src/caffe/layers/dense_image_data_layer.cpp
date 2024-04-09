#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>  // NOLINT(readability/streams)
#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <vector>

#include "caffe/data_transformer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/layers/dense_image_data_layer.hpp"
#include "caffe/util/benchmark.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/math_functions.hpp"
#include "caffe/util/rng.hpp"

namespace caffe {

template <typename Dtype>
DenseImageDataLayer<Dtype>::~DenseImageDataLayer<Dtype>() {
  this->StopInternalThread();
}

template <typename Dtype>
void DenseImageDataLayer<Dtype>::DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const int new_height = this->layer_param_.dense_image_data_param().new_height();
  const int new_width  = this->layer_param_.dense_image_data_param().new_width();
  const int crop_height = this->layer_param_.dense_image_data_param().crop_height();
  const int crop_width  = this->layer_param_.dense_image_data_param().crop_width();
  const bool is_color  = this->layer_param_.dense_image_data_param().is_color();
  const int label_divide_factor = this->layer_param_.dense_image_data_param().label_divide_factor();
  string root_folder_image = this->layer_param_.dense_image_data_param().root_folder_image();
  string root_folder_label = this->layer_param_.dense_image_data_param().root_folder_label();

  CHECK((new_height == 0 && new_width == 0) ||
      (new_height > 0 && new_width > 0)) << "Current implementation requires "
      "new_height and new_width to be set at the same time.";
  CHECK((crop_height == 0 && crop_width == 0) ||
      (crop_height > 0 && crop_width > 0)) << "Current implementation requires "
      "crop_height and crop_width to be set at the same time.";
  // Read the file with filenames and labels
  const string& source = this->layer_param_.dense_image_data_param().source();
  LOG(INFO) << "Opening file " << source;
  std::ifstream infile(source.c_str());
  string filename;
  string filename2;
  string label_filename;
  string label_filename2;
  while (infile >> filename >> label_filename) {
    lines_.push_back(std::make_pair(filename, label_filename));
  }
  CHECK(!lines_.empty()) << "File is empty";

  if (this->layer_param_.dense_image_data_param().shuffle()) {
    // randomly shuffle data
    LOG(INFO) << "Shuffling data";
    const unsigned int prefetch_rng_seed = caffe_rng_rand();
    prefetch_rng_.reset(new Caffe::RNG(prefetch_rng_seed));
    ShuffleImages();
  } else {
    if (this->phase_ == TRAIN && Caffe::solver_rank() > 0 &&
        this->layer_param_.dense_image_data_param().rand_skip() == 0) {
      LOG(WARNING) << "Shuffling or skipping recommended for multi-GPU";
    }
  }
  LOG(INFO) << "A total of " << lines_.size() << " images.";

  lines_id_ = 0;
  // Check if we would need to randomly skip a few data points
  if (this->layer_param_.dense_image_data_param().rand_skip()) {
    unsigned int skip = caffe_rng_rand() %
        this->layer_param_.dense_image_data_param().rand_skip();
    LOG(INFO) << "Skipping first " << skip << " data points.";
    CHECK_GT(lines_.size(), skip) << "Not enough points to skip";
    lines_id_ = skip;
  }
  // Read an image, and use it to initialize the top blob.
  cv::Mat cv_img = ReadImageToCVMat(root_folder_image + lines_[lines_id_].first,
                                    new_height, new_width, is_color, CV_INTER_LINEAR);
  CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
  const int height = cv_img.rows;
  const int width  = cv_img.cols;
  const int channels = cv_img.channels();
  // Read an label image, and use it to check label format and image consistent
  cv::Mat cv_lab = ReadImageToCVMat(root_folder_label + lines_[lines_id_].second,
                                    new_height/label_divide_factor, new_width/label_divide_factor, 
                                    false, CV_INTER_NN);
  CHECK(cv_lab.channels() == 1) << "Can only handle grayscale label images";
  // CHECK(cv_lab.rows == height && cv_lab.cols == width) << "Input and label "
  //    << "image heights and widths must match!";

  // Use data_transformer to infer the expected blob shape from a cv_image.
  // vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
  // Do crop outside the transformer
  vector<int> top_shape_data(4);
  vector<int> top_shape_label(4);
  if (crop_height>0 && crop_width>0) {
    top_shape_data[0] = 1;
    top_shape_data[1] = channels;
    top_shape_data[2] = crop_height;
    top_shape_data[3] = crop_width;
    top_shape_label[0] = 1;
    top_shape_label[1] = 1;
    top_shape_label[2] = crop_height;
    top_shape_label[3] = crop_width;
  } else {
    top_shape_data[0] = 1;
    top_shape_data[1] = channels;
    top_shape_data[2] = height;
    top_shape_data[3] = width;
    top_shape_label[0] = 1;
    top_shape_label[1] = 1;
    top_shape_label[2] = height/label_divide_factor;
    top_shape_label[3] = width/label_divide_factor;
  }
  this->transformed_data_.Reshape(top_shape_data);
  this->transformed_label_.Reshape(top_shape_label);
  // Reshape prefetch_data and top[0] according to the batch_size.
  const int batch_size = this->layer_param_.dense_image_data_param().batch_size();
  CHECK_GT(batch_size, 0) << "Positive batch size required";
  top_shape_data[0] = batch_size;
  top_shape_label[0] = batch_size;
  for (int i = 0; i < this->prefetch_.size(); ++i) {
    this->prefetch_[i]->data_.Reshape(top_shape_data);
    this->prefetch_[i]->label_.Reshape(top_shape_label);
  }
  top[0]->Reshape(top_shape_data);
  top[1]->Reshape(top_shape_label);

  LOG(INFO) << "output data size: " << top[0]->num() << ","
      << top[0]->channels() << "," << top[0]->height() << ","
      << top[0]->width();
  // label
  LOG(INFO) << "output label size: " << top[1]->num() << ","
      << top[1]->channels() << "," << top[1]->height() << ","
      << top[1]->width();
}

template <typename Dtype>
void DenseImageDataLayer<Dtype>::ShuffleImages() {
  caffe::rng_t* prefetch_rng =
      static_cast<caffe::rng_t*>(prefetch_rng_->generator());
  shuffle(lines_.begin(), lines_.end(), prefetch_rng);
}

// This function is called on prefetch thread
template <typename Dtype>
void DenseImageDataLayer<Dtype>::load_batch(Batch<Dtype>* batch) {
  CPUTimer batch_timer;
  batch_timer.Start();
  double read_time = 0;
  double trans_time = 0;
  CPUTimer timer;
  CHECK(batch->data_.count());
  CHECK(this->transformed_data_.count());
  DenseImageDataParameter dense_image_data_param = this->layer_param_.dense_image_data_param();
  const int batch_size = dense_image_data_param.batch_size();
  const int new_height = dense_image_data_param.new_height();
  const int new_width = dense_image_data_param.new_width();
  const bool is_color = dense_image_data_param.is_color();
  const string root_folder_image = dense_image_data_param.root_folder_image();
  const string root_folder_label = dense_image_data_param.root_folder_label();
  const int crop_height = dense_image_data_param.crop_height();
  const int crop_width  = dense_image_data_param.crop_width();
  const int label_divide_factor = dense_image_data_param.label_divide_factor();
  
  // Reshape according to the first image of each batch
  // on single input batches allows for inputs of varying dimension.
  cv::Mat cv_img = ReadImageToCVMat(root_folder_image + lines_[lines_id_].first,
      new_height, new_width, is_color, CV_INTER_LINEAR);
  CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
  const int height = cv_img.rows;
  const int width  = cv_img.cols;
  const int channels = cv_img.channels();
  // Use data_transformer to infer the expected blob shape from a cv_img.
  // vector<int> top_shape = this->data_transformer_->InferBlobShape(cv_img);
  vector<int> top_shape_data(4);
  vector<int> top_shape_label(4);
  if (crop_height>0 && crop_width>0) {
    top_shape_data[0] = 1;
    top_shape_data[1] = channels;
    top_shape_data[2] = crop_height;
    top_shape_data[3] = crop_width;
    top_shape_label[0] = 1;
    top_shape_label[1] = 1;
    top_shape_label[2] = crop_height;
    top_shape_label[3] = crop_width;
  } else {
    top_shape_data[0] = 1;
    top_shape_data[1] = channels;
    top_shape_data[2] = height;
    top_shape_data[3] = width;
    top_shape_label[0] = 1;
    top_shape_label[1] = 1;
    top_shape_label[2] = height/label_divide_factor;
    top_shape_label[3] = width/label_divide_factor;
  }
  this->transformed_data_.Reshape(top_shape_data);
  this->transformed_label_.Reshape(top_shape_label);
  // Reshape batch according to the batch_size.
  top_shape_data[0] = batch_size;
  top_shape_label[0] = batch_size;
  batch->data_.Reshape(top_shape_data);
  batch->label_.Reshape(top_shape_label);

  Dtype* prefetch_data = batch->data_.mutable_cpu_data();
  Dtype* prefetch_label = batch->label_.mutable_cpu_data();

  // datum scales
  const int lines_size = lines_.size();
  for (int item_id = 0; item_id < batch_size; ++item_id) {
    // get a blob
    timer.Start();
    CHECK_GT(lines_size, lines_id_);
    cv::Mat cv_img = ReadImageToCVMat(root_folder_image + lines_[lines_id_].first,
        new_height, new_width, is_color, CV_INTER_LINEAR);
    CHECK(cv_img.data) << "Could not load " << lines_[lines_id_].first;
    cv::Mat cv_lab = ReadImageToCVMat(root_folder_label + lines_[lines_id_].second,
                                    new_height/label_divide_factor, new_width/label_divide_factor, 
                                    false, CV_INTER_NN);
    CHECK(cv_lab.channels() == 1) << "Can only handle grayscale label images";
    // CHECK(cv_lab.rows == height && cv_lab.cols == width) << "Input and label "
      // << "image heights and widths must match!";
    read_time += timer.MicroSeconds();

    timer.Start();
    // Apply random horizontal mirror of images
    if (this->layer_param_.dense_image_data_param().mirror()) {
      const bool do_mirror = caffe_rng_rand() % 2;
      if (do_mirror) {
        cv::flip(cv_img,cv_img,1);
        cv::flip(cv_lab,cv_lab,1);
      }
    }

    // Apply multi_scale and crop to the initial size
    int h_off = 0;
    int w_off = 0;
    if (dense_image_data_param.multi_scale()) {
      const bool do_multi_scale = caffe_rng_rand() % 2;
      if (do_multi_scale) {
        float scale = (caffe_rng_rand() % 5)/10 + 1; // [1, 1.5]
        cv::resize(cv_img, cv_img, cv::Size(new_width*scale, new_height*scale), 0, 0, CV_INTER_LINEAR);
        cv::resize(cv_lab, cv_lab, cv::Size(new_width*scale, new_height*scale), 0, 0, CV_INTER_NN);
        // crop
        h_off = caffe_rng_rand() % (cv_img.rows - new_height + 1);
        w_off = caffe_rng_rand() % (cv_img.cols - new_width + 1);
        cv::Rect myROI(w_off, h_off, new_width, new_height);
        cv_img = cv_img(myROI);
        cv_lab = cv_lab(myROI);
      }
    }
    // Apply crop TODO: crop is not applicable when label_divide_factor>1
    

    if (crop_height>0 && crop_width>0) {
      h_off = cv_img.rows - crop_height;
      w_off = caffe_rng_rand() % (cv_img.cols - crop_width + 1);
      cv::Rect myROI(w_off, h_off, crop_width, crop_height);
      cv_img = cv_img(myROI);
      cv_lab = cv_lab(myROI);
    }

    // Apply transformations (...) to the image
    int offset = batch->data_.offset(item_id);
    this->transformed_data_.set_cpu_data(prefetch_data + offset);
    this->data_transformer_->Transform(cv_img, &(this->transformed_data_), false);
    // Apply transformations (...) to the label
    offset = batch->label_.offset(item_id);
    this->transformed_label_.set_cpu_data(prefetch_label + offset);
    this->data_transformer_->Transform(cv_lab, &(this->transformed_label_), true);

    trans_time += timer.MicroSeconds();
    // go to the next iter
    lines_id_++;
    if (lines_id_ >= lines_size) {
      // We have reached the end. Restart from the first.
      DLOG(INFO) << "Restarting data prefetching from start.";
      lines_id_ = 0;
      if (this->layer_param_.dense_image_data_param().shuffle()) {
        ShuffleImages();
      }
    }
  }
  batch_timer.Stop();
  DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
  DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
  DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
}

INSTANTIATE_CLASS(DenseImageDataLayer);
REGISTER_LAYER_CLASS(DenseImageData);

}  // namespace caffe
#endif  // USE_OPENCV
