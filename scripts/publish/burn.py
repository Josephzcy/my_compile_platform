#
# install
# python3.9
# pip3 install paramiko -i https://pypi.douban.com/simple/ --trusted-host pypi.douban.com
# python3.6
# pip3 install -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com  cryptography==36.0.2
# sudo pip3 uninstall PyNaCl
# sudo pip3 install PyNaCl

import os
import platform
import sys
import time

import paramiko

PRINT_ALL = 0

Cur_Counter = 0
End_Counter = 1
START = time.perf_counter()


def init_process(_counter):
    global End_Counter
    global START
    End_Counter = _counter
    START = time.perf_counter()


def print_process(message=''):
    global End_Counter
    global Cur_Counter
    global START
    progress = (Cur_Counter / End_Counter) * 100
    run_time = ''
    # a_int = int(int(progress) / 2)
    # finish = "=" * a_int
    # need_do = "-" * (End_Counter - a_int)
    dur = int(time.perf_counter() - START)
    if dur < 60:
        run_time = ' ' + str(dur) + ' s '
    else:
        m = int(dur / 60)
        sec = dur % 60
        run_time = '' + str(m) + 'm' + str(sec) + 's'

    # print("\r[\033[92m{:^3.0f}%\033[0m][{:.2f}s][{}{}]{}".format(progress, dur, finish, need_do, message), end="")
    if PRINT_ALL == 1:
        print("\r[\033[92m{:^3.0f}%\033[0m][{}]{}".format(progress, run_time, message))  # , end="")
    else:
        print('\x1b[2K', end='\r')
        print("\r[\033[92m{:^3.0f}%\033[0m][{}]{}".format(progress, run_time, message), end="")


class bcolors:
    HEADER = '\033[95m'  # 紫色
    OKBLUE = '\033[94m'  # 蓝色
    OKGREEN = '\033[92m'  # 翠绿
    WARNING = '\033[93m'  # 浅黄色
    FAIL = '\033[91m'  # 红色
    ENDC = '\033[0m'  # 默认值
    BOLD = '\033[1m'  # 加粗
    UNDERLINE = '\033[4m'  # 下斜线


DeployAPADir = 'app/algo/apa'
APADir = '/app/algo/apa'
LocalTmpDir = '/tmp/'
G_Burn = True


class mySSH:
    def __init__(self, host='', username='root', port=22, password='root'):
        self.ip = host
        self.port = port
        self.username = username
        self.password = password
        self.connection = None

    def connect(self):
        self.connection = paramiko.SSHClient()
        self.connection.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            if self.password != '':
                self.connection.connect(self.ip, self.port, self.username, (str(self.password)), timeout=5.0)
            else:
                try:
                    self.connection.connect(self.ip, self.port, self.username, look_for_keys=False,
                                            allow_agent=False, timeout=5.0)
                except paramiko.ssh_exception.SSHException:
                    self.connection.get_transport().auth_none(self.username)
                    self.connection.exec_command('uname -a')
            if self.connection is not None:
                self.connection.sftp = paramiko.SFTPClient.from_transport(self.connection.get_transport())
        except Exception as e:
            try:
                print(str(e.args))
                self.connection = None
            finally:
                e = None
                del e


class EnvPublish(object):
    def __init__(self):
        self.curr_module_path = os.path.dirname(os.path.realpath(__file__))
        self.os_platform = ''

    def init(self):
        self.os_platform = platform.system()  # 获取当前的系统，"Windows/Linux"

    def get_current_dir(self):
        return self.curr_module_path


def del_file_cmd(file_path, top_down=False):
    cmd = ''
    os_system = platform.system()
    if os_system == "Windows":
        cmd = 'rd /s /q ' + file_path
    else:
        cmd = 'rm -rf ' + file_path

    return cmd


def unzip_package(zipname):
    # cmd = 'unzip -o ' + zipname + ' -d ' + LocalTmpDir + ' >/dev/null 2>&1'
    if os.path.exists(LocalTmpDir + 'app'):
        os.system('rm -rf ' + LocalTmpDir + 'app')

    cmd = 'tar -zxvf ' + zipname + ' -C ' + LocalTmpDir + ' >/dev/null 2>&1'
    print(cmd)
    print("Info: starting unzip %s the package", zipname)
    if os.system(cmd) == 0:
        return True
    else:
        return False


def check_python_version():
    major_var = sys.version_info[0]
    if major_var < 2:
        print("Error: The python version is %d.%d. But python 2.x was closed. (Version 3.6 is well tested)\n "
              "Download it here: https://www.python.org/" % (major_var, sys.version_info[1]))
        return False
    else:
        print("Info: Python version = %d.%d.%d" % (sys.version_info[0], sys.version_info[1], sys.version_info[2]))
    return True


def _is_exists(path, function):
    path = path.replace('\\', '/')
    try:
        function(path)
    except Exception as error:
        return False
    else:
        return True


