// Copyright [2019] - MiniEye INC.
#include "glog/logging.h"
#include "image.h"

Image::~Image() {
    Clear();
}

Image::Image(int rows, int cols, int type) {
    Reset(rows, cols, type);
}

Image::Image(const std::string& image_name) {
    m_data_mat = cv::imread(image_name.c_str(), 1);
    m_buff = NULL;
    m_buff_size = 0;
}

const Image& Image::operator=(const Image& lhs) {
    Reset(lhs.Rows(), lhs.Cols(), lhs.Type());
    m_data_mat = (lhs.GetMat())->clone();
    return *this;
}

void Image::Swap(Image* lhs) {
    lhs->SwapOut(&m_buff, &m_buff_size, &m_data_mat);
}

void Image::SwapOut(void** buff, uint32_t* buff_size, cv::Mat* mat) {
    *buff = m_buff;
    *buff_size = m_buff_size;
    *mat = m_data_mat;

    m_buff = NULL;
    m_buff_size = 0;
}

void Image::GetMat(cv::Mat* mat) {
    *mat = m_data_mat;
}

const cv::Mat* Image::GetMat() const {
    return &m_data_mat;
}

void Image::Reset(int rows, int cols, int type) {
    // in bytes
    int elem_size = CV_ELEM_SIZE(type);
    int row_step = cols * elem_size;
    m_buff_size = row_step * rows;
    m_buff = reinterpret_cast<void*>(Allocate<uint8_t>(m_buff_size));

    m_data_mat = cv::Mat(rows, cols, type, m_buff, row_step);
}

int Image::Rows() const {
    return m_data_mat.rows;
}

int Image::Cols() const {
    return m_data_mat.cols;
}

int Image::Type() const {
    return m_data_mat.type();
}

int Image::Channels() const {
    return m_data_mat.channels();
}

void Image::Clear() {
    if (m_buff != NULL)
        Free(reinterpret_cast<uint8_t*>(m_buff));
    m_buff = NULL;
}

Image* Image::Resize(float scale_ratio) {
    LOG(FATAL) << "not implemented";
    return NULL;
}

void Image::Show(const std::string& name, bool waited) {
    if (CV_ELEM_SIZE(m_data_mat.type()) > 1) {
        std::vector<cv::Mat> split_mats;
        cv::split(m_data_mat, split_mats);
        // find min max value
        std::vector<double> min_vals(split_mats.size());
        std::vector<double> max_vals(split_mats.size());

        // normalized
        int type = CV_MAKETYPE(CV_8U, m_data_mat.channels());
        for (int8_t i = 0; i < m_data_mat.channels(); ++i) {
            cv::minMaxLoc(split_mats[i], &min_vals[i], &max_vals[i]);
        }
        std::vector<cv::Mat> norm_mats(m_data_mat.channels());
        for (int8_t c = 0; c < m_data_mat.channels(); ++c) {
            split_mats[c].convertTo(norm_mats[c], CV_8UC1,
                    255 / (max_vals[c] - min_vals[c]), /* scale */
                    -min_vals[c] * 255 / (max_vals[c] - min_vals[c]) /* shift */
                    );
        }
        // merge
        cv::Mat demo_mat;
        cv::merge(norm_mats, demo_mat);
        cv::imshow(name.c_str(), demo_mat);
    } else {
        cv::imshow(name.c_str(), m_data_mat);
    }

    if (waited)
        cv::waitKey(-1);
    else
        cv::waitKey(1);
}
