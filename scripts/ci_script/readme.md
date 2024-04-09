# ci_script

## CI Project for APA
## 介绍

- ci_script python project 为脚本一次构建模块

## 使用指导
- <font size=2 color=red> 1 - 3 章为执行方式 </font>
- <font size=2 color=red> 第 4 章为各个模块开发人员使用（对应以前的 submodules） </font>


## 1、测试：
1、需要安装库 python 3.6 / 如果使用3.9 以上需要：
- sudo apt install python3.9-venv
- pip3 install gitpython

~~2、在 /scripts/ci_script 脚本中执行  python3 main.py~~
~~- 输入: build~~

2、在CI 或者本地直接执行  /scripts/ci_script 下的脚本  
- build : 构建 
- test  : 目前不支持
- deploy: 构建 
- ~~- deploy：部署 python3 main.py deploy~~

## 2.编译要求
- [ ] <font size=3 color=green>python: python3 version>=3.6 / 也可以使用 anacoda python3 env >=3.6</font>
- 如果在 ubuntu18.04 下构建如果需要升级 参考一下的安
### <font size=2 color=green>Linux 安装</font> 
```
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt-get install -y python3.9
sudo cd /usr/bin
sudo rm -r pydoc3
sudo ln -s pydoc3.9 pydoc3
sudo rm -r pygettext3
sudo ln -s pygettext3.9 pygettext3
sudo rm -r python3
sudo ln -s python3.9 python3
** 也可以使用 update-alternatives --install 进行安装，选择相关的 python 版本
```
### <font size=2 color=green>Windows 安装</font>
```
下载安装库，安装并配置环境变量
```

- [ ] <font size=3 color=green>cmake:  cmake version >3.8 </font>

## 3.编译的方法使用方法 （CI 管理人员 和 全部开发人员适用）
- [ ] 支持的命令为以下3种， 其他的命令会报错, 下面为例子和参考命令方式
***
- build
- - <font size=2 color=green> python3 main.py build</font>
- - 拉取编译命令
***
- deploy [deploy_path] 
- - <font size=2 color=green> python3 main.py deploy /home/user/tsf</font>
- - deploy_path 参数为当前 <font size=2 color=red>发布包</font> 的文件夹 /apa 的上面的路径
- - 打包命令

***
-  build deploy [deploy_path]  
- - <font size=2 color=green> python3 main.py build deploy /home/user/tsf</font>
- - deploy_path 参数为当前 <font size=2 color=red>发布包</font> 的文件夹 /apa 的上面的路径
- - 拉取编译 + 打包命令
***

## 4、开发人员（sub modules）如何使用
### <font size=5 color=green> 4.1、目录结构 </font>
- 文件位于 ${User_Dir}/aiplorer/scripts 目录下
```
.
├── cmakebuild
├── ci_script
│   ├── apa_ci.py
│   ├── code_note.md
│   ├── command_parse.py
│   ├── config_json_parse.py
│   ├── env_build.py
│   ├── main.py
│   ├── __pycache__
│   │   ├── apa_ci.cpython-39.pyc
│   │   ├── command_parse.cpython-39.pyc
│   │   ├── config_json_parse.cpython-39.pyc
│   │   └── env_build.cpython-39.pyc
│   └── readme.md
├── ci_config
│   ├── package.json
│   ├── package.json.bak
│   └── package.json.bak.test
└── target_config
    └── deploy.json
```
- ci_script    目录为脚本执行的地方  
- - main.py 为入口执行脚本
- ci_config 目录为需要编译的<font size=3 color=green>子模块开发人员</font> 的配置目录
- - package.json 为 <font size=3 color=red>模块编译</font> 的 json 配置文件<4.2>中详细介绍
- target_config 目录为模块需要打包的<font size=3 color=green>子模块开发人员</font> 的配置目录
- - package.json 为 <font size=3 color=red>发布版本</font> 的 json配置文件<4.2>中详细介绍
----
### <font size=5 color=green> 4.2、配置编译的各个模块参数 </font>
目录为 ${User_Dir}/aiplorer/scripts/ci_config/            文件为名称为：  package.json
### <font size=3 color=red>1、目前只能支持到 component -> submodules -> submodules 的三层嵌套！！！，请不要再深！！！</font>
### <font size=3 color=green>2、参数</font>

  #### <font size=2 color=green> 最外层参数 </font>
```
  "name": "ci_build_config",  -- 固定不边，不需要管
  "version": "1.0.0",         -- 固定不边，不需要管
  "target_arch": "tda4",      -- 编译和打包对应的 arch 有： linux/tda4 两种，后期更新
  "dependencies":             -- 包含下面的子模块
```
  ####  <font size=2 color=green> dependencies 下的 第一层数组的参数 </font>
```
  "component": "dfg_apa"                                    -- 表示当前的 子模块的名称 一定要和 "path" 的 最后的路径名相同     
  "path": "aipilot/modules/dfg_apa",                        -- <font size=2 color=red> 相对于 [aiplorer] </font>工程 路径下的 路径
  "url": "https://git.minieye.tech/wushishun/dfg_apa.git",  -- "component" 模块的 git  地址
  "branch": "feature-dds",                                  -- git 地址中的分支
  "submodules": null                                        -- 有无submodule
```
   ####  <font size=2 color=green> submodules 有值时 第 一层 数据 </font>
```
   "component": "auto_generator",                                -- 表示当前的 sbumodule 名称 ****一定要和 "path" 的 最后的路径名相同***
   "path": "aipilot/modules/path_planning/auto_generator",       -- 相对于 [aiplorer] 工程 路径下的 路径
   "url": "http://39.99.151.9:19089/neptune/auto_generator.git", -- "component" 模块的 git  地址
   "branch": "dev",                                              -- git 地址中的分支
   "build": true,                                                -- 是否编译！！！（和上面是有区别的！！！，请注意）
   "submodules": null                                            -- 有无submodule
```
   ####  <font size=2 color=green> submodules 下面还有 submodules </font>