def _get_files_from_dir(_dir):
    get_counts_cmd = "cd  " + _dir + " && " + " ls -lR|grep \"-\"|wc -l"
    ret = os.popen(get_counts_cmd)
    return int(ret.read()) - 7


# 拷贝文件
def _copy(ssh, sftp, local, remote):
    global Cur_Counter
    # 判断remote是否是目录
    if _is_exists(remote, function=sftp.chdir):
        # 是，获取local路径中的最后一个文件名拼接到remote中
        filename = os.path.basename(os.path.normpath(local))
        remote = os.path.join(remote, filename).replace('\\', '/')

    # 如果local为目录
    if os.path.isdir(local):
        # 在远程创建相应的目录
        _is_exists(remote, function=sftp.mkdir)
        # 遍历local
        for file in os.listdir(local):
            # 取得file的全路径
            localfile = os.path.join(local, file).replace('\\', '/')
            # 深度递归_copy()
            _copy(ssh=ssh, sftp=sftp, local=localfile, remote=remote)

    # 如果local为文件
    if os.path.isfile(local):
        try:
            # ssh.exec_command("rm -rf %s" % (remote))
            # time.sleep(0.1)
            sftp.put(local, remote)

        except Exception as error:
            print(error)
            print('[put]', local, '\033[94m====>', remote, '  \033[91mFAILED\033[0m')
            exit(1)
            return False
        else:
            Cur_Counter = Cur_Counter + 1
            print_message = '\033[94m ' + remote + '\033[0m'
            print_process(print_message)
            # print('[put]', local, '\033[94m====>', remote, '\033[92mSUCCEED\033[0m')


if __name__ == '__main__':
    if not check_python_version():
        exit()
    print("Info: Please accord to under use params: \n--1: apa\n--2: other \n--3: all")
    env = EnvPublish()
    print("Info: The current dir =%s " % env.get_current_dir())

    list_of_arguments = sys.argv
    list_of_args = []
    arg_len = len(list_of_arguments)

    if arg_len != 4:
        print("Error: The cmd is error please check len= %s" % arg_len)
        print("\033[91mWarning: Please use burn.py x.x.x.x [apa] <zip_name.zip>\nexample: burn.py "
              "192.168.66.234 apa xxx.zip\033[0m")
        exit(1)

    ip_addr = (list_of_arguments[1])
    func_module = (list_of_arguments[2])
    zip_name = (list_of_arguments[3])

    # print("Error: The cmd is error please check len= %s" % arg_len)

    test = mySSH(ip_addr, 'root', 22, 'root')
    print("Info: Connecting %s ", ip_addr)
    test.connect()
    if test.connection is None:
        print("\033[93mError" ": not connect the board... please check the connect\033[0m")
        exit(1)

    print("Info: Connect the board succeed...")
    stdin, stdout, stderr = test.connection.exec_command('uname -a')
    print(stdout.read())
    # first do the remount cmd
    remount_cmd = 'remount'
    stdin_r, stdout_r, stderr_r = test.connection.exec_command(remount_cmd)
    time.sleep(3)
    if stdout_r.read() == '':
        print("Info: remount was succeed...")
    #  burn the package
    print("Info: Starting Burn the apa filesystem")
    # delete the board file
    stdin_del, stdout_del, stderr_del = test.connection.exec_command(del_file_cmd(APADir))
    # unzip the zip to tmp
    if not unzip_package(env.get_current_dir() + '/' + zip_name):
        print("\033[91mError: Unzip the package [-%s-] wrong\033[0m" % zip_name)
        exit(1)
    dist_dir = LocalTmpDir + DeployAPADir
    counter = _get_files_from_dir(dist_dir)
    print("local dir=%s counter = %s" % (dist_dir, counter))
    print("remote dir=%s" % APADir)
    init_process(counter)
    # copy the -rf zip_name/app/algo/app to /app/algo/app
    # test.connection.sftp.put(LocalTmpDir + DeployAPADir, APADir)
    _copy(test.connection, test.connection.sftp, LocalTmpDir + DeployAPADir, APADir)
    x_cmd = 'chmod +x /app/algo/apa/*.sh && ' \
            'chmod +x /app/algo/apa/script/*.* && chmod +x /app/algo/apa/bin/* '
    stdin_x, stdout_x, stderr_x = test.connection.exec_command(x_cmd)

    sync_cmd = 'sync'
    stdin_sync, stdout_sync, stderr_sync = test.connection.exec_command(sync_cmd)

    ver_cmd = 'cat /app/algo/apa/version.txt'
    stdin_v, stdout_v, stderr_v = test.connection.exec_command(ver_cmd)
    ver_back = stdout_v.read().decode('utf-8')
    print("\n[\033[92m100 %\033[0m]\033[93m----the apa version-----\033[0")
    print("[\033[94m%s\033[0m" % ver_back)
    print("\n[\033[92m100 %\033[0m]\033[92m----burn the apa firmware is succeed---- \033[0m")

    # print_process(ver_back)
