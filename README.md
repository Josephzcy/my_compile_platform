#### **1. 概述** 
&ensp;&ensp;&ensp;&ensp;aiplorer是一个基于gitlab之上的集成开发平台，主要涉及到以下几方面：版本管理，接口管理，构建，一站式发布和开发闭环：   
&ensp;&ensp;&ensp;&ensp;**版本管理**：aiplorer改变了以往各模块零散管理，导致版本混乱的现状。   
&ensp;&ensp;&ensp;&ensp;**接口管理**：统一了消息接口，管理更便捷。   
&ensp;&ensp;&ensp;&ensp;**构建**: 长远规划迁移到bazel代码构建方式，构建更快速，支持多平台，对象化模块。   
&ensp;&ensp;&ensp;&ensp;**一键式发布**：一键式构建出行泊车系统的包，规避了人工生成包导致的文件缺失甚至错误，极大地便利了版本发布和   
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;交付，提升了开发效率，最终实现公司行泊车系统研发迭代加速。   
&ensp;&ensp;&ensp;&ensp;**开发闭环**：基于gitlab之上的aiplorer与Coding打通，然后借助gitlab-ci做CI/CD, 整体形成了一个开发流程的闭环。   


#### **2. aiplorer框架** 
&ensp;&ensp;&ensp;&ensp;以下是aiplorer在[gitlab](https://git.minieye.tech/ap/aiplorer)上工程2级目录状况：
```
├── aipilot
│   ├── modules                -->存放各算法模块代码，比如apa_manager,apa_planning,...
│   └── services               -->存放一些服务性质代码，比如dfg_avm_calib,bev..
├── bazel                      -->bazel交叉编译环境相关文件
│   └── ...                  
├── common                     -->存放公司内部开发的公用模块代码,特别的是linux和tda4目录存放linux和tda4平台的做构建时的依赖库
│   ├── common-2.0.1           -->存放早期的开发的公共接口源代码
│   ├── dfg_apa_common         -->存放apa相关的另外一个公共库源代码
│   ├── dfg_apa_log_reader     -->存放公司内部开发的log read的库源代码
│   ├── linux                  -->存放x64 ubuntu平台构建时的依赖库
│   └── tda4                   -->存放tda4平台构建时的依赖库
├── docs                       -->各类汇总的文档
│   ├── gcc升级到9.4说明.md     -->文档
│   ├── images                 -->存放文档中用到的贴图
│   ├── 基于aiplorer开发流程.md -->文档
│   ...
├── examples                   -->存放示例
│   ├── args_main
│   ├── pb_topic
│   ...
├── msg                        -->存放模块间交互的message
│   ├── interface.json         -->暂未使用
│   ├── pb                     -->汇总的统一的通信用的proto文件
│   ├── precompiler            -->proto文件生成的C++源代码
│   ├── README.md              -->markdown说明
│   ├── topic.json             -->汇总的统一的topic.json
│   └── Version                -->整体所有pb文件对应的版本号,每次修改了pb需要更新
├── README.md                  -->本文档
├── scripts                    -->存放发起BUILD和CI/CD相关的脚本和配置等
│   ├── cmakebuild             -->存放cmake构建相关脚本
│   ├── bazelbuild             -->存放bazel构建脚本
│   ├── ci_script              -->存放cicd脚本
│   ├── ci_config              -->存放cicd配置
│   ├── toolbox                -->存放维护相关的脚本或工具
│   ├── publish                -->存放发布用的脚本和配置
│   └── target_config          -->存放发布用的配置文件
├── third_party                -->存放第三方开源库源代码和生成库文件
│   ├── boost_1_56_0
│   ├── caffe-1.0.0
│   ├── ceres-solver-1.14.x
│   ...
│   ├── linux                  -->存放第三方开源库源代码在x64 ubuntu上生成库文件
│   └── tda4                   -->存放第三方开源库源代码在tda4上生成库文件
├── tools                      -->用来存放一些仿真，测试以及可视化等工具
│    └── mviz
├── .gitlab-ci.yml             -->aiplorer CICD流程控制配置文件
└── ...
```
       
#### **3. 编译工具链安装**  
- 交叉编译工具链安装    
&ensp;&ensp;&ensp;&ensp;构建用的交叉工具链(g++,gcc等,目前版本是9.2)固定放置在/opt目录下, 可以从
https://git.minieye.tech/ap/toolchains/-/tree/master/  工程获得：
因为当前暂时没启用tda4这一级目录，所以此时/opt目录下的文件目录可参照下图:   
```
toolchains/
├── J3
│   ├── gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu
│   └── sysroots
├── linux
│   └── ubuntu18.04.v1.img
├── README.md
└── tda4
    ├── gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu
    └── sysroots
```
- linux编译安装工具安装   
  在ubuntu 18.04环境安装gcc/g++ 9.4, 可以参考[本链接](https://blog.csdn.net/weixin_46584887/article/details/122527982)做法   

#### **4. 模块集成到aiplorer**        
&ensp;&ensp;&ensp;&ensp;~~当前仅仅集成了dfg_apa等少数几个算法模块代码进aiplorer(即融合感知算法模块),后续会有新的模块集成进来, 现以集成dfg_apa为例来解说详细步骤   
(假设build的是linux版本，在构建tda4版时才会用到上述交叉编译工具链)：    
**步骤**：git clone aipilot工程, git clone dfg_apa工程代码, 进入dfg_apa, git checkout 到目标分支(比如dds分支),然后将整个   
工程直接copy到已clone下来的aiplorer工程下的aiplorer/aipilot/modules下 (最新的代码集成到aiplorer使用submodule的方式，详细说明见[说明](https://git.minieye.tech/ap/aiplorer/-/blob/master_apa/docs/%E5%9F%BA%E4%BA%8Eaiplorer%E7%9A%84%E5%BC%80%E5%8F%91%E6%B5%81%E7%A8%8B.md))~~   

这里穿插一个submodule的话题(20231226)   
```
    submodule的方式可以让我们在aipilot下的moudules和services目录下建立一个submodule的链接，只有有对应权限的人才能点击直达，无   
对应模块权限的人可以虽然无法点击进去，但是可以看到当前项目分支包含哪些模块，便于我们一窥全貌，但submodule的方式经过一段时间的使用普遍   
觉得不是很方便，而且使用过程中容易出现一些异常，导致当前分支不可用，所以我们决定放弃submodule的使用。当前做了一个替代方案，那就是一键   
重建aipilot下的工作模块(会帮忙拉取自己有权限的模块,没无权的模块会有一个带模块名的空目录，以及aiplorer自带的模块代码):   
$ ./scripts/toolbox/onekey_workspace.sh
```

**aiplorer集成新的模块使用说明:**   
https://alidocs.dingtalk.com/i/nodes/N7dx2rn0JbZxZ2D6Cjpnje7QJMGjLRb3?doc_type=wiki_doc&dontjump=true&utm_scene=team_space#

支持cmake和bazel两种编译方式, 比如linux下，切换到aiplorer/scripts/目录下执行   
```
./cmakebuild/single_build.sh linux release dds gflags dfg_apa  
```
如果编译到最后会出现多个库的依赖，需要自己在命令行下手动安装一下(tda4版不需要以下安装),冉崇重新启动编译:   
```
$sudo apt install -y libwebp5 libgfortran4 libavcodec-ffmpeg56 libavformat-ffmpeg56  libavutil-ffmpeg54 libswscale-ffmpeg3 
```
当前aiplorer还支持bazel编译，比如编译tda4平台的模块apa_planning:   
```
$ ./scripts/bazelbuild/build.sh tda4 debug apa_planning
```   


#### **5. protobuffer的问题**  
    protobuffer目前使用跟嵌入保持一致的3.12.3版本(特例：j3平台目前使用的2.6.1)，执行pb目录下的脚本文件(gen.sh),   
即可在与pb同级目录下的precompiler文件夹下生成所有proto对应的.cc和.h源文件：  
```
./gen.sh
```

#### **6. 常见问题**   
- aiplorer原则上都用bazel来做编译，我有个很大的模块用的cmake，但改成bazel成本很高，但是又想在aiplorer上做CI/CD怎么办? 
 (20231226)   
  目前aiplorer不仅仅支持bazel和cmake两种方式分别单独做CI/CD, 同时也已经支持了混合的方式,只需要在ci_config/package.json里正确填写   
  模块支持的build_model即可。   
  **但请注意:** bazel编译更多时候是静态链接的方式，将模块内的代码编译到一个可执行文件里，而cmake可能会生成一些libxx.so, 所以用   
  cmake的方式要多加核实在target_config/deploy.json里当前模块是否有生成的*.so没有最终打包进最终的app包。   

- bazel编译后debug版可执行程序在gdb下为什么不能显示源代码? (20231226)  
  aiplorer下examples/hello_world程序作为例子   
  **① 直接在x86-64 linux下的gdb调试:**   
  在aiplorer根目录可以直接用如下命令, 就可以在单步调试时看到对应源代码   
  ```
  $ gdb bazel-bin/examples/hello_world/hello_world
  ```
  如果切到aiplorer非根目录,直接去gdb调试是看不到源代码的, 这时需要做一些路径配置, 指定aiplorer的根目录的绝对路径(其实是指定     
  WORKSPACE所在路径), 二进制内部会记录examples/hello_world这个路径，把两者拼接起来就能找到源代码(bazel调试需要用到源代码，   
  尽管二进制内部已经包含了调试信息,很无语~)   
  ```
  $ gdb hello_world
  (gdb) set directories /home/wss/aiplorer
  ```
  甚至可以直接指向代码目录:
  ```
  (gdb) set directories /home/wss/aiplorer/examples/hello_world
  ```
  这时就可以在gdb调试过程中看到源代码了   
  **② 在域控板子上gdb调试:**   
  在板子上当前没有aiplorer这样的现场，所以就需要去copy源代码到板子上，比如/app/alog/apa/bin,需要注意的是需要连同examples/hello_world   
  这个路径也copy，建议用cp -rf --path examples/hello_world xx, 然后把xx目录打包，scp到目标目录后再解压,然后用①里的方法去设置路径:   
  ```
  (gdb) set directories /app/alog/apa/bin/examples/hello_world
  ```
  这时用gdb调试就能正常地看到源代码了   

- linux命令行下每次git clone、pull、push等操作时都需要输入账号密码，十分繁琐, 怎么办?   
  ① linux命令行下输入指令生成ssh公钥和私钥对：ssh-keygen -t rsa -C 'xxx@minieye.cc' 其中xxx@minieye.cc即为登录gitlab的邮箱地址,   
    点击回车，会让你选择存储路径，此时不用理会直接回车，其会保存到默认路径,可能有人此目录下已有该文件,会提示是否覆盖,输入yes回车会提   
    示输入密码，不用理会，后面一路回车,即在~/.ssh生成一对密钥对   
  ② 到GitLab界面，点击settings，后点击SSH-Keys，将步骤①生成公钥(id_rsa.pub)内容复制粘贴到key框中，下方title可自己命名，点击    
     addkey按钮。   
  ③ 经过如上操作，再次pull/push 等git操作若还是需要输入密码，输入   
     git config --global user.email "你的git的注册邮箱"   
    git config --global user.user "你的git用户名"   
    然后输入命令： git config --global credential.helper store   下次再操作pull/push时就不需要输入用户名密码了。   

- ~~为什么还是必须用gcc/g++ 4.9这么老的编译器来代码？   
&ensp;&ensp;&ensp;&ensp;虽然aiplorer不再需要依赖远程的conan库，但是我们当前工程中依然使用的是从conan远程库下载下来的依赖库，这些依赖库中   
文件linux版的统一都是用的gcc/g++ 4.9构建, 链接的是与gcc/g++ 4.9对应版本的c/c++ 的标准库，如果我们在build时用更高版本   
的gcc/g++ 编译器，链接时也会是新版本的c/c++库, 会发生报错。所以除非aiplorer下所有依赖库都改为以源代码的形式提供了,    
则可以采用更新的编译器去build，或者将所有依赖库重新用某个高版本的编译器build一遍，更替旧有的依赖库，则往后大家便可以   
切换到这个高版本的编译器来构建，但是这个工作需要所有相关模块负责人都能协调一致。~~   
**更新：当前aiplorer的构建支持x86 linux使用gcc/g++ 9.4, tda4交叉编译gcc/g++9.2**

- ~~为什么推荐使用ubuntu 18.04版本系统？   
&ensp;&ensp;&ensp;&ensp;因为以后aiplorer要从当前的cmake构建方式迁移到[bazel](https://bazel.build/start/getting-started)构建方式,而截至目前bazel官方仅仅在ubuntu 16.04和18.04有做测试其他版本比如   
20.04甚至更高版本不保证能work，所以为了有一个稳定和统一的环境，当前推荐使用18.04版。~~   
**更新：当前bazel的推荐系统是ubuntu 18.04和20.04** (20231226)    
