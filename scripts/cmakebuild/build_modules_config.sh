#!/bin/bash

Object=$1

total_modules=(
"dfg_apa_common"
"dfg_apa_log_reader"
"dfg_apa_transparent"
"dfg_apa_cnn"
"dfg_obstacles_detect"
"dfg_obstacles_fusion"
"dfg_parkingspace_cornerpts"
"dfg_parkingspace_track"
"dfg_ultrasonic_parkingspot"
"path_planning"
"vehicle_control"
"apa_odometer"
"dfg_apa"
)

omit_modules=(
)

# object
case "$Object" in
    cicd|common|third_party)
        branch=$Object
        omit_modules=(
        )
        ;;
    master)
        branch=master
        omit_modules=(
        )
        ;;
    linux-pc)
        branch=feature-pc
        omit_modules=(
        )
        ;;
    tang20)
        branch=feature-tang
        omit_modules=(
        )
        ;;
    skoda)
        branch=feature-skoda
        omit_modules=(
        )
        ;;
    dev)
        branch=develop
        omit_modules=(
        )
        ;;
    dds)
        branch=feature-dds
        omit_modules=(
        )
        ;;
    *)
        echo "unhandled $Object"
        ;;
esac

module_names=${total_modules[@]}
#echo ${module_names[@]}
for del in ${omit_modules[@]}
do
    module_names=("${module_names[@]/$del}")
done