#include <caffe/caffe.hpp>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
//#include <gperftools/profiler.h>

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;


class Classifier {
public:
	Classifier(const string& model_file,
		const string& trained_file);
	std::vector<float> Predict(int iterations);

private:
	cv::Size GetImgSize() { return input_geometry_; }
	int GetChannels() { return num_channels_; }

private:
	shared_ptr<Net<float> > net_;
	cv::Size input_geometry_;
	int num_channels_;
	cv::Mat mean_;
	std::vector<string> labels_;
};

Classifier::Classifier(const string& model_file,
	const string& trained_file) {

	Caffe::set_mode(Caffe::CPU);

	/* Load the network. */
	net_.reset(new Net<float>(model_file, TEST));
        std::fstream file(trained_file.c_str(), ios::in);
        if (file)
        {
          net_->CopyTrainedLayersFrom(trained_file);
        }
	else //use random weights
        { 
          std::cout<<"profiling with random weights"<<std::endl;
        }

	Blob<float>* input_layer = net_->input_blobs()[0];
	num_channels_ = input_layer->channels();
	CHECK(num_channels_ == 3 || num_channels_ == 1)
		<< "Input layer should have 1 or 3 channels.";
	input_geometry_ = cv::Size(input_layer->width(), input_layer->height());
}

std::vector<float> Classifier::Predict(int iterations) {
	Blob<float>* input_layer = net_->input_blobs()[0];
	input_layer->Reshape(1, num_channels_,
		input_geometry_.height, input_geometry_.width);
	int width = input_layer->width();
	int height = input_layer->height();
	float* input_data = input_layer->mutable_cpu_data();
	
	net_->Reshape();
        struct timespec time1 = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &time1);
	std::cout<<"start counting"<<std::endl;
//ProfilerStart("my.prof");
	for (int i = 0; i < iterations; i++)
	{
		memset(input_data, 0, width*height*num_channels_);
		input_data[i % (width*height*num_channels_)] = (i % 255);
		net_->Forward();
	}
        struct timespec time2 = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &time2);
        double time_spend = (time2.tv_sec-time1.tv_sec)+(time2.tv_nsec-time1.tv_nsec)/1000000000.0;
//ProfilerStop();
	std::cout<<"stop counting"<<std::endl;
	std::cout << "totle Time : " << time_spend << "s" << std::endl;
	/* Copy the output layer to a std::vector */
	Blob<float>* output_layer = net_->output_blobs()[0];
	const float* begin = output_layer->cpu_data();
	const float* end = begin + output_layer->channels();
	return std::vector<float>(begin, end);
}


int main(int argc, char** argv) {
	
	::google::InitGoogleLogging(argv[0]);
	string model_file = argv[1];
	string trained_file = argv[2];
	int iterations;
	sscanf(argv[3], "%d", &iterations);
  //iterations=10000;
  //string model_file="/data/wzheng/projects/lane_detect/lanemark_filtering/caffe_training_ARM/lane_pts_cls_deploy.prototxt";
  //string trained_file="/data/wzheng/projects/lane_detect/lanemark_filtering/caffe_training_ARM/models/minieye_lanemark_iter_4000.caffemodel";

	Classifier classifier(model_file, trained_file);
	classifier.Predict(iterations);
}