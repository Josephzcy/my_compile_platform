# coding=utf-8

import os
import sys
import uuid
import platform
import shutil
import subprocess

# 定义项目的文件目录
from apa_ci_logger import MyLog

ProjDocs = [
    '3rdparty',
    'doc',
    'inc',
    'package',
    'res',
    'src',
    'utils'
]

# 定义当前的文件的目录
G_AiplorerRootDir = ''
G_AiplorerCIConfDir = ''
G_AiplorerCIScriptsDir = ''
G_AiplorerDeployDir = ''
G_AiplorerTargetConfDir = ''
G_AiplorerModulesRootDir = ''
G_AiplorerServicesRootDir = ''

CIConfDirName = 'ci_config'
TargetConfDirName = 'target_config'
CIConfName = [
    'package.json',
    'deploy.json',
    'gitconf.json',
    'check_ignore.json'
]

PathKeyWords = \
    {
        'Windows': '\\',
        'Linux': '/',
        'Darwin': '/'
    }

DeployBinLibDocs = [
    'bin',
    'lib'
]

CDModule = [
    'apa',
    'avm'
]

DeployOtherDocs = [
    'flag',
    'model',
    'param',
    'topic',
    'script'
]

DeployScriptFiles = [
    '_run.sh',
    'proc.sh',
    'run.sh',
    'stop.sh',
    'version.txt'
]

BurnScriptFiles = [
    'burn.py'
]

VersionKey = {
    '_RELEASE_VERSION_': '0',
    '_MAJOR_VERSION_': '0',
    '_MINOR_VERSION_': '0'
}

ModuleElems = [
    # APA
    'havp_odometry',
    'apa_manager',
    'vehicle_control_dds',
    'apa_localization',
    'apa_planning',
    'havp_inference',
    'havp_parkingspace_postprocess',
    'apa_sentinel',
    'apa_ultrasonic_parkingspace',
    'sensor_fusion',
    'vtr',
    'mod',
    # AVP
    'ap_slam',
    'ap_map',
    'map_engine',
    'fake_state_machine'
]

ServiceElems = [
    # APA
    'bird_eye_view',
    'apa_avm_calib',
    'apa_mapping',
    'dfg_avm_calib',
    'dfg_avm_mapping',
    'odom_calibration',
    # AVP
    'camera_service',
    'imu_asensing',
    'imu_gnss_service'
]


class bcolors:
    HEADER = '\033[95m'  # 紫色
    OKBLUE = '\033[94m'  # 蓝色
    OKGREEN = '\033[92m'  # 翠绿
    WARNING = '\033[93m'  # 浅黄色
    FAIL = '\033[91m'  # 红色
    ENDC = '\033[0m'  # 默认值
    BOLD = '\033[1m'  # 加粗
    UNDERLINE = '\033[4m'  # 下斜线


def check_python_version():
    major_var = sys.version_info[0]
    if major_var < 2:
        MyLog.error("The python version is %d.%d. But python 2.x was closed. (Version 3.6 is well tested)\n "
                    "Download it here: https://www.python.org/" % (major_var, sys.version_info[1]))
        return False
    else:
        MyLog.info("Python version = %d.%d.%d" % (sys.version_info[0], sys.version_info[1], sys.version_info[2]))
    return True


def _install_compiler_env():
    cmd_line = "sudo apt install libgfortran4 libleveldb-dev liblmdb-dev"
    ret = os.system(cmd_line)
    if ret != 0:
        exit(1)


def get_a_uuid():
    return "".join(str(uuid.uuid4())).upper()


def cpy_dir(src_path, target_path):
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    files = os.listdir(src_path)
    for f in files:
        if os.path.isdir(src_path + '/' + f):
            tmp_src_path = src_path + '/' + f
            if tmp_src_path.__contains__('_solib') or tmp_src_path.__contains__('external') or tmp_src_path.__contains__('msg'):
                continue
            else:
                cpy_dir(src_path + '/' + f, target_path + '/' + f)
        else:
            shutil.copy(src_path + '/' + f, target_path + '/' + f)


def del_file(file_path, top_down=False):
    cmd = ''
    os_system = platform.system()
    if os_system == "Windows":
        cmd = 'rd /s /q ' + file_path
    else:
        cmd = 'rm -rf ' + file_path
    # print(cmd)
    os.system(cmd)
    MyLog.info("clear %s" % file_path)

    # if os_system == "Windows":
    #    os.system(cmd)
    # elif os_system == "Linux":
    #    # p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    #    # print(p.stdout.readlines())
    #    # retval = p.wait()
    #    try:
    #        res = subprocess.call(cmd)
    #    except Exception as e:
    #        print(e)
    #    print("Clean Files %s" % res)


class EnvBuild(object):
    def __init__(self):
        self.curr_module_path = os.path.dirname(os.path.realpath(__file__))
        self.aiplorer = 'aiplorer'
        self.oa_platform = ''

    def init(self):
        self.oa_platform = platform.system()  # 获取当前的系统，"Windows/Linux"
        self.__get_aiplorer_root_dir()  # 获取当前的root 文件夹

    def __get_aiplorer_root_dir(self):
        global G_AiplorerRootDir
        global G_AiplorerCIConfDir
        global G_AiplorerCIScriptsDir
        global G_AiplorerTargetConfDir
        global G_AiplorerModulesRootDir
        global G_AiplorerServicesRootDir

        module_path = self.curr_module_path
        if not module_path.strip():
            return False
        else:
            pos_path = module_path.rfind(self.aiplorer)
            if pos_path < 0:
                return False
            else:
                G_AiplorerRootDir = module_path[0:pos_path + len(self.aiplorer) + 1]
            pos_path = module_path.rfind('scripts')
            if pos_path < 0:
                return False
            else:
                G_AiplorerCIScriptsDir = module_path[0:pos_path + len('scripts') + 1]
                G_AiplorerCIConfDir = module_path[0:pos_path + len('scripts') + 1] \
                                      + CIConfDirName + PathKeyWords[self.oa_platform]
                G_AiplorerTargetConfDir = module_path[0:pos_path + len('scripts') + 1] \
                                          + TargetConfDirName + PathKeyWords[self.oa_platform]

                G_AiplorerModulesRootDir = G_AiplorerRootDir + 'aipilot/modules/'
                G_AiplorerServicesRootDir = G_AiplorerRootDir + 'aipilot/services/'

        MyLog.info('Start:\n----aiplorer root dir: %s \n----ci_config json dir: %s \n----ci_build_scripts dir: %s \n'
                   '----ci_deploy_config json path: %s \n----modules_root dir: %s \n----services_root dir: %s'
                   % (G_AiplorerRootDir, G_AiplorerCIConfDir, G_AiplorerCIScriptsDir,
                      G_AiplorerTargetConfDir, G_AiplorerModulesRootDir, G_AiplorerServicesRootDir))
        return True
