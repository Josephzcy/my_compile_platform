#include <fstream>
#include <iomanip>
#include <iostream>

#include "json.hpp"
using json = nlohmann::json;

bool readJson(json &jsonData, const std::string &fname) {
  std::ifstream ifs(fname, std::fstream::in);
  try {
    ifs >> jsonData;
  } catch (std::exception &e) {
    std::cout << "[" << __FUNCTION__ << "]" << e.what() << "\n";
    ifs.close();
    return false;
  }
  ifs.close();
  return true;
}

int jsonHandle(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s [source.json] [target.json]\n", argv[0]);
    return -1;
  } else {
    printf("Input: source:%s, target:%s\n", argv[1], argv[2]);
  }
  json j_origin;
  if (!readJson(j_origin, argv[1])) {
    printf("Read json file error!\n");
  }
  json j_target;
  if (!readJson(j_target, argv[2])) {
    printf("Read json file error!\n");
  }
  json j_patch = json::diff(j_origin, j_target);
  json j_result = j_origin.patch(j_patch);
  std::ofstream ofs(argv[1], std::fstream::out);
  ofs << std::setw(4) << j_result << std::endl;
  ofs.close();
  return 0;
}

int main(int argc, char *argv[]) {
#ifdef WITH_LINUX_PC
  std::cout << "WITH LINUX~~~" << std::endl;
#endif
  if (jsonHandle(argc, argv) < 0) {
    return -1;
  }
  std::cout << "Hello World!" << std::endl;

  return 0;
}
