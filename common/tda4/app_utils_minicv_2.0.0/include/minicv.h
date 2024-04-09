#ifndef _MINICV_H_
#define _MINICV_H_

#define MINICV_TARGET_DSP1 1
#define MINICV_TARGET_DSP2 2
#define MINICV_TARGET_DSP_C7 3

extern int32_t minicv_convolve_i8u_c16s_o8u(const uint8_t src[], uint8_t dst[], int32_t img_width, int32_t img_height,
                                            const int16_t conv_mat[], int32_t conv_width, int32_t conv_height,
                                            uint32_t conv_scale, uint8_t target_dsp_nr);

extern int32_t minicv_convolve_i8u_c16s_o16s(const uint8_t src[], int16_t dst[], int32_t img_width, int32_t img_height,
                                             const int16_t conv_mat[], int32_t conv_width, int32_t conv_height,
                                             uint32_t conv_scale, uint8_t target_dsp_nr);

extern int32_t minicv_convolve_i8u_cf_o16s(const uint8_t src[], int16_t dst[], int32_t img_width, int32_t img_height,
                                           const float conv_mat[], int32_t conv_width, int32_t conv_height,
                                           uint32_t conv_scale, uint8_t target_dsp_nr);

extern int32_t minicv_nv12toi420(const uint8_t src0[], const uint8_t src1[], int32_t img_width, int32_t img_height,
                                 uint8_t dst0[], uint8_t dst1[], uint8_t dst2[], uint8_t target_dsp_nr);

enum minicv_interpolation_type_e {
  MINICV_INTERPOLATION_NEAREST_NEIGHBOR = 0,
  MINICV_INTERPOLATION_BILINEAR,
};

/* the output image width should be 64-byte aligned */
extern int32_t minicv_scale_i8u_o8u(const uint8_t src[], int32_t in_width, int32_t in_height, uint8_t dst[],
                                    int32_t out_width, int32_t out_height, enum minicv_interpolation_type_e type,
                                    uint8_t target_dsp_nr);

/* only support nearest neighbor interpolation */
extern int32_t minicv_scale_nv12_i8u_o8u(const uint8_t src[], int16_t *recparam, int32_t in_width, int32_t in_height,
                                         uint8_t dst[], int32_t out_width, int32_t out_height,
                                         enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);

extern int32_t minicv_scaleRoi_nv12_i8u_o8u(uint8_t src[], int32_t in_width, int32_t in_height, uint8_t dst[],
                                            int32_t out_width, int32_t out_height,
                                            enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);

int32_t minicv_scaleRoi_2camM1_nv12_i8u_o8u(uint8_t src[], int32_t in_width, int32_t in_height, uint8_t dst[],
                                            int32_t out_width, int32_t out_height,
                                            enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);

vx_status app_create_graph_scale(void **mulInPic, int32_t *paramObj, int32_t objlen, int32_t in_width,
                                 int32_t in_height, void **mulOutPic, int32_t out_width, int32_t out_height,
                                 enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);

vx_status app_run_graph_scale(void **mulInPicP, void **mulOutPicP);
void app_delete_graph_scale();

vx_status app_create_graph_scale_2camM1(void **mulInPic, int32_t *paramObj, int32_t objlen, int32_t in_width,
                                        int32_t in_height, void **mulOutPic, int32_t out_width, int32_t out_height,
                                        enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);
// vx_status app_run_graph_scale_2camM1(void **mulInPicP, void **mulOutPicP);
vx_status app_run_graph_scale_2camM1(void **mulInPicP, void **mulOutPicP, int32_t *paramObj, int32_t objlen);

void app_wait_graph_scale_2camM1();

void app_delete_graph_scale_2camM1();

vx_status app_create_graph_scale_grid(void **mulInPic, int32_t *paramObj, int32_t objlen, int32_t in_width,
                                      int32_t in_height, void **mulOutPic, int32_t out_width, int32_t out_height,
                                      enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);
// vx_status app_run_graph_scale_grid(void **mulInPicP, void **mulOutPicP);
vx_status app_run_graph_scale_grid(void **mulInPicP, void **mulOutPicP, int32_t *paramObj, int32_t objlen);

void app_delete_graph_scale_grid();

vx_status app_create_graph_scale_2tda(void **mulInPic, int32_t *paramObj, int32_t objlen, int32_t in_width,
                                      int32_t in_height, void **mulOutPic, int32_t out_width, int32_t out_height,
                                      enum minicv_interpolation_type_e type, uint8_t target_dsp_nr);
vx_status app_run_graph_scale_2tda(void **mulInPicP, void **mulOutPicP, int32_t *paramObj, int32_t objlen,
                                   uint8_t target_dsp_nr);
void app_wait_graph_scale_2tda(uint8_t target_dsp_nr);
void app_delete_graph_scale_2tda(uint8_t target_dsp_nr);
void app_set_inImgPara_2tda(uint32_t width, uint32_t height, uint8_t target_dsp_nr);
void app_set_outImgPara_2tda(uint32_t width, uint32_t height, uint8_t target_dsp_nr);

#endif
