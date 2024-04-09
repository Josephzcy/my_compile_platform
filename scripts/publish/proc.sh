ENV=`uname -m`

declare -A Process_Dict
Process_Dict=(\
[apa_manager]=apa_manager \
[localization]=apa_localization \
[bev]=bird_eye_view \
[inference]=havp_inference \
[postprocess]=havp_parkingspace_postprocess \
[planning]=apa_planning_dds \
[vehicle]=vehicle_control_dds
)
declare -A Script_Dict
Script_Dict=(\
[postprocess]=run_postprocess.sh \
[inference]=run_havp_inference.sh \
[localization]=run_apa_localization.sh \
[bev]=run_bev.sh \
[planning]=run_apa_planning_dds_x50.sh \
[vehicle]=run_vehicle_control_dds.sh \
[apa_manager]=run_apa_manager_dds.sh
)

Program=${0##*/}

show_usage(){
echo "Usage:" 
echo "$Program [Options] <Target>"
echo
echo "Options:"
echo " [-h]:  show help msg and exit"
echo " [-start]:    start service or  module node"
echo " [-stop]:     stop/kill  service or  module node"
echo " [-restart]:  restart service or module node"
echo
echo "Target:"
echo "< all >                  -- all modules and services "
echo "< postprocess >          -- havp_parkingspace_postprocess module"
echo "< planning >             -- apa_planning module"
echo "< inference >            -- havp_inference module"
echo "< vehicle_ctrl >         -- vehicle_control_dds module"
echo "< localization >         -- apa_localization module"
echo "< apa_manager >          -- apa_manager module"
echo "< avm_calib >            -- dfg_avm_calib service"
echo "< bev >                  -- bird_eye_view service"
echo "< avm_map >              -- dfg_avm_mapping service"
echo "< avm_park >             -- dfg_avm parking service"
}

#for sName in ${!Process_Dict[@]}
#do
#  echo  "${sName}" :  currScore = ${Process_Dict[${sName}]}
#done

# parse the input parmas
Argv="$@"
array=($(echo $Argv | tr ' ' ' '))

declare -a input_target
function _input_target
{
  for (( i = 1;  i< ${#array[@]}; i++));
  do
     input_target[$i-1]=${array[$i]}
  done
}

# kill the process according to the pid
function _kill_proc_by_id
{
  kill -9 $1
}
# run the special sh according to input param
function _run_special_sh
{
  ./script/$1 >/dev/null 2>&1 &
}

# operator the all modules  include start and stop cmd
function _op_all
{
  op=$1
  for sName in ${!Process_Dict[@]}
    do
      echo "check [ -${Process_Dict[$sName]}- ] proc "
      local_pid=`ps aux|grep ${Process_Dict[$sName]} |grep -v "grep"|awk '{print $2}'`
      if [ "$local_pid" == "" ] ; then    # process is not run and  is empty
          if [ $op == "start" ]; then
            _run_special_sh ${Script_Dict[$sName]}
            echo "[-${Script_Dict[$sName]}-] is running succeed "
          fi
          if [ $op == "stop" ]; then
            echo "[-${Script_Dict[$sName]}-] had not run "
          fi
      else      # process is running and not deal
        if [ $op == "start" ]; then
          echo "[-${Process_Dict[$sName]}-] had been starting the pid "$local_pid" "
        fi
        if [ $op == "stop" ]; then
          _kill_proc_by_id $local_pid
          echo "[-${Process_Dict[$sName]}-] had been killed"
        fi
      fi
    done
}

# start all or special and want run process
function _start
{
  len=${#input_target[@]}
# start all
  if [ $len == 0 ]; then
    echo "start run all proc"
    _op_all "start"
    return
  fi

  if [ $len == 1 ]; then
      if [ ${input_target[0]} == "all" ]; then
        echo "start run all proc"
        _op_all "start"
      fi
  fi

# start input params
  for var in ${input_target[@]}
    do
      for sName in ${!Process_Dict[@]}
      do
        if [ $var == $sName ]; then
          echo "check [ -${Process_Dict[$var]}- ] proc "
          local_pid=`ps aux|grep ${Process_Dict[$var]} |grep -v "grep"|awk '{print $2}'`
          if [ "$local_pid" == "" ] ; then    # process is not run and  is empty
            echo "start  run [ -${Script_Dict[$var]}-] "
            _run_special_sh ${Script_Dict[$var]}
          else
            echo "[${Process_Dict[$var]}] had been starting the pid "$local_pid" "  # process is running and not deal
          fi
          continue
        fi
      done
      # echo "Warring: Have not the this proc [ -$var- ]"
    done

}
# stop services or modules
function _stop
{
  len=${#input_target[@]}
  if [ $len == 0 ]; then
    echo "stop run all proc"
    _op_all "stop"
    return
  fi

  if [ $len == 1 ]; then
      if [ ${input_target[0]} == "all" ]; then
          echo "stop run all proc"
          _op_all "stop"
      fi
  fi

  for var in ${input_target[@]}
    do
      for sName in ${!Process_Dict[@]}
      do
        if [ $var == $sName ]; then
          echo "Check [ -${Process_Dict[$var]}- ] proc "
          local_pid=`ps aux|grep ${Process_Dict[$var]} |grep -v "grep"|awk '{print $2}'`
          if [ "$local_pid" == "" ] ; then
            echo "[ -${Process_Dict[$var]}-] has not run"
          else
            echo " Will kill [ -${Process_Dict[$var]}-] "
            _kill_proc_by_id $local_pid
            echo "[${Process_Dict[$var]}] had been killed"
          fi
          continue
        fi
      done
      # echo "Warring: Have not the this proc [ -$var- ]"
    done

}

function _restart
{
  _op_all "stop"
  echo  "stop all services or modules"
  sleep 2
  echo  "starting all services or modules"
  _op_all "start"
}

# do cmd
function funWithCmd
{
  if [ $1 == '-h' ]; then
   show_usage
   exit 1
  fi
  _input_target
  if [ $1 == '-start' ]; then
     _start  "$input_target"
  elif [ $1 == '-stop' ]; then
      _stop "$input_target"
  elif [ $1 == '-restart' ]; then
      _restart "$input_target"
  else
     echo "Error: not support cmd and please check..."
     show_usage
  fi
}

# input params is zero print usage and exit
if [ $# -eq 0 ];then
    show_usage
    exit 1
fi

funWithCmd $Argv

killall `echo "$0"|sed -e s'/.\/\(.*\)/\1/'`

