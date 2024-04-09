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
DeployAVMDir = 'app/algo/avm'
DeployHMIDir = 'app/algo/hmiproxy'
DeployAVMMNGDir = 'app/base/avmman'
APADir = '/app/algo/apa'
LocalTmpDir = '/tmp/'
G_Burn = True


class mySSH:
    def __init__(self, host='', username='root', port=22, keypath=''):
        self.keypath = keypath
        self.ip = host
        self.port = port
        self.username = username
        self.private_key = ''
        self.connection = None

    def connect(self):
        self.private_key = paramiko.RSAKey.from_private_key_file(self.keypath + "/debug.key")
        self.connection = paramiko.SSHClient()
        self.connection.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            if self.private_key.size > 0:
                self.connection.connect(self.ip, self.port, self.username, pkey=self.private_key, timeout=5.0)
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


def untar_package(tarname):
    # cmd = 'unzip -o ' + tarname + ' -d ' + LocalTmpDir + ' >/dev/null 2>&1'
    if os.path.exists(LocalTmpDir + 'app'):
        os.system('rm -rf ' + LocalTmpDir + 'app')

    cmd = 'tar zxf ' + tarname + ' -C ' + LocalTmpDir
    print("Info: starting untar %s the package" % tarname)
    print(cmd)    
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
        exit(1)
    #print("Info: Please accord to under use params: \n--1: apa\n--2: other \n--3: all")
    env = EnvPublish()
    print("Info: The current dir =%s " % env.get_current_dir())

    list_of_arguments = sys.argv
    list_of_args = []
    arg_len = len(list_of_arguments)

    if arg_len != 4:
        print("\033[91mError: Parameter error, len= %s\033[0m" % arg_len)
        print("Usage: \033[92mpython3 burn_key.py 192.168.98.233 apa app.tar.gz\033[0m\n")
        exit(1)

    ip_addr = (list_of_arguments[1])
    func_module = (list_of_arguments[2])
    tar_name = (list_of_arguments[3])

    # print("Error: The cmd is error please check len= %s" % arg_len)

    test = mySSH(ip_addr, 'root', 22, env.get_current_dir())
    print("Info: Connecting %s " % ip_addr)
    test.connect()
    if test.connection is None:
        print("\033[91mError" ": not connect the board... please check the connect\033[0m")
        exit(1)

    print("\033[92mInfo: succeed to connect the board ...\033[0m")
    #stdin, stdout, stderr = test.connection.exec_command('uname -a')
    #print(stdout.read())
    # first do the remount cmd
    remount_cmd = 'mount -o rw,remount /app'
    stdin_r, stdout_r, stderr_r = test.connection.exec_command(remount_cmd)
    #print('remount出错:', stderr_r.read().decode('utf-8'))
    time.sleep(1)
    if stderr_r.channel.recv_exit_status() != 0:
        print("\033[91mError: remount fail ...\033[0m")
        #test.connection.close()
    #   exit(1)
    #  burn the package
    print("\033[92m--------------- Info: Starting Burn the app filesystem ----------------\033[0m")
    # delete the board file
    stdin_del, stdout_del, stderr_del = test.connection.exec_command(del_file_cmd(APADir))
    if stderr_del.channel.recv_exit_status() != 0:
        print("\033[91mError: remove %s failed ...\033[0m" % APADir)
        exit(1)

    stdin_del2, stdout_del2, stderr_del2 = test.connection.exec_command(del_file_cmd('/' + DeployAVMDir))
    if stderr_del2.channel.recv_exit_status() != 0:
        print("\033[91mError: remove %s failed ...\033[0m" % '/' + DeployAVMDir)
        exit(1)

    stdin_del3, stdout_del3, stderr_del3 = test.connection.exec_command(del_file_cmd('/' + DeployAVMMNGDir))
    if stderr_del3.channel.recv_exit_status() != 0:
        print("\033[91mError: remove %s failed ...\033[0m" % '/' + DeployAVMMNGDir)
        exit(1)

    stdin_del4, stdout_del4, stderr_del4 = test.connection.exec_command(del_file_cmd('/' + DeployHMIDir))
    if stderr_del4.channel.recv_exit_status() != 0:
        print("\033[91mError: remove %s failed ...\033[0m" % '/' + DeployHMIDir)
        exit(1)
        
    # untar the tgz to tmp
    if not untar_package(env.get_current_dir() + '/' + tar_name):
        print("\033[91mError: untar the package [%s] wrong\033[0m" % tar_name)
        exit(1)
    dist_apa_dir = LocalTmpDir + DeployAPADir
    dist_avm_dir = LocalTmpDir + DeployAVMDir
    dist_hmi_dir = LocalTmpDir + DeployHMIDir
    dist_avmmng_dir = LocalTmpDir + DeployAVMMNGDir
    counter = _get_files_from_dir(dist_apa_dir)
    counter += _get_files_from_dir(dist_avm_dir)
    counter += _get_files_from_dir(dist_hmi_dir)    
    counter += _get_files_from_dir(dist_avmmng_dir)
    print("local dir:%s,%s,%s,%s counter = %s" % (dist_apa_dir, dist_avm_dir, dist_hmi_dir, dist_avmmng_dir, counter))
    print("remote dir:%s,%s,%s,%s" % (APADir,'/'+DeployAVMDir,'/'+DeployHMIDir, '/'+DeployAVMMNGDir))
    init_process(counter)
    # test.connection.sftp.put(LocalTmpDir + DeployAPADir, APADir)
    _copy(test.connection, test.connection.sftp, dist_apa_dir, APADir)
    _copy(test.connection, test.connection.sftp, dist_avm_dir, '/'+ DeployAVMDir)
    _copy(test.connection, test.connection.sftp, dist_hmi_dir, '/'+ DeployHMIDir)    
    _copy(test.connection, test.connection.sftp, dist_avmmng_dir, '/'+ DeployAVMMNGDir)    
    sh_x_cmd = 'find /app/base/avmman /app/algo/apa  /app/algo/avm /app/algo/hmiproxy -name "*.sh" -type f |xargs -i chmod +x {} '
    stdin_x, stdout_x, stderr_x = test.connection.exec_command(sh_x_cmd)
    if stderr_x.channel.recv_exit_status() != 0:
        print("\033[91mError: [%s] exec failed ...\033[0m" % sh_x_cmd)
                
    bin_x_cmd = 'chmod +x /app/algo/avm/bin/* && chmod +x /app/algo/hmiproxy/bin/* && chmod +x /app/algo/apa/bin/* && chmod +x /app/base/avmman/bin/* '
    stdin_bx, stdout_bx, stderr_bx = test.connection.exec_command(bin_x_cmd)
    if stderr_bx.channel.recv_exit_status() != 0:
        print("\033[91mError: [%s] exec failed ...\033[0m" % bin_x_cmd)

    sync_cmd = 'sync'
    stdin_sync, stdout_sync, stderr_sync = test.connection.exec_command(sync_cmd)

    ver_cmd = 'cat /app/algo/apa/version.txt'
    stdin_v, stdout_v, stderr_v = test.connection.exec_command(ver_cmd)
    ver_back = stdout_v.read().decode('utf-8')
    print("\n[\033[92m100 %\033[0m]\033[92m ----the apa version-----\033[0")
    print("[\033[95m%s\033[0m" % ver_back)
    print("[\033[92m100 %\033[0m]\033[92m ----burn the app firmware is succeed---- \033[0m")
    test.connection.close()
    # print_process(ver_back)