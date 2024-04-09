#include <iostream>
#include <string>

#include "examples/sayhello/lib/hello-time.h"
#include "examples/sayhello/main/hello-greet.h"
#include "gtctimer/gtctimer.h"

int main(int argc, char** argv) {
  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << get_greet(who) << std::endl;
  print_localtime();

  std::cout << "<~~~~~~~~~~~~~~~~~~~~>" << std::endl;
  std::cout << GetAdsTimeInUsecs() << std::endl;

  return 0;
}
