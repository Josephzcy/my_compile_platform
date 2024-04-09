#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

struct ApaCnnReq {
    uint32_t frame_id;
    void* avm_image_vir;
    uint64_t avm_image_phy[2];
    uint32_t avm_image_width;
    uint32_t avm_image_height;
    void* front_camera;
    void* rear_camera;
    void* left_camera;
    void* right_camera;
    //运行选项标志,详细见下
    uint32_t flag;
};


//CNN的标志位选项

//传入图片使用物理地址
#define CNN_IMAGE_ADDR_PHY  0x1

//传入图片使用虚拟地址(此时宽度和高度非必须)
#define CNN_IMAGE_ADDR_VIR  0x2

//打开障碍物探测CNN功能(obs)
#define CNN_FUNC_OBS_ON     0x4

//打开车位探测CNN功能(parkingslot)
#define CNN_FUNC_PS_ON      0x8

struct ApaCnnRsp {
    void** cnn_obs_res;
    void* cnn_ps_res;
};

void ApaCnnInit(int pipeline_num);
void ApaCnnRun(void* req, void* rsp);
void ApaCnnDestroy();
void ApaCnnReload();


#ifdef __cplusplus
};
#endif