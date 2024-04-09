#!/bin/bash

pushd .
export LD_LIBRARY_PATH=`pwd`/protobuf/lib/
echo $LD_LIBRARY_PATH
dst_dir=../precompiler
if [ ! -d $dst_dir ]; then
    mkdir $dst_dir
else
    echo "clean files under precompiler"
    rm -rf ${dst_dir}/*.pb.*
fi

echo "========================"
./protobuf/bin/protoc --version

echo "======== begin ========="
#for var in ${total_proto[@]};
for var in `ls *.proto`;
do
    echo $var
    ./protobuf/bin/protoc --cpp_out ${dst_dir} $var
done
cd ../precompiler
find . -name "*" -type f|xargs sed -i 's/Auxillary/Auxiliary/g'

popd
echo "========== done ========="
