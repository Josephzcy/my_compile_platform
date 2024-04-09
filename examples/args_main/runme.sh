export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../../third_party/linux94/glog_0.3.5-rc.5/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../../third_party/linux94/gflags_2.2.2/lib

rm -rf output/*.a
# Print help
echo "----->  Print help: ./output/args_main -h  <-----"
./output/args_main -h
echo
echo
echo

# Print version
echo "----->Print version: ./output/args_main -V  <-----"
./output/args_main -V
echo
echo
echo

# enable glog: info, warning and error
echo "----->  enable glog[info, warning and error]: /output/args_main -v 3  <-----"
./output/args_main -v 3
echo
echo
echo

# enable glog: warning and error
echo "----->  enable glog[warning and error]: ./output/args_main -v 2  <-----"
./output/args_main -v 2
echo
echo
echo

# enable glog: error
echo "----->  enable glog[error]: ./output/args_main -v 1  <-----"
./output/args_main -v 1
echo
echo
echo

# enable glog: error
echo "----->  enable glog[none - FATAL is not allowed]: ./output/args_main -v 0  <-----"
./output/args_main -v 0
echo
echo
echo
