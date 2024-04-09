FROM ubuntu:18.04

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list \
    && apt clean && apt update; 
RUN apt install -y build-essential libncurses5-dev autoconf automake dos2unix \
    python python3 python3-pip lrzsz lzop flex libssl-dev u-boot-tools git bison bc kmod \
    cmake cpio libarchive13 makeself cppcheck
RUN pip3 install gitpython
RUN cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && echo 'Asia/Shanghai'>/etc/timezone
