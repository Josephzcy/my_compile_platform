# coding=utf-8
import json
import env_build
from collections import defaultdict


class ConfigJsonParse(object):
    def __init__(self):
        self.__git_user_conf_json_path = env_build.G_AiplorerCIConfDir + env_build.CIConfName[2]
        self.git_user = ''
        self.git_passwd = ''
        self.token = ''
        self.use_https = False
        self.__user_read_json()
        # region == build json ==
        self.__cppcheck_ignore_conf_path = env_build.G_AiplorerCIConfDir + env_build.CIConfName[3]
        self.__check_ignore_elements = defaultdict(list)
        self.__cppcheck_read_json()
        # endregion == build json ==
        # region == build json ==
        # self.__build_conf_json_path = env_build.G_AiplorerCIConfDir + env_build.CIConfName[0]
        self.__build_conf_json_path = ''
        self.__get_build_json()
        self.__build_depends_modules = defaultdict(list)
        self.__build_ci_version_ = ''
        self.__build_target_arch = ''
        self.__build_master_branch = ''
        self.__build_read_json()
        # endregion == build json ==
        # region == deploy json ==
        self.__deploy_conf_json_path = env_build.G_AiplorerTargetConfDir + env_build.CIConfName[1]
        self.__deploy_elements = defaultdict(list)
        self.__deploy_ci_version_ = ''
        self.__deploy_target_arch = ''
        self.__deploy_master_branch = ''
        self.__deploy_dist_deploy_root = ''
        self.__deploy_read_json()
        # endregion == deploy json ==

    def __user_read_json(self):
        tmp = json.load(open(self.__git_user_conf_json_path, 'r', encoding="utf-8"))
        self.git_user = tmp['user']
        self.git_passwd = tmp['passwd']
        self.token = tmp['token']
        self.enable = tmp['enable']
        if len(self.git_user) > 0:
            self.use_https = True

    def get_git_user(self):
        return self.git_user

    def get_git_passwd(self):
        return self.git_passwd

    def get_git_token(self):
        return self.token

    def get_git_use_enable(self):
        return self.enable

    # region ===== build  =========
    def __get_build_json(self):
        if not self.use_https:
            self.__build_conf_json_path = env_build.G_AiplorerCIConfDir + env_build.CIConfName[0]
        else:
            self.__build_conf_json_path = env_build.G_AiplorerCIConfDir + 'package.https.json'

    def __build_read_json(self):
        tmp = json.load(open(self.__build_conf_json_path, 'r', encoding="utf-8"))
        self.__build_depends_modules = tmp['dependencies']
        self.__build_ci_version_ = tmp['version']
        self.__build_target_arch = tmp['target_arch']

    def get_build_parse_conf_path(self):
        return self.__build_conf_json_path

    def get_build_ci_version(self):
        return self.__build_ci_version_

    def get_build_target_arch(self):
        return self.__build_target_arch

    def get_build_depends_modules(self):
        return self.__build_depends_modules

    # endregion ===== build  =========

    # region ===== deploy  =========
    def __deploy_read_json(self):
        tmp = json.load(open(self.__deploy_conf_json_path, 'r', encoding="utf-8"))
        self.__deploy_elements = tmp['deploy_elements']
        self.__deploy_ci_version_ = tmp['version']
        self.__deploy_target_arch = tmp['target_arch']
        self.__deploy_dist_deploy_root = tmp['dist_deploy_root']
        self.__deploy_master_branch = tmp['master_branch']

    def get_deploy_parse_conf_path(self):
        return self.__deploy_conf_json_path

    def get_deploy_dist_deploy_root(self):
        return self.__deploy_dist_deploy_root

    def get_deploy_master_branch(self):
        return self.__deploy_master_branch

    def get_deploy_target_arch(self):
        return self.__deploy_target_arch

    def get_deploy_ci_version(self):
        return self.__deploy_ci_version_

    def get_deploy_elements(self):
        return self.__deploy_elements

    # endregion ===== deploy  =========

    # region ===== cppcheck  =========
    def __cppcheck_read_json(self):
        tmp = json.load(open(self.__cppcheck_ignore_conf_path, 'r', encoding="utf-8"))
        self.__check_ignore_elements = tmp['elements']

    def get_cppcheck_ignore_elements(self):
        return self.__check_ignore_elements

    # endregion ===== cppcheck  =========
