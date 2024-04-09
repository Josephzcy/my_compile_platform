# -*- coding: utf-8 -*-
import os
import env_build
from config_json_parse import ConfigJsonParse
from apa_ci_logger import MyLog

BuildScriptDir = "cmakebuild/cmake/deps.tda4.cmake"


def cmd_copy(src_path, dist_path):
    _cmd = "cp -rf " + src_path + " " + dist_path
    MyLog.info('cp %s to %s' % (src_path, dist_path))
    if os.system(_cmd) == 0:
        return True
    else:
        return False


class PKGComm(object):
    def __init__(self):
        self.conf_json_parse = ConfigJsonParse()
        self.aiplorer_dir = env_build.G_AiplorerRootDir
        self.deploy_arch = self.conf_json_parse.get_deploy_target_arch()
        self.cmake_libcom_path = self.__get_deps_path(self.deploy_arch)
        MyLog.info("----com3rd lib conf file: %s" % self.cmake_libcom_path)
        self.deploy_dist_deploy_root = self.conf_json_parse.get_deploy_dist_deploy_root() + "lib/"
        MyLog.info("----com3rd lib targe dir: %s" % self.deploy_dist_deploy_root)
        self.add_flag = False
        self.all_lib_path_dict = dict()

    def __get_deps_path(self, arch):
        return env_build.G_AiplorerCIScriptsDir + 'cmakebuild/cmake/deps.' + self.deploy_arch.lower() + '.cmake'

    def get_pkg_com_lib(self, _dir):
        self.__parse_lib_files()
        for k, v in self.all_lib_path_dict.items():
            if not cmd_copy(k, _dir + '/' + v):
                MyLog.error("\033[94mError: not copy the %s \033[0m" % k)
                exit(1)

    def __add_path_to_all_path_list(self, line):
        pos_path = line.find("/")
        if pos_path > 0:

            root_type = line[0:pos_path]
            if root_type.__contains__("${TDA4_SYS_ROOT}"):
                MyLog.info("root_type = %s" % root_type)
                return

            if root_type.__contains__("${J3_SYS_ROOT}"):
                MyLog.info("root_type = %s" % root_type)
                return

            sub_path = line[pos_path + 1:len(line)]
            path = env_build.G_AiplorerRootDir + sub_path
            path = path.replace('\n', '')
            for file in os.listdir(path):
                file_path = os.path.join(path, file)
                if os.path.isfile(file_path):
                    if ".so" in file_path:
                        self.all_lib_path_dict[file_path] = self.deploy_dist_deploy_root + file

    def __parse_lib_files(self):
        file_object1 = open(self.cmake_libcom_path, 'r')
        try:
            while True:
                line = file_object1.readline()
                if line:
                    if 'LINK_DIRECTORIES(' in line:
                        self.add_flag = True
                    if self.add_flag:
                        self.__add_path_to_all_path_list(line)
                else:
                    break
        finally:
            file_object1.close()
