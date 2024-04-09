#!/bin/bash
#set -x
#set -o pipefail

RED_COLOR='\E[1;31m'   #红
GREEN_COLOR='\E[1;32m' #绿
YELOW_COLOR='\E[1;33m' #黄
BLUE_COLOR='\E[1;34m'  #蓝
RES='\E[0m'

script_sh=${0##*/}
script_dir=$(cd $(dirname $0) && pwd)
export PATH=${PATH}:${script_dir}

json_dir=$(cd $(dirname $0)/../ci_config && pwd)
pack_json=$json_dir/package.json

aiplore_dir=$(cd $(dirname $0)/../.. && pwd)
cd $aiplore_dir

# git_clone_recursive：read json and git clone recursively
#
# parameter：
# $1: node number
# $2：node
#
# return value：
# void
git_clone_recursive() {
  local i
  for (( i = 0; i < $1; i++)); do
    #echo -e "~~~~ $i ~~~~~"
    node=$(echo $2 |jq  ".[$i]")
    component=$(echo "$node" | jq -r '.component')
    path=$(echo "$node" | jq -r '.path')
    url=$(echo "$node" | jq -r '.url')
    branch=$(echo "$node" | jq -r '.branch')
    submodules=$(echo "$node" | jq -r '.submodules')

    if [ "$url" == "null" ]; then
      echo -e "${YELOW_COLOR}[$component] came with aiplorer, no need to clone $RES"    
      continue
    fi

    if [ -d "$path" ]; then
      rm -rf $path
    fi
    mkdir -p $path    

    set +e
    git clone -b $branch $url $path 2>/dev/null
    if [ $? -ne 0 ]; then
      echo -e "${RED_COLOR}check permission to clone [$component] $RES"
    else
      echo -e "${GREEN_COLOR}succeed to clone [$component] $RES"  
    fi
    set -e
    
    if [ "$submodules" != "null" ]; then
      sub_count=$(echo $submodules | jq length)
      #echo "----------------- $sub_count"
      #echo "----------------- $submodules"
      git_clone_recursive $sub_count "$submodules"
      #echo "@@@@@@@@@@@@@@@@@ $submodules"
    fi
  done
}

cur_array=$(jq .dependencies $pack_json)
comp_count=$(echo $cur_array | jq length)
#echo -e "@@@@@ $comp_count"
#echo -e "@@@@@ $cur_array"
git_clone_recursive $comp_count "$cur_array"

