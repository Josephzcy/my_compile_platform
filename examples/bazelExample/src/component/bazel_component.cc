
#include "bazel_component.h"

using namespace std;

namespace minieye{
namespace bazel{

BazelComponent::BazelComponent(){
}

bool BazelComponent::init(){
    std::cout << "Bazel init, starting ..." << std::endl;
    return true;
}


}
}