```
    "component": "path_planning",                                         -- 表示当前的 sbumodule 名称 ****一定要和 "path" 的 最后的路径名相同***
    "path": "aipilot/modules/path_planning/auto_generator/path_planning", -- 相对与 [aiplorer] 工程 路径下的 路径
    "url": "https://git.minieye.tech/fanyu/apa_planning.git",             -- "component" 模块的 git  地址
    "branch": "dev_fanyu_checksim",                                       -- git 地址中的分支
    "build": true,                                                        -- 是否编译！！！（和上面是有区别的！！！，请注意）
    "submodules":null                                                     -- 这一层一定要是 null ！！！！！！
```
- ### <font size=3 color=green>3、例子</font>
- <font size=2 color=green> 3.1  无 submodule 例子 </font>
```
{
  "name": "ci_build_config",
  "version": "1.0.0",
  "target_arch": "tda4",
  "dependencies":
  	[
	   	{
			"component": "dfg_apa",
			"path": "aipilot/modules/dfg_apa",
			"url": "https://git.minieye.tech/wushishun/dfg_apa.git",
			"branch": "feature-dds",
			"submodules": null
	    }
	]
}
```
- <font size=2 color=green> 3.2  一层 submodule 例子 </font>
```
{
  "name": "ci_config",
  "version": "1.0.0",
  "target_arch": "tda4",
  "dependencies":
  	[
	   	{
			"component": "path_planning",
			"path": "aipilot/modules/path_planning",
			"url": "https://git.minieye.tech/fanyu/apa_planning.git",
			"branch": "dev_fanyu_checksim",
			"submodules":
			[
				{
				"component": "auto_generator",
				"path": "aipilot/modules/path_planning/auto_generator",
				"url": "http://39.99.151.9:19089/neptune/auto_generator.git",
				"branch": "dev",
				"build": true,
				"submodules": 
				  [
					{
						"component": "path_planning",
						"path": "aipilot/modules/path_planning/auto_generator/path_planning",
						"url": "https://git.minieye.tech/fanyu/apa_planning.git",
						"branch": "dev_fanyu_checksim",
						"build": true,
						"submodules":null
					}
				  ]
				}
			]
	    }
	]
}
```
- <font size=2 color=green> 3.3 两层 submodules 例子 </font>
```
{
  "name": "ci_config",
  "version": "1.0.0",
  "target_arch": "tda4",
  "dependencies":
  	[
	   	{
			"component": "path_planning",
			"path": "aipilot/modules/path_planning",
			"url": "https://git.minieye.tech/fanyu/apa_planning.git",
			"branch": "dev_fanyu_checksim",
			"submodules":
			[
				{
				"component": "auto_generator",
				"path": "aipilot/modules/path_planning/auto_generator",
				"url": "http://39.99.151.9:19089/neptune/auto_generator.git",
				"branch": "dev",
				"build": true,
				"submodules": 
				  [
					{
						"component": "path_planning",
						"path": "aipilot/modules/path_planning/auto_generator/path_planning",
						"url": "https://git.minieye.tech/fanyu/apa_planning.git",
						"branch": "dev_fanyu_checksim",
						"build": true,
						"submodules":null
					}
				  ]
				}
			]
	    }
	]
}
```
----

### <font size=5 color=green> 4.3、配置编译的各个模块发布参数 </font>
目录为 ${User_Dir}/aiplorer/scripts/target_config/            文件为名称为：  deploy.json

### <font size=3 color=green>1、参数</font>
  ####  <font size=2 color=green> 最外层参数 </font>
```
  "name": "ci_deploy_config",  -- 固定不边，不需要管
  "version": "1.0.0",         -- 固定不边，不需要管
  "target_arch": "tda4",      -- 打包对应的 arch 有： linux/tda4 两种，一定要和 package.json 一样！！！！！ 
  "dist_deploy_root":         -- "app/algo/apa/", 这是个 相对于 命令 deploy [deploy_path] 中  deploy_path 的 下层路径
  "deploy_elements":          -- 包含要打包的元素
```
  ####  <font size=2 color=green> deploy_elements 中的参数 </font>
```
"component": "dfg_apa",
"targe":                --  dfg_apa 模块的打包目标
```
####  <font size=2 color=green> target 中的参数 </font>
```
 "bin": [ "bin_1_name", "bin_2_name","bin_3_name","bin_3_name"]   -- 模块在build中生成的bin， 放置在打包文件中 bin 目录下的文件
 "lib":  [ "name.so", "name1.so","name2.so","name3.so"]           -- 模块在build中生成的lib， 放置在打包文件中lib 目录下的文件
 "config":[ "config_1_name", "config_2_name"]                     -- 模块在build中生成的配置文件， 放置在打包文件中 config 目录下的文件
 "model": [ "config_1_name", "config_2_name"]                     -- 模块在build中生成的模型文件， 放置在打包文件中 model 目录下的文件
```
### <font size=3 color=green>2、例子</font>

```
{
  "name": "ci_deploy_config",
  "version": "1.0.0",
  "target_arch": "tda4",
  "dist_deploy_root": "app/algo/apa/",
  "deploy_elements":
    [
      {
        "component": "dfg_apa",
        "target":
          {
            "bin":
              [
                "apa_postprocess",
                "apa_seg"
              ],
            "lib":
              [
                "libapa_dds.so",
                "libdds_proto.so"
              ],
            "config": null,
            "model": null
          }
      }
    ]
}
```
----
## Roadmap
- 待完善

## Contributing
- 待完善

## Project status
- V1.0.0 beta
