1：设置两个文件夹，一个为ci_script 一个为ci_config
==> finished and ok 0610, git push svr ok

2：在ci_config的配置文件中确定相关的 json 字段的意义。
==> beta 版本 0612 git push to git svr

3：在ci_script中创建主程序和相关子模块处理一键打包的功能：
3.1：主程序开启的 Env 中 加入 ConfigParse，对ci_config/package.json配置文件进行完全的解析
==> will create the py file 

3,2：在主程序环境读取完成以后，在command_parse 加入CmdParse 对为外部的命令的解析

3.3：通过不同的command_parse 来对不同的CI Handle 处理

3.4：可以一键打包相关的全部系统部分。