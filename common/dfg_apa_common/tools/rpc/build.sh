source '/home/zzq/data3/amba/sw-sdk/setupenv.sh'
aarch64-linux-gnu-gcc rpc.c -lambaipc -lgflags -shared -lglog -fPIC -o librpc.so
mv librpc.so ../../lib/librpc.so
