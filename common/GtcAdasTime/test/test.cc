#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <thread>

#include "GtcAdasTime.h"

using namespace std;


int main(){

    while(1)
    {
        printf("GetAdsTimeInUsecs: %lu, utc_ime: %lu, tick_to_utc_ime: %lu \n", GetAdsTimeInUsecs(), get_utc_time_in_us(), tick_to_utc_time_in_us(GetAdsTimeInUsecs()));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}

