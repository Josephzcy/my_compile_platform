#
import env_build
from apa_ci_logger import MyLog
from command_parse import CmdParse
import sys

if __name__ == '__main__':
    if not env_build.check_python_version():
        exit()
    MyLog.info("Please accord to under use params: \n----1: build\n----2: deploy [deploy_path] \n"
               "----3: build deploy [deploy_path]")
    env = env_build.EnvBuild()
    env.init()  # init the env
    cmd_parse = CmdParse()  # init cmd parse
    # get input params
    # input_cmd = input('Please input create sln or project cmd:')
    # cmd_parse.parse_input(input_cmd)
    list_of_arguments = sys.argv
    list_of_args = []
    arg_len = len(list_of_arguments)

    if len(list_of_arguments) > 1:
        for i in range(1, arg_len):
            list_of_args.append(list_of_arguments[i])

        if cmd_parse.parse_input(list_of_args):
            if list_of_args[0] == "cppcheck":
                MyLog.info('APA CL %s Finished ......' % list_of_args[0])
            else:
                MyLog.info('APA CL %s SUCCEED !!!...' % list_of_args[0])
        else:
            MyLog.error('APA CL %s FAIL !!!...' % list_of_args[0])
    else:
        MyLog.error('Please and cmd params !!!...')
        MyLog.error('APA CL %s Fail !!!...' % list_of_args[0])
