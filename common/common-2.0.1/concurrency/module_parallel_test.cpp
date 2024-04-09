#include "common/concurrency/module_parallel.h"

int main(int argc, char *argv[])
{
    REGISTER_PARALLEL_MODULE(lane);
    REGISTER_PARALLEL_MODULE(vehicle);

    ModuleParallel<Lane>::Instance().Initialize(2);
    ModuleParallel<Vehicle>::Instance().Initialize(4);
    return 0;
}
