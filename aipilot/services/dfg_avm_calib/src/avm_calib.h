//
// Created by minieye on 2022/5/12.
//

#ifndef AVM_CALIB_H
#define AVM_CALIB_H

#ifdef WIN32
#define AVM_API  _declspec(dllexport)
#else
#define AVM_API    __attribute__ ((visibility("default")))
#endif
#define AvmHandle void*
#define AVM_CAMERA_NUM (4u)
#ifdef __cplusplus
extern "C" {

#endif
/**
 * 根据传入的内参文件与外参文件进行初始化，得到标定句柄
 * @param camera_file_path，内参配置文件路径列表
 * @param extrins_file，外参文件路径
 * @return
 */
AVM_API AvmHandle initAvmCalib(const char camera_file_path[AVM_CAMERA_NUM][128],
                               const char layout_file[128],
                               const char car_info_file[128]);

/**
 * 输入标定相机id，以及图像数据，将标定结果存储在指定路径
 * @param handle，非空句柄
 * @param camera_id，front--0x1, rear--0x2, left--0x4, right--0x8
 * @param image_data，图像数据
 * @param calib_file，保存标定结果的文件名称
 * @return 重投影误差
 */
AVM_API float avmCalib(AvmHandle handle, unsigned int camera_id,
                       const unsigned char *image_data, const char *calib_file);

AVM_API void avmRelease(AvmHandle handle);

#ifdef __cplusplus
}
#endif

#endif //AVM_CALIB_H
