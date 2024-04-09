# 结构
```
.bird_eye_view
├── BUILD           bazel编译文件
├── CMakeLists.txt  cmake文件
├── flag            配置文件
├── main_bev.cpp    主函数
├── README.md
├── script          打包，运行脚本
├── src             源代码
└── utils           功能函数
```
# 编译说明
cmake编译脚本：
```
cd script
./build_scripts_apa/single_build.sh tda4 release dev gflags bird_eye_view
```
bazel编译
```
./scripts/bazelbuild/build.sh tda4 release //aipilot/services/bird_eye_view:bird_eye_view
```
# 格式化代码
```
clang-format -style=file -i $(find . \( -name build \) -prune -false -o -name "*.c*" -o -name "*.h*")
```
# cppcheck
```
cppcheck --enable=all --force --inconclusive --std=c++11 --language=c++ . -i ./build >bev.log 2>&1
```
# 更新日志
- 1.0.25 删除dds接受后需要条件变量触发才处理逻辑，减少了处理延迟
- 1.0.26 添加最大帧率控制，节省算力和降低CPU占用率
- 1.0.27 
    - 去掉对camera_aerial.json依赖，改成程序自动生成的投影变换矩阵，
    - 改变标定映射表位置到`/calib/avm_calib`
    - 减小对GPU资源占用，25fps：18%->7.5%，12.5fps：9%->3.5%
- 1.0.28 增加故障诊断功能，包括故障上报和心跳包
- 1.0.29 增加故障恢复功能
- 1.0.30 和x50保持一直version
- 1.0.31 修改拼接范围和CPU结果不一致的bug，修复cppcheck中的warning
- 1.0.32 
  - 修改dds输出buffer num由1改为8，
  - 输出当前拼接改为输出上一帧确认拼接完成的帧，
  - 存中间变量rgba改为存输出到下游的NV12再转成RGB图
- 1.0.33 修改使用openGL把RGBA转NV12时左偏1.5pixel的bug
- 1.0.34 支持bazel编译，删除第三方库，用glog替换spdlog，用gflag替换CLI，代码规范化修改
- 1.0.35 bazel修改cc_library，代码规范化修改
- 1.0.36 代码规范化修改,删除没用过的函数和类
- 1.0.37 bev使用dsp进行拼接
- 1.0.38 
  - 增加跳过apa_state的代码，方便debug
  - 修改启动脚本中LD_LIBRARY_PATH的链接路径
  - 对CMakeLists中的TDA4_SYS_ROOT进行适配
