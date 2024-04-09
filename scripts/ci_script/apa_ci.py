# sudo apt install python3.9-venv
# pip3 install gitpython
import os
import pathlib
import configparser
import time
import datetime
import subprocess
from builtins import print

import git
from apa_ci_logger import MyLog
from pkg_com3rd import PKGComm
from config_json_parse import ConfigJsonParse
from git.repo import Repo
import platform
import env_build
from urllib import parse

HTTPS = 'https://'
HTTP = 'http://'
GITUSER = 'zhangjia'
GITKEY = 'zhangjia'

Per_Counter = 0
End_Counter = 50
START = time.perf_counter()


def init_process():
    global End_Counter
    global START
    End_Counter = 50
    START = time.perf_counter()


def print_progess(progress, message=''):
    global End_Counter
    global START
    a_int = int(int(progress) / 2)
    finish = "=" * a_int
    need_do = "-" * (End_Counter - a_int)
    dur = time.perf_counter() - START
    print("\r[{:^3.0f}%][{:.2f}s][{}{}] {}".format(progress, dur, finish, need_do, message), end="")


#  used the build command to build
class Progress(git.remote.RemoteProgress):
    def update(self, op_code, cur_count, max_count=None, message=''):
        progress = (cur_count / max_count) * 100
        # print_progess(progress, message)


