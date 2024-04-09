#### **1. 概述** 
&ensp;&ensp;&ensp;&ensp;当前我们在进行泊车系统模块开发的过程中，通常同一份代码需要构建两个版本，一个是用于烧录到台架上的做调试测试的基于tda4架构的交叉编译版本，另外一个基于我们x86_64架构Ubuntu PC版本用于仿真调试测试，前者用到的交叉编译工具链使用的是gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu, 不难发觉使用的是gcc/g++ 9.2，但是由于历史原因，x86_64架构Ubuntu版构建过程中所依赖的conan远程库都是基于gcc/g++ 4.9.3所构建，如果使用更高版本比如gcc/g++ 7.5 或 9 去做构建链接阶段是通不过的, 于是C++开发者为了兼顾x86_64架构Ubuntu linux版本，只能使用gcc/g++ 4.9.3版本编译器支持的C++语法(支持到C++11)，但是C++14,17语法已经加入了大量新的特性，无论是从语法上带来的诸多编码便捷性，还是新编译器带来的生成二进制质量的提高和性能的提升，都无法享受到。相比有的企业内部C++ 20都已经大行其道，我司开发团队只能使用C++11做开发的实在难以忍受，因此做出改变势在必行。    
&ensp;&ensp;&ensp;&ensp;aiplorer是一个基于gitlab的集成开发平台，提供了两种行泊车系统模块构建方式：一种是不需要依赖远程conan库的构建方式(也是我们推荐的基于aiplorer平台做开发的构建方式), 此种方式我们已经将x86_64架构Ubuntu版本的构建升级到支持gcc/g++ 9.4，可以放心使用C++14,17新特性; 另一种是为了兼顾习惯使用远程conan库的开发者，需要在构建环境配置conan远程库的构建方式，但是这种方式受限于远程conan库，x86_64架构Ubuntu版本的构建依然只能使用gcc/g++ 4.9.3。   
**(具体使用构建方式请查看aiplorer根目录下的[Readme.md](https://git.minieye.tech/ap/aiplorer/-/blob/master/README.md)第4点)**

#### **2. 升级到gcc/gcc+ 9.4** 
&ensp;&ensp;&ensp;&ensp;如果你使用的Ubuntu 20.04系统，那么该系统默认自带的gcc/gcc就是9.4版了(通过gcc --version, g++ --version查看)。   
&ensp;&ensp;&ensp;&ensp;如果你使用的Ubuntu 18.04系统, 那么该系统默认自带的gcc/g++是7.5，但是该编译器不是我们要用的，相信大家都已经安装了gcc/g++ 4.9.3版,这样就需要将编译编译器进行升级，升级编译器有多种途径，可以下载源码在本地编译安装进行升级，这种方式比较耗时，这里推荐使用添加安装源在线安装的方式：   
① 添加gcc/g++ ppa安装源，安装gcc-9 g++-9，安装完成后实际安装的gcc/g++版本就是9.4
```
sudo apt install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install -y gcc-9 g++-9
```
② 安装完gcc/g++ 9.4版后，通过gcc --version, g++ --version查看, 系统默认的编译器依然是旧的，需要切换编译器，因为7.5版编译器用不着可以执行以下命令卸载：   
```
sudo apt remove gcc-7 g++-7
```
③ 这里使用到系统自带的一个工具update-alternatives，极大地方便了同一应用在不同版本之间来回切换, 注意需要用sudo提升到root权限:   
```
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 40
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 40
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90
$ sudo update-alternatives --config gcc
有 2 个候选项可用于替换 gcc (提供 /usr/bin/gcc)。

  选择       路径            优先级  状态
------------------------------------------------------------
  0            /usr/bin/gcc-9     90        自动模式
* 1            /usr/bin/gcc-4.9   40        手动模式
  2            /usr/bin/gcc-9     90        手动模式
要维持当前值[*]请按<回车键>，或者键入选择的编号：2          // 这里数字是需要自己选择，选择1 则会enable gcc-4.9, 选择2 则是gcc-9
$ sudo update-alternatives --config g++                 // 别忘了同样办法设置g++, 后续操作步骤略
...... 
```
       
#### **3. 运行时runtime库**  
&ensp;&ensp;&ensp;&ensp;当前基于aiplorer平台构建(不依赖conan库得方式构建)的结果的build/linux或build/tda4目录已经加入了运行时依赖库文件，跟依赖conan库构建一样放到runtime目录下了，使用方式也一样，在runtime目录下执行:   
```
$ export LD_LIBRARY_PATH=`pwd`
```

#### **4. 有待做或正在做的事项**  
① 与嵌入式使用的各种第三方库版本统一起来   
② 让linux和tda4两边构建所用库版本统一起来   
③ 一键式构建行泊车系统工具 (这里需要各模块负责人保证自己模块能做到一键构建, 而不是在构建前需要run一些环境的脚本或者配置特殊环境)   
④ cmake到bazel的迁移   
⑤ ......   

#### **5. 各模块的版本管理**  
&ensp;&ensp;&ensp;&ensp;当前各模块在构建后会生成一些可执行二进制，或者.so动态库, 比如dfg_apa_cnn模块，编译后会生成libApaCnnModule.so,但是从这个动态库名上看不出版本, 建议做法在cmake里加上版本号的信息:   
```
// 原来cmake的部分，生成的目标库名叫ApaCnnModule
IF(BUILD_SHARED_LIBS)
   ADD_LIBRARY (ApaCnnModule SHARED ${SRC_LIST})
ELSE()
    ADD_LIBRARY (ApaCnnModule STATIC ${SRC_LIST})
ENDIF()
......

// 在CMakeLists.txt尾部添加以下修改版本的cmake内容
IF(BUILD_SHARED_LIBS)
   SET(LIB_VERSION "1.0.3")                   // 此处版本号由开发自己定
   SET(LIB_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/build/${ADD_DIRS}/lib")
   set_target_properties(ApaCnnModule         // 注意这里的target名要跟前面目标库名 ApaCnnModule 保持一致才能关联起来
   PROPERTIES
   LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
   ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
   ) 
   add_custom_command(TARGET ApaCnnModule POST_BUILD
     COMMAND
     mv libApaCnnModule.so libApaCnnModule.so.${LIB_VERSION}
     COMMAND
     ln -s libApaCnnModule.so.${LIB_VERSION} libApaCnnModule.so
     WORKING_DIRECTORY ${LIB_OUTPUT_DIR}
   )
ENDIF()
```
```
wss@Daniel:~/aiplorer/scripts$ ./cmakebuild/single_build.sh tda4 release develop gflags dfg_apa_cnn    
wss@Daniel:~/aiplorer/aipilot/modules/dfg_apa_cnn/build/tda4/lib$ ll
total 300
lrwxrwxrwx 1 wss wss     24 Jun 17 17:25 libApaCnnModule.so -> libApaCnnModule.so.1.0.3*
-rwxrwxr-x 1 wss wss 306152 Jun 17 17:25 libApaCnnModule.so.1.0.3*

```

#### **6. 有问题请提到这里**   
&ensp;&ensp;&ensp;&ensp;aiplroer平台是持续迭代的，目的是服务开发人员高效工作，使用中肯定会有一些不得心应手的地方，比如当前工程中添加了大量submodule，有时候没法下载自己有权限的子模块，但是换了一台电脑又能下载，诸如此类的环境问题也在持续关注和搜集中，如果使用中有问题或者有好的意见可以随时联系@吴师顺，也可以提[问题单](https://alidocs.dingtalk.com/i/team/R2PmK7275VLQWmvp/docs/R2PmKVMEx3LQrXvp# 「aiplorer问题与需求」)   

