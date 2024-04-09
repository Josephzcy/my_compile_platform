//
// Created by minieye on 2022/6/8.
//
#include <thread>

#include "aipilot/services/dfg_avm_calib/avm_calib_service/avm_calib_service.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "aipilot/services/dfg_avm_calib/include/version.h"

using namespace minieye::avm;

DEFINE_double(err_threshold, 0.02, "reproject error");

DEFINE_string(config_file, "avm_calib.json", "dds config file");
DEFINE_string(topic_camera, "camera30", "calibration image topic name");
DEFINE_string(topic_avm_status, "avm_calib_status", "avm_calib_status topic name");
DEFINE_string(topic_avm_ctrl, "avm_calib_ctrl", "avm_calib_ctrl topic name");
DEFINE_string(car_file, "/calib/avm_calib/car_info.json", "car file path");
DEFINE_string(layout_file, "/calib/avm_calib/avm_layout.json", "car file path");
DEFINE_string(intrins_file_path, "/calib/avm_calib/", "intrins file path");
DEFINE_string(save_exp_path, "/calib/avm_calib/", "export extrins file");
DEFINE_int32(loglevel, 1, "set log level");
DEFINE_bool(logtostderr, true, "");
DEFINE_bool(colorlogtostderr, true, "");

void signalHandler(int signum) {
    LOG(ERROR) << "Interrupt signal (" << signum << ") received.\n";
    exit(signum);
}

void print_version() {
    std::string version =
            std::string("AVM-CALIB-SERVER_T1Q_V") + PROJECT_VER + "_" + BUILD_TIME_2;
    fprintf(stderr, "VERSION INFO:\n");
    fprintf(stderr, "\t PROJECT NAME: %s\n", PROJECT_NAME);
    fprintf(stderr, "\t PROJECT VERSION: %s\n", version.c_str());
}

int main(int argc, char *argv[]) {
//    google::InitGoogleLogging(argv[0]);
    print_version();
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::SetStderrLogging(FLAGS_loglevel);
    signal(SIGINT, signalHandler);
    AvmCalibService service;

    LOG(INFO) << "init avm_calib_service";
    std::vector<std::string> direction_str = {
            "front", "rear", "left", "right"
    };
    std::map<int32_t, std::string> intrins_files;
    for (uint32_t inum = 0; inum < direction_str.size(); ++inum) {
        intrins_files.insert(std::pair<int32_t, std::string>(
                1u << inum, FLAGS_intrins_file_path
                            + "/" + direction_str[inum] + ".json"));
    }

    std::map<std::string, std::string> topic_names = {
            std::pair<std::string, std::string>("camera", FLAGS_topic_camera),
            std::pair<std::string, std::string>("output_status", FLAGS_topic_avm_status),
            std::pair<std::string, std::string>("ctrl_status", FLAGS_topic_avm_ctrl),
    };
    service.init(FLAGS_config_file, topic_names, intrins_files,
                 FLAGS_car_file, FLAGS_layout_file, FLAGS_save_exp_path,
                 FLAGS_err_threshold);
    LOG(INFO) << "run avm_calib_service";
    std::thread calib_service_thread(AvmCalibService::run, &service);
    calib_service_thread.join();
    service.uninit();
    return 0;
}