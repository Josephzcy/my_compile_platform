#pragma once
#include <string>
#include <vector>
#include "gflags/gflags.h"
#include "opencv2/opencv.hpp"

struct Tda4MaskInfo {
    uint64_t phy_addr;
    uint8_t *virt_addr;
    int width;
    int height;
    int channel;
    int datatype;
};

struct CnnRoi {
    uint64_t src_y;
    uint64_t src_uv;
    int img_w;
    int img_h;
    int start_x;
    int start_y;
    int roi_w;
    int roi_h;
    int resize_w;
    int resize_h;
    int frameId;
};

class Tda4Accelerator {
public:
    /**
     * @brief Construct a new Tda 4 Accelerator object
     * 
     * @param config_path model config file, with surfix： "_1"
     * @param network_path model network file
     * @param out_tensor_bytes output sentor data format bytes, default:1
     */
    explicit Tda4Accelerator(const std::string &config_path,
                             const std::string &network_path,
                             int out_tensor_bytes = 1);

    // Forbid copy constructor and operator= constructor
    Tda4Accelerator(const Tda4Accelerator &) = delete;
    Tda4Accelerator &operator=(const Tda4Accelerator &) = delete;

    /**
     * @brief Release the memory allocated by constructor
     * 
     */
    ~Tda4Accelerator();

    /**
     * @brief Resize 2560x1600 -> 1024x640, cvtColor(NV12_To_YUV444), 
     * copy to input tensor
     * 
     * @param roi struct of input data, include addr, 
     * img size, resized size, frameid
     * @param y_virt_addr output virtual addr of Y plane's physical addr
     * @param uv_virt_addr output virtual addr of UV plane's physical addr
     */
    void PrepareInput_freespace(const CnnRoi &roi,
                                void **y_virt_addr,
                                void **uv_virt_addr);

    /**
     * @brief Crop padding bytes, cvtColor(NV12_To_YUV444), copy to input tensor
     * 
     * @param roi resized region
     * @param src_y image Y physical addr
     * @param src_uv image UV physical addr
     * @param img_w input image actual width, including padding, i.e. 384
     * @param img_h input image actual height, i.e. 480
     * @param y_virt_addr output virtual addr of Y plane's physical addr
     * @param uv_virt_addr output virtual addr of UV plane's physical addr
     */
    void PrepareInput_parkingslot(cv::Rect &roi,
                                  uint64_t src_y,
                                  uint64_t src_uv,
                                  int img_w,
                                  int img_h,
                                  void **y_virt_addr,
                                  void **uv_virt_addr);

    /**
     * @brief Parse mask data from output tensor 
     * 
     * @param output output mask struct
     */
    void ParseOutput(std::vector<Tda4MaskInfo> *output);

    /**
     * @brief Parse several raw vector from instance ps output tensor
     * 
     * @param output vector of output ptrs
     */
    void ParseOutputInsPs(std::vector<void *> &output);

    /**
     * @brief Call tda4 @ref vxProcessGraph to run inference
     * 
     */
    void Forward();

    /**
     * @brief Translate physical addr to virtual addr
     * 
     * @param phy_addr physical addr
     * @param size size of memory to be translated
     * @param virt_add output virtual addr
     */
    static void Tda4PhyToVirt(uint64_t phy_addr,
                              uint32_t size,
                              void **virt_add);

private:
    /**
     * @brief Wraped function to translate nv12 physical addr
     *  to virtual addr
     * 
     * @param src_y image Y physical addr
     * @param src_uv image UV physical addr
     * @param img_w input image actual width, including padding, i.e. 384
     * @param img_h input image actual height, i.e. 480
     * @param data_ptr output virtual addr of Y plane's physical addr
     * @param data_ptr2 output virtual addr of UV plane's physical addr
     */
    void TranslatePhyAddr(uint64_t src_y,
                          uint64_t src_uv,
                          int img_w,
                          int img_h,
                          void **data_ptr,
                          void **data_ptr2);

    /**
     * @brief save YUV444 image to be copied into input tensor
     * 
     * @param yuv444ptrs y-ptr
     * @param width width of image
     * @param height height of image
     * @param type type of model: FS / PS
     */
    void save_yuv444_img(u_char *yuv444ptrs,
                         int width,
                         int height,
                         const std::string &type);

    struct AppObj *m_obj = nullptr;
    std::map<uint64_t, void *> m_y_addr_map;
    std::map<uint64_t, void *> m_uv_addr_map;
    int m_buf_idx = 0;
    uint8_t *m_p_in_planes;
    std::vector<std::vector<uint8_t *>> m_p_out_planes;
    bool m_scale_app_init = false;
    int m_out_tensor_bytes = 1;

    // tensor size config
    size_t m_start[8][4];
    size_t m_input_strides[8][4];
    size_t m_input_sizes[8][4];
    size_t m_output_sizes[8][4];
    size_t m_output_strides[8][4];
    int m_input_pad_l[8];
    int m_input_pad_r[8];
    int m_input_pad_t[8];
    int m_input_pad_b[8];

    static std::mutex g_dsp_mutex;
    static bool g_scale_app_init;
    static std::mutex g_acc_count_mutex;
    static int g_acc_count;
    static int g_count;
};