class ApaCi(object):
    def __init__(self):
        self.conf_json_parse = ConfigJsonParse()
        self.oa_platform = platform.system()
        self.git_user = self.conf_json_parse.get_git_user()
        self.git_passwd = self.conf_json_parse.get_git_passwd()
        self.token = self.conf_json_parse.get_git_token()
        self.git_use_en = self.conf_json_parse.get_git_use_enable()
        # region build params
        self.build_target_arch = self.conf_json_parse.get_build_target_arch()
        self.build_version = self.conf_json_parse.get_build_ci_version()
        self.build_dependencies = self.conf_json_parse.get_build_depends_modules()
        self.build_main_component = ''
        self.build_doc = ''
        self.build_total_cmd = ''
        # endregion build params
        # region deploy params
        self.deploy_dist_deploy_root = self.conf_json_parse.get_deploy_dist_deploy_root()
        self.deploy_target_arch = self.conf_json_parse.get_deploy_target_arch()
        self.deploy_version = self.conf_json_parse.get_deploy_ci_version()
        self.deploy_elements = self.conf_json_parse.get_deploy_elements()
        self.deploy_master_branch = self.conf_json_parse.get_deploy_master_branch()
        # endregion deploy params
        # region deploy params
        self.cppcheck_ignore_elements = self.conf_json_parse.get_cppcheck_ignore_elements()
        # endregion deploy params
        # topic.json
        self.deploy_topic_src_path = env_build.G_AiplorerRootDir + 'msg/topic.json'
        # version
        self.modules_root_dir = env_build.G_AiplorerModulesRootDir
        self.services_root_dir = env_build.G_AiplorerServicesRootDir
        self.dt_ms = datetime.datetime.now(datetime.timezone(datetime.timedelta(hours=+8))).strftime('%Y%m%d%H%M%S')
        self.version_dict = {}
        self.deploy_ver_path = ''
        # burn path
        self.burn_scripts_path = env_build.G_AiplorerCIScriptsDir + 'publish/burn.py'
        self.burn_key_scripts_path = env_build.G_AiplorerCIScriptsDir + 'publish/burn_key.py'
        # debug key path
        self.debug_key_path = env_build.G_AiplorerCIScriptsDir + 'publish/debug.key'
        # test
        self.pkgcomm = PKGComm()
        # path && build_model
        self.main_build_model = ''
        self.main_path = ''
        self.main_target_name = None
        self.sub_build_model = ''
        self.sub_path = ''
        self.sub_target_name = None
        self.sub_sub_build_model = ''
        self.sub_sub_path = ''
        self.sub_sub_target_name = None
        # bazel
        self.cur_bin_cache_dir = '/builds/ap/aiplorer/collect'
        self.prefix = ''
        # service
        self.service_elems = []

    def __clone_clear_component_cache(self, component, path):
        local_path = env_build.G_AiplorerRootDir + path.replace('/', env_build.PathKeyWords[self.oa_platform])
        MyLog.info('--Clear [%s] cache' % component)
        try:
            if os.path.exists(local_path):
                env_build.del_file(local_path, True)  # 删除文件
        except Exception as e:
            MyLog.error("ERROR: clear [%s] cache error and exit" % component)
            MyLog.error("Exception: %s" % e)
            MyLog.info('Finished Build All Component [%s] ERROR------------------------------------' % component)
            exit(1)
        return local_path

    def __clone_url_user_key(self, url):
        _url = ''
        pos_path = url.rfind(HTTPS)
        if pos_path == 0:
            if self.git_use_en:
                if len(self.git_user) == 0:
                    _url = HTTPS + url[len(HTTPS):len(url)]
                else:
                    _url = HTTPS + parse.quote(self.git_user) + ':' + parse.quote(self.git_passwd) \
                           + '@' + url[len(HTTPS):len(url)]
                return _url
            else:
                return url

        pos_path = url.rfind(HTTP)
        if pos_path == 0:
            if self.git_use_en:
                if len(self.git_user) == 0:
                    _url = HTTP + url[len(HTTP):len(url)]
                else:
                    _url = HTTP + parse.quote(self.git_user) + ':' + parse.quote(self.git_passwd) \
                           + '@' + url[len(HTTP):len(url)]
                return _url
            else:
                return url

        return url

    def __clone_git_clone_component(self, component, path, url, branch):
        # 如果是服务，退出
        if branch is None:
            return

        # clear submodule cache and clone the repo
        local_path = self.__clone_clear_component_cache(component, path)
        MyLog.info('Git Clone [\033[95m%s\033[0m] branch [\033[95m%s\033[0m] to path [\033[95m%s\033[0m] ' % (component, branch, local_path))
        init_process()
        try:
            Repo.clone_from(self.__clone_url_user_key(url), to_path=local_path, branch=branch, progress=Progress(), multi_options=['--depth=1'])
            # print('Finished...')
        except Exception as e:
            MyLog.error("\033[91mERROR: clone [%s] error and exit" % component)
            MyLog.error("Exception: %s" % e)
            MyLog.error('Finished CPP CheckBuild All Component ERROR-------\033[0m')
            exit(1)

    # region =================Build==========================
    def __build_cmd(self, component, layout=0):
        build_sh_pat = env_build.G_AiplorerCIScriptsDir  # +
        if not os.path.exists(build_sh_pat):
            MyLog.error('--ERROR: sh build not exist')
            exit(1)

        path = ''
        bm = ''
        tn = None
        tn_cnt = 0
        cur_component = component
        if layout == 0:
            bm = self.main_build_model
            path = self.main_path            
            if self.main_target_name is not None:
                tn = self.main_target_name
                tn_cnt= len(tn)
                MyLog.info('~~~~~~~~~~~~~\033[95m include %s targets: %s \033[0m~~~~~~~~~~~~' % (tn_cnt, tn))
        if layout == 1:
            bm = self.sub_build_model
            tn = self.sub_target_name
            if tn is not None:
                tn_cnt= len(tn)
            if bm == 'bazel':
                path = self.main_path + '/' + component
            else:
                path = self.sub_path
                cur_component = self.build_main_component + '/' + component
        if layout == 2:
            bm = self.sub_sub_build_model
            path = self.sub_sub_path
            tn = self.sub_sub_target_name
            if tn is not None:
                tn_cnt= len(tn)

            self.prefix = ''
        #if self.build_target_arch == 'linux':
        #    self.prefix = '_linux'

        if bm == "bazel":
            cd_cmd = 'cd ' + env_build.G_AiplorerRootDir
            sh_cmd = ' && chmod +x ' + 'scripts/bazelbuild/build.sh'
            if tn_cnt == 0:
                build_cmd = ' && ' + './scripts/bazelbuild/build.sh ' + self.build_target_arch + ' release ' + \
                            '//' + path + ':' + cur_component + self.prefix
            elif tn_cnt > 0:
                build_cmd = ''
                for t in tn:
                    build_cmd += ' && ' + './scripts/bazelbuild/build.sh ' + self.build_target_arch + ' release ' + \
                                 '//' + path + ':' + t + self.prefix
            self.build_total_cmd = cd_cmd + sh_cmd + build_cmd
            self.main_target_name = None
            self.sub_target_name = None
            self.sub_sub_target_name = None
        else:
            cd_cmd = 'cd ' + env_build.G_AiplorerCIScriptsDir
            sh_cmd = ' && chmod +x ' + 'cmakebuild/single_build.sh'
            build_extern = ' release cicd gflags '
            build_cmd = ' && ' + './cmakebuild/single_build.sh ' + \
                        self.build_target_arch + build_extern + cur_component
            self.build_total_cmd = cd_cmd + sh_cmd + build_cmd

        MyLog.info(self.build_total_cmd)
        ret = os.system(self.build_total_cmd)
        if ret != 0:
            exit(1)

    def __build_previous(self, m_c_name, sub_name, sub_sub_name):
        if sub_sub_name is not None:
            MyLog.info('Build Component [%s]-sub[%s]-sub[%s] build......' % (m_c_name, sub_name, sub_sub_name))
            # self.__build_cmd(sub_sub_name, 2)
        else:
            if sub_name is not None:
                MyLog.info('Build Component [%s]-sub [%s] building......' % (m_c_name, sub_name))
                self.__build_cmd(sub_name, 1)
            else:
                if m_c_name is not None:
                    MyLog.info('Build Component [%s] building......' % m_c_name)
                    self.__build_cmd(m_c_name, 0)

    # check the module files and delete
    # 最内层的 submodules
    def __build_check_submodules_exist(self, submodules):
        if submodules is None:
            return False
        else:
            if len(submodules) == 0:
                return False
        return True

    def __build_print_s(self, m_c, s_c, path, url, br, bm, build, sub, level, flag='Build'):
        submodule_name = []
        if level == 0:
            MyLog.info('----------\033[94m %s Component [ %s ] Starting \033[0m------------' % (flag, m_c))
        elif level == 1:
            MyLog.info('----------\033[94m %s Component [ %s ] Submodule [ %s ] Starting \033[0m------------'
                       % (flag, m_c, s_c))
        elif level == 2:
            MyLog.info('----------\033[94m %s Component [%s] Submodule [ %s ] Submodule [ %s ] Starting \033[0m'
                       '------------' % (flag, self.build_main_component, m_c, s_c))
        MyLog.info('--path: %s' % path)
        MyLog.info('--url: %s' % url)
        MyLog.info('--branch: %s' % br)
        if bm is not None:
            MyLog.info('--build model: %s' % bm)
        else:
            MyLog.info('--build model: cmake')
        if build is not None:
            MyLog.info('--build: %s' % build)
        if sub is None:
            MyLog.info('--submodules: %s' % sub)
        else:
            for _vl in sub:
                submodule_name.append(_vl['component'])
            MyLog.info('--submodules has : %s' % submodule_name)

    # 外层 main
    def __build_pre_submodules_main(self, component, submodules):
        if not self.__build_check_submodules_exist(submodules):
            MyLog.info('\n[%s] have no submodule' % component)
            return False

        for value_1 in submodules:
            self.__build_print_s(component, value_1['component'], value_1['path'], value_1['url'], value_1['branch'],
                                 value_1['build_model'], value_1['build'], value_1['submodules'], 1)
            # clone 当前的git rep
            self.__clone_git_clone_component(value_1['component'], value_1['path'], value_1['url'],
                                             value_1['branch'])                                 
            # 最内部的 submodules
            if self.__build_check_submodules_exist(value_1['submodules']):
                submodule = value_1['submodules']
                for value_2 in submodule:
                    self.__build_print_s(value_1['component'], value_2['component'], value_2['path'], value_2['url'],
                                         value_2['branch'], value_2['build_model'], value_2['build'],
                                         value_2['submodules'], 2)
                    # git clone 获取当前的 git 数据
                    self.__clone_git_clone_component(value_2['component'], value_2['path'], value_2['url'],
                                                     value_2['branch'])                                         
                    # 进行编译
                    if value_2['build'] is True:
                        self.sub_sub_build_model = value_2['build_model']
                        if "target_name" in value_2:
                            self.sub_sub_target_name = value_2['target_name']
                        self.__build_previous(self.build_main_component, value_1['component'], value_2['component'])

            # 进行编译
            if value_1['build'] is True:
                self.sub_build_model = value_1['build_model']
                if "target_name" in value_1:
                    self.sub_target_name = value_1['target_name']
                self.__build_previous(self.build_main_component, value_1['component'], None)

    # build最外部的组成的component
    def __build_pre_component(self, component, path, url, branch, submodules):
        # 判断 submodules 状态 并进入内部
        # git clone 获取当前的 git 数据
        self.__clone_git_clone_component(component, path, url, branch)
        self.__build_pre_submodules_main(component, submodules)

    # build 全部的模块
    def __build_all_component(self):
        for module in self.build_dependencies:
            self.build_main_component = module['component']
            self.main_build_model = module['build_model']
            self.main_path = module['path']
            if "target_name" in module:
                self.main_target_name = module['target_name']
            MyLog.info('\033[94m================ Build Component [%s] Starting ==================\033[0m'
                       % module['component'])
            self.__build_print_s(self.build_main_component, None, module['path'], module['url'], module['branch'],
                                 module['build_model'], None, module['submodules'], 0)
            self.__build_pre_component(module['component'], module['path'], module['url'], module['branch'],
                                       module['submodules'])
            self.__build_previous(self.build_main_component, None, None)
            MyLog.info('\033[92m================ Build Component [%s] Finished ==================\033[0m' % module['component'])

    # bazel files patch
    def __copy_bazel_bin(self):
        rm_cmd = 'mkdir -p collect/_bazel_root collect/aipilot  collect/code ; rm -rf collect/aipilot  collect/code ;'
        ret = os.system(rm_cmd)
        bazel_bin_path = env_build.G_AiplorerRootDir + '/bazel-bin'
        if os.path.exists(bazel_bin_path):
            #check_path_cmd = "cd " + bazel_bin_path + " && pwd -P"
            #p1 = subprocess.run(check_path_cmd, shell=True, encoding='utf-8', stdout=subprocess.PIPE)
            #if p1.returncode == 0:
                #bin_cache_path = p1.stdout.rsplit()[0]
                #MyLog.info('check the bazel-bin = %s ' % bin_cache_path)
                #rm_cmd = 'cp -rf ' + bin_cache_path + '/aipilot ' + self.cur_bin_cache_dir + '/ ;'
            rm_cmd = 'find bazel-bin -name "*.params" -o -name "*.runfiles*" -o -name "_objs" -o -name "*.o" -o -name "*.a" |xargs rm -rf ;'             
            ret = os.system(rm_cmd)
            #MyLog.info('To be exec [%s]' % rm_cmd)
        #backup param & config & version.h .etc to cache dir for both bazel and cmake build
        rm_cmd = 'find aipilot -name "\.git" -o -name "old_model" -o -name "*.c*" -o -name "*.hpp" -o -name "*.o"|xargs rm -rf ;'
        ret = os.system(rm_cmd)

        rm_cmd = 'mv aipilot collect/code && sync && du -sh collect/*; '
        ret = os.system(rm_cmd)
        if ret != 0:
            MyLog.error('\033[91m [%s] Fail\033[0m' % rm_cmd)
            exit(1)

    def build(self):
        MyLog.info('ci apa version = [%s] target_arch = [%s]  will be building ...' % (self.build_version,
                                                                                       self.build_target_arch))
        MyLog.info('\033[94mStarting Build All Component---------------\033[0m')
        self.__build_all_component()
        self.__copy_bazel_bin()
        MyLog.info('\033[94mFinished Build All Component----------------\033[0m')

    # endregion =================Build==========================

    # region =================Deploy==========================
    def __check_deployDocs(self, key):
        for doc in env_build.DeployBinLibDocs:
            if doc == key:
                return 0

        for doc in env_build.DeployOtherDocs:
            if doc == key:
                return 1

        return 1 # -1 ->1

    def __return_deploy_source_dir(self, comp_value, build_model, m_path, dir_name):
        _res_src_dir = ''
        if comp_value == 0:  # module
            if build_model == "bazel":
                _res_src_dir = 'bazel-bin/' + m_path
            else:
                _res_src_dir = env_build.G_AiplorerRootDir + m_path + '/build/' + self.build_target_arch \
                               + '/' + dir_name
        elif comp_value == 1:  # service
            _res_src_dir = env_build.G_AiplorerRootDir + m_path + dir_name

        return _res_src_dir

    def __deploy_get_source_and_target_dir(self, component, key=None, values=None, algo_root=None):
        _dirs = []
        _build_src_dir = ''
        if key is None:
            return []

        for module in self.build_dependencies:
            if module['component'] == component:
                build_model = module['build_model']
                comp_value = self.__check_deployDocs(key)
                pre_deploy_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root
                # 本地编译的文件路劲 bin/lib
                if comp_value == 0:
                    _build_src_dir = self.__return_deploy_source_dir(comp_value, build_model, module['path'], key)
                    _dirs.append(_build_src_dir)
                elif comp_value == 1:
                    if values is not None:
                        _build_src_dir = self.__return_deploy_source_dir(comp_value, build_model, module['path'],
                                                                         values['src_path'])
                        _dirs.append(_build_src_dir)
                else:
                    return []

                # 打包的路径
                #if self.deploy_master_branch.__contains__(env_build.CDModule[1]):
                if algo_root is not None:              
                    if key == 'script':
                        _deploy_dir = env_build.G_AiplorerDeployDir + '/' + algo_root
                    else:
                        _deploy_dir = env_build.G_AiplorerDeployDir + '/' + algo_root + '/' + key
                else:
                    _deploy_dir = pre_deploy_dir + key

                _dirs.append(_deploy_dir)

        return _dirs

    def __create_target_dir(self, deploy_root_dir, member):
        # tmp_member = member
        # tmp_root = deploy_root_dir
        # pos_path = tmp_member.find('/')

        # while pos_path > 0:
        #     tmp_root += '/' + tmp_member[0:pos_path]
        #     MyLog.info('tmp_root = %s' % tmp_root)
        #     tmp_member = tmp_member[pos_path + 1:]
        #     MyLog.info('tmp_member = %s' % tmp_member)
        #     if not os.path.exists(tmp_root):
        #         if os.system('mkdir %s' % tmp_root) != 0:
        #             return False
        #     else:
        #         if not os.path.isdir(tmp_root):
        #             return False
        #     pos_path = tmp_member.find('/')

        if not os.path.exists(deploy_root_dir):
            if os.system('mkdir -p %s' % deploy_root_dir) != 0:
                return False      

        return True

    def __op_version_text(self, path):
        MyLog.info(" Op Version")
        config = configparser.ConfigParser()
        config.read(path)
        # 添加section
        if not config.has_section("APA_SUB"):
            config.add_section("APA_SUB")
        # 添加option并设置值，只能是string值
        for key, values in self.version_dict.items():
            MyLog.info("bin: %s ver: %s" % (key, values))
            config.set("APA_SUB", key, values)

        # 添加大版本
        version = 'APA_T1Q3' + '_PR_' + 'V' + self.deploy_version + '_' + self.dt_ms
        if not config.has_section("APA_PKG"):
            config.add_section("APA_PKG")
        config.set("APA_PKG", "SP", version)

        # 写入ini文件，注意写入的mode会影响是否覆盖ini文件
        with open(path, "w", encoding="utf8") as f:
            config.write(f)

    # run.sh and proc.sh deploy in the pkg
    def __deploy_script(self):
        build_scripts_dir = env_build.G_AiplorerCIScriptsDir
        deploy_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root
        MyLog.info('--build_scripts_dir : %s' % build_scripts_dir)
        MyLog.info('--deploy_dir : %s' % deploy_dir)
        MyLog.info('--burn_script_path : %s' % self.burn_scripts_path)
        MyLog.info('--ssh_debug_key_path : %s' % self.debug_key_path)
        for file in env_build.DeployScriptFiles:
            build_scripts_path = build_scripts_dir + 'publish/' + file
            deploy_path = deploy_dir + file

            if file == 'version.txt':
                self.deploy_ver_path = deploy_path

            ret = os.system('cp -rf %s %s' % (build_scripts_path, deploy_path))
            if ret == 0:
                MyLog.info('[%s] OK:   copy %s to %s ' % (file, build_scripts_path, deploy_path))
            else:
                MyLog.error('[%s] Fail: copy %s to %s ' % (file, build_scripts_path, deploy_path))
                exit(1)
            # add the +x to .sh file
            if deploy_path.__contains__(".sh"):
                os.system('chmod +x %s' % deploy_path)

    # get per component all version
    def __get_component_version(self, _component_name, _type, ver_path):
        component_ver_path = ''
        if _type == 'module':
            component_ver_path = self.modules_root_dir + _component_name + ver_path
        if _type == 'service':
            component_ver_path = self.services_root_dir + _component_name + ver_path
            MyLog.error(' services_root_dir: [%s] component:[%s]  path = [%s] ' % (self.services_root_dir, _component_name, component_ver_path))
        if not os.path.exists(component_ver_path):
            MyLog.error('Error: get [%s] version Fail , path = %s ' % (_component_name, component_ver_path))
            #os.system('tree -L 3 aipilot/modules')
            exit(1)
        MyLog.info('ver_path: %s' % component_ver_path)
        MyLog.info('_type: %s' % _type)
        with open(component_ver_path, 'r', encoding='utf-8') as fs:
            data_lines = fs.readlines()
            for data in data_lines:
                for key in env_build.VersionKey.keys():
                    pos = data.find(key)
                    if pos > 0:
                        _ver = data[pos + len(key):len(data)].strip()
                        if _ver.isalnum():
                            env_build.VersionKey[key] = data[pos + len(key):len(data)].strip()  # Release
                        MyLog.info('%s Version: %s' % (key, env_build.VersionKey[key]))

        self.version_dict[_component_name] = env_build.VersionKey['_RELEASE_VERSION_'] + '.' \
                                             + env_build.VersionKey['_MAJOR_VERSION_'] + '.' + \
                                             env_build.VersionKey['_MINOR_VERSION_']

    # deploy the topic.json to topic doc
    def __deploy_topic(self):
        build_topic_dir = env_build.G_AiplorerRootDir + 'msg'
        deploy_topic_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root + 'topic'
        MyLog.info('--msg_topic_dir : %s' % build_topic_dir)
        MyLog.info('--deploy_dir : %s' % deploy_topic_dir)
        MyLog.info('--topic_json_path : %s' % self.deploy_topic_src_path)
        deploy_topic_dist_path = deploy_topic_dir + '/' + 'topic.json'
        ret = os.system('cp -rf %s %s' % (self.deploy_topic_src_path, deploy_topic_dist_path))
        if ret == 0:
            MyLog.info('[%s] OK:   copy %s to %s ' % ('topic', self.deploy_topic_src_path, deploy_topic_dist_path))
        else:
            MyLog.error('[%s] Fail: copy %s to %s ' % ('topic', self.deploy_topic_src_path, deploy_topic_dist_path))
            exit(1)

    # deploy the pre component
    def __deploy_pre_component(self, key, build_dir, deploy_dir, member):
        MyLog.info('deploy_dir:[%s] member:[%s]' % (deploy_dir, member))        
        build_path = build_dir + '/' + member
        if not self.__create_target_dir(deploy_dir, member):
            return False

        deploy_path = deploy_dir + '/' + member

        ret = os.system('cp -rf %s %s' % (build_path, deploy_dir))
        if ret == 0:
            MyLog.info('[%s]-[%s] OK:   copy %s to %s ' % (key, member, build_path, deploy_dir))
            if deploy_path.__contains__(".sh"):  # add the +x to .sh file
                os.system('chmod +x %s' % deploy_path)
                MyLog.info('[%s]-[%s] :  set the +x to %s ' % (key, member, deploy_path))
            return True
        else:
            MyLog.error('[%s]-[%s] Fail: copy %s to %s ' % (key, member, build_path, deploy_dir))
            return False

    def __deploy_elements(self):
        for element in self.deploy_elements:
            MyLog.info('\033[94m---------------- Deploy Component [%s] Stating ----------------\033[0m'
                       % element['component'])
            # src_dir =  __deploy_get_source_dir(element['component'])
            component_target = element['target']
            if component_target is not None:
                MyLog.info('target: %s' % component_target)
                for key, values in component_target.items():
                    if key == 'version':
                        # self.version_dict[element['component']] = values
                        self.__get_component_version(element['component'], element['type'], values)
                        continue
                    if values is not None:
                        _dirs = self.__deploy_get_source_and_target_dir(element['component'], key, values,
                                                                        element['algo_root'])
                        MyLog.info('_dirs: %s ' % _dirs)
                        _dir_size = len(_dirs)
                        comp_value = self.__check_deployDocs(key)
                        if _dir_size == 2:
                            MyLog.info('------Starting deploy [%s]------' % key)
                            if comp_value == 0:
                                if values is not None:
                                    for value in values:
                                        if not self.__deploy_pre_component(key, _dirs[0], _dirs[1], value):
                                            MyLog.info(
                                                '\033[91m----ERROR: Deploy Component [%s] Fail ---- \033[0m'
                                                % element['component'])
                                            exit(1)
                            elif comp_value == 1:
                                if values is not None:
                                    target_list = values['target']
                                    if target_list is not None:
                                        for target in target_list:
                                            if not self.__deploy_pre_component(key, _dirs[0], _dirs[1], target):
                                                MyLog.info(
                                                    '033[91m----ERROR: Deploy Component [%s] Fail ---- \033[0m'
                                                    % element['component'])
                                                exit(1)
                                else:
                                    return False
                            else:
                                return False
                        else:
                            MyLog.error('---- Deploy Component [%s]-[%s] %s Fail ---- ' %
                                        (element['component'], key, _dir_size))
                            exit(1)
                MyLog.info('---------------- Deploy Component [%s] Finished ---------------- ' % element['component'])

    def __deploy_all_component(self):
        dep_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root + '/'
        if not os.path.exists('collect') or not os.path.exists('collect/code'):
            MyLog.error('\033[91m cache collect is not existed \033[0m')
            return False
              
        rm_cmd = 'rm -rf deploys aipilot && du -sh collect/* '
        ret = os.system(rm_cmd)        
        if ret != 0:
            MyLog.error('\033[91m CMD:[%s] failed in __deploy_all_component \033[0m' % rm_cmd)
            return False

        rm_cmd = 'mv collect/code aipilot && '
        rm_cmd += 'mkdir -p '+ dep_dir+'bin '+dep_dir+'lib '+ dep_dir+'topic '+dep_dir+'flag '+dep_dir+'param '+dep_dir+'model '+dep_dir+'script ;'
        #MyLog.info('to be exec [%s] ' % rm_cmd) 
        ret = os.system(rm_cmd)
        if ret != 0:
            MyLog.error('\033[91mmv collect/code aipilot failed in __deploy_all_component \033[0m')
            return False

        deploy_root_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root
        if not os.path.exists(deploy_root_dir):
            MyLog.error('---------------- Deploy ERROR: [%s] Not Find Deploy Doc ---------------- ' % deploy_root_dir)
            return False

        if self.build_target_arch != self.deploy_target_arch:
            MyLog.error(
                '---------------- Deploy ERROR: Build arch [%s] != Deploy arch [%s] Please Check Again !!!--- ' %
                (self.build_target_arch, self.deploy_target_arch))
            return False

        # deploy the run/proc shell
        self.__deploy_script()

        # deploy the topic json
        self.__deploy_topic()

        # deploy the elements
        self.__deploy_elements()

        return True

    def __deploy_pkg(self):
        if os.path.exists(env_build.G_AiplorerDeployDir + '/app.tar.gz'):
            os.system(" rm -rf " + env_build.G_AiplorerDeployDir + '/app.tar.gz')

        cd_cmd = "cd " + env_build.G_AiplorerDeployDir
        # copy the version.txt to deploy dir
        copy_cmd = "&& cp -rf app/algo/apa/version.txt ./version.txt"
        version_copy_cmd = cd_cmd + copy_cmd
        os.system(version_copy_cmd)
        tar_gz_cmd = ' && tar -zcvf app.tar.gz app/'
        del_cmd = ' && rm -rf app/'
        package_total_cmd = cd_cmd + tar_gz_cmd + del_cmd
        MyLog.info(package_total_cmd)
        exitcode = os.system(package_total_cmd)
        # process = subprocess.Popen([package_total_cmd])
        # exitcode = process.wait()
        time.sleep(5)
        # burn py copy
        if exitcode != 0:
            MyLog.error('------------------------Deploy All Elements ERROR [%s]--------------------' % exitcode)
            exit(1)

        burn_deploy_path = env_build.G_AiplorerDeployDir + '/burn.py'
        ret = os.system('cp -rf %s %s' % (self.burn_scripts_path, burn_deploy_path))

        if ret == 0:
            MyLog.info('[%s] OK:   copy %s to %s ' % ('burn', self.burn_scripts_path, burn_deploy_path))
        else:
            MyLog.error('[%s] Fail: copy %s to %s ' % ('burn', self.burn_scripts_path, burn_deploy_path))
            MyLog.error('------------------------Deploy All Elements ERROR [%s]--------------------' % exitcode)
            exit(1)

        burn_key_scripts_path = env_build.G_AiplorerDeployDir + '/burn_key.py'
        ret = os.system('cp -rf %s %s' % (self.burn_key_scripts_path, burn_key_scripts_path))

        if ret == 0:
            MyLog.info('[%s] OK:   copy %s to %s ' % ('burn', self.burn_key_scripts_path, burn_key_scripts_path))
        else:
            MyLog.error('[%s] Fail: copy %s to %s ' % ('burn', self.burn_key_scripts_path, burn_key_scripts_path))
            MyLog.error('------------------------Deploy All Elements ERROR [%s]--------------------' % exitcode)
            exit(1)

        debug_key_path = env_build.G_AiplorerDeployDir + '/debug.key'
        ret = os.system('cp -rf %s %s' % (self.debug_key_path, debug_key_path))

        if ret == 0:
            MyLog.info('[%s] OK:   copy %s to %s ' % ('debug_key', self.debug_key_path, debug_key_path))
        else:
            MyLog.error('[%s] Fail: copy %s to %s ' % ('debug_key', self.debug_key_path, debug_key_path))
            MyLog.error('------------------------Deploy All Elements ERROR [%s]--------------------' % exitcode)
            exit(1)

    # region ===========================avm========================
    # def __deploy_avm_pre_component(self, key, build_dir, deploy_dir, member):

    def __deploy_all_avm_component(self):  # 打包
        deploy_root_dir = env_build.G_AiplorerDeployDir + '/' + self.deploy_dist_deploy_root
        if not os.path.exists(deploy_root_dir):
            MyLog.error('---------------- Deploy ERROR: [%s] Not Find Deploy Doc ---------------- ' % deploy_root_dir)
            exit(1)

        if self.build_target_arch != self.deploy_target_arch:
            MyLog.error(
                '---------------- Deploy ERROR: Build arch [%s] != Deploy arch [%s] Please Check Again !!!--- ' %
                (self.build_target_arch, self.deploy_target_arch))
            exit(1)

        # deploy the elements
        self.__deploy_elements()

        return True

    def __deploy_avm_component(self):
        MyLog.info('--------------------------------- Deploy AVM Elements------------------------------------')
        self.__deploy_all_avm_component()

    # end region ===========================avm========================

    # deploy function
    def deploy(self, _dir):
        MyLog.info("deploy_ver=[%s] arch=[%s] branch=[%s] is running.." % (self.deploy_version, self.deploy_target_arch,
                                                                           self.deploy_master_branch))
        MyLog.info('\033[92m---------------------------------Starting Deploy All Elements------------------------------------\033[0m')

        if self.deploy_master_branch.__contains__(env_build.CDModule[0]):
            if self.__deploy_all_component():  # 打包
                self.__op_version_text(self.deploy_ver_path)  # 写入版本
                MyLog.info('----------------------------Starting Deploy 3rdparty and comm Elements-------------------')
                self.pkgcomm.get_pkg_com_lib(_dir)  # 打包第三方库
                self.__deploy_pkg()  # package
                MyLog.info('\033[92m-------------------------Deploy All Elements Success----------------------\033[0m')
            else:
                MyLog.info('\033[91m---------------------------------Deploy All Elements ERROR--------------------------------\033[0m')           
                exit(1)

        # if self.deploy_master_branch.__contains__(env_build.CDModule[1]):
        #     self.__deploy_avm_component()

    # endregion =================Deploy==========================

    # region ====================cppcheck========================
    def __get_cppcheck_ignore_files(self, component):
        ignore_cmd = ''
        for element in self.cppcheck_ignore_elements:
            if element["component"] == component:
                if element["type"] == "module":
                    for file in element["files"]:
                        ignore_cmd += ' -i ' + self.modules_root_dir + component + file
                elif element["type"] == "service":
                    for file in element["files"]:
                        ignore_cmd += ' -i ' + self.services_root_dir + component + file
        return ignore_cmd

    def __get_component_cmd_path(self, component):
        # for elem in env_build.ModuleElems:
        #     if component == elem:
        #         return self.modules_root_dir + component

        # for elem in env_build.ServiceElems:
        #     if component == elem:
        #         return self.services_root_dir + component
        if os.path.exists(self.modules_root_dir + component):
            return self.modules_root_dir + component
        else:
            return self.services_root_dir + component

    def __check_want_rebase(self, component, main_branch, branch):
        if main_branch is None:
            MyLog.info('\033[94m\033[1m=========== [%s] want not rebase ===========\033[0m' % component)
            return False
        if main_branch == branch:
            MyLog.info('\033[94m\033[1m=========== [%s] want not rebase  ===========\033[0m' % component)
            return False

        MyLog.info('\033[94m\033[1m====== [%s] rebase [%s] to [%s] =======\033[0m' % (component, branch, main_branch))
        return True

    def __rebase_cmd(self, component, main_branch, branch):
        component_path = self.__get_component_cmd_path(component)
        rebase_cmd = "cd " + component_path + " && "
        rebase_cmd += "git pull --rebase origin " + main_branch
        MyLog.info('\033[94m rebase_cmd: %s  \033[0m' % rebase_cmd)
        ret = os.system(rebase_cmd)
        MyLog.info('\033[94m [%s] - [%s] rebase [%s]  \033[0m' % (component, branch, ret))
        if ret != 0:
            MyLog.error('\033[91m [%s] - Rebase ERR0R \033[0m' % component)
            exit(1)

    def __cppcheck_cmd(self, component, branch):
        cd_cmd = "cd /usr/bin "
        cppcheck_cmd = ' && ./cppcheck --enable=warning --std=c++11 --language=c++ '
        cppcheck_cmd += self.__get_component_cmd_path(component)
        cppcheck_cmd += self.__get_cppcheck_ignore_files(component)
        cpp_check_total_cmd = cd_cmd + cppcheck_cmd
        MyLog.info('\033[94m\033[1m=========== STARTING CPPCHECK  [%s] Component ===========\033[0m' % component)
        MyLog.info(cpp_check_total_cmd)
        ret = os.system(cpp_check_total_cmd)
        if ret != 0:
            # exit(1)
            MyLog.error('\033[91m [%s] - CPP CHECK ERR0R \033[0m' % component)

    # 外层 main
    def __cppcheck_pre_submodules_main(self, component, submodules):
        if not self.__build_check_submodules_exist(submodules):
            MyLog.info('\n[%s] have not submodules' % component)
            return False

        for value_1 in submodules:
            self.__build_print_s(component, value_1['component'], value_1['path'], value_1['url'], value_1['branch'],
                                 None, value_1['build'], value_1['submodules'], 1)
            # clone 当前的git rep
            self.__clone_git_clone_component(value_1['component'], value_1['path'], value_1['url'],
                                             value_1['branch'])
            # 最内部的 submodules
            if self.__build_check_submodules_exist(value_1['submodules']):
                submodule = value_1['submodules']
                for value_2 in submodule:
                    self.__build_print_s(value_1['component'], value_2['component'], value_2['path'], value_2['url'],
                                         value_2['branch'], None, value_2['build'], value_2['submodules'], 2,
                                         'CPP CHECK')
                    # git clone 获取当前的 git 数据
                    self.__clone_git_clone_component(value_2['component'], value_2['path'], value_2['url'],
                                                     value_2['branch'])
                    # 进行编译
                    # if value_2['build'] is True:
                    #    self.__build_previous(self.build_main_component, value_1['component'], value_2['component'])

            # cppcheck
            # self.__cppcheck_cmd(value_1['component'])

    def __cppcheck_pre_component(self, component, path, url, main_branch, branch, submodules):
        # 判断 submodules 状态 并进入内部
        # git clone 获取当前的 git 数据
        self.__clone_git_clone_component(component, path, url, branch)
        self.__cppcheck_pre_submodules_main(component, submodules)

        if self.__check_want_rebase(component, main_branch, branch):
            self.__rebase_cmd(component, main_branch, branch)

        self.__cppcheck_cmd(component, branch)

    def __cppcheck_all_component(self):
        for module in self.build_dependencies:
            self.build_main_component = module['component']
            MyLog.info('\033[94m==== CPPCHECK  [%s] Component =====\033[0m' % module['component'])
            self.__cppcheck_pre_component(module['component'], module['path'], module['url'], module['main_branch'],
                                          module['branch'], module['submodules'])

    def cpp_check(self):
        MyLog.info('\033[92mStarting CPP Check All Component--------------\033[0m')
        self.__cppcheck_all_component()
        MyLog.info('\033[92mFinished CPP Check All Component--------------\033[0m')

    # end region ================cpp check repos=================

    # region ================test repos=================
    def test(self):
        print("ci apa version = [%s] target_arch = [%s]  build is running..." % (self.build_version,
                                                                                 self.build_target_arch))
        # self.pkgcomm.get_pkg_com_lib()
    # end region ================test repos=================
