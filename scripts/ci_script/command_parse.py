import os.path
from collections import defaultdict
# V1.0 Command Operate List
import env_build
from apa_ci import ApaCi
from apa_ci_logger import MyLog


CmdOperaList = \
    [
        'build',
        'deploy',
        'test',
        'cppcheck'

    ]
# V1.0 Depend Params List
DependsParamList = \
    [
        'path',
        'url',
        'branch',
        'submodules'
    ]


class CmdParse(object):
    def __init__(self):
        self.cmd_list_dict = []
        self.ci_cmd_prj_dict = defaultdict(list)
        self.apa_build = ApaCi()

    def __init_all_cmds(self):
        for _cmd in CmdOperaList:
            self.ci_cmd_prj_dict[CmdOperaList[0]].append(_cmd)
        MyLog.info(' support [%s]' % self.ci_cmd_prj_dict)
        return True

    def __output_cmd(self, tm_cmd, result):
        cmd_str = ''
        for i in range(len(tm_cmd)):
            cmd_str += tm_cmd[i] + ' '
        self.out_string = 'CI Command  [' + cmd_str + '] Result -> ' + result
        MyLog.info(self.out_string)

    def parse_input(self, in_cmd):
        # cmd_params = re.split(r"[ ]+", in_cmd)
        cmd_params = in_cmd
        _len = len(cmd_params)
        if _len > 3:
            self.__output_cmd(cmd_params, 'ERROR: CMD too long please check')
            return False
        else:
            if _len == 1:
                if cmd_params[0] == CmdOperaList[0] or cmd_params[0] == CmdOperaList[2] or \
                        cmd_params[0] == CmdOperaList[3]:
                    self.__output_cmd(cmd_params, 'INFO: Starting [-%s-]' % cmd_params[0])
                    self.dispatch_cmd_dict(cmd_params[0])  # 获取当前的 build
                    return True
                return False
            elif _len == 2:
                if cmd_params[0] == CmdOperaList[1]:
                    env_build.G_AiplorerDeployDir = cmd_params[1]
                    if not os.path.exists(env_build.G_AiplorerDeployDir):
                        os.makedirs(env_build.G_AiplorerDeployDir)
                    MyLog.info('----ci_script deploy root dir: %s' % env_build.G_AiplorerDeployDir)
                    #else:
                    #    MyLog.error('ci_script deploy root dir: %s not exist' % env_build.G_AiplorerDeployDir)
                    #    return False
                    self.__output_cmd(cmd_params, 'INFO: Starting...')
                    self.dispatch_cmd_dict(cmd_params[0], env_build.G_AiplorerDeployDir)  # 获取当前的 deploy
                    return True
            elif _len == 3:
                if cmd_params[0] != CmdOperaList[0]:
                    self.__output_cmd(cmd_params, 'ERROR: Please Check Params')
                    return False
                elif cmd_params[1] != CmdOperaList[1]:
                    self.__output_cmd(cmd_params, 'ERROR: Please Check Params')
                    return False
                else:
                    env_build.G_AiplorerDeployDir = cmd_params[2]
                    if os.path.exists(env_build.G_AiplorerDeployDir):
                        MyLog.info('----ci_script deploy root dir: %s' % env_build.G_AiplorerDeployDir)
                    else:
                        MyLog.error('----ci_script deploy root dir: %s not exist' % env_build.G_AiplorerDeployDir)
                        return False
                    self.dispatch_cmd_dict(cmd_params[0])  # 获取当前的 build
                    self.dispatch_cmd_dict(cmd_params[1], env_build.G_AiplorerDeployDir)  # 获取当前的 build
                    return True
        return False

    def cmd_error_log(self, _op):
        MyLog.error('Not support the [%s] command' % _op)

    def dispatch_cmd_dict(self, _op, _dir=None):
        return {
            CmdOperaList[0]: lambda: self.apa_build.build(),  # build
            CmdOperaList[1]: lambda: self.apa_build.deploy(_dir),  # deploy
            CmdOperaList[2]: lambda: self.apa_build.test(),  # test
            CmdOperaList[3]: lambda: self.apa_build.cpp_check(),  # cppcheck
        }.get(_op, lambda: self.cmd_error_log(_op))()
        # }.get(_op, lambda: None)()
