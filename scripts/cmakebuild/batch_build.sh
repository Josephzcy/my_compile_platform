module=$1
project=$2
echo $module
echo $project
./cmakebuild/single_build.sh linux release $project gflags $module
./cmakebuild/single_build.sh cv22 release $project env $module
