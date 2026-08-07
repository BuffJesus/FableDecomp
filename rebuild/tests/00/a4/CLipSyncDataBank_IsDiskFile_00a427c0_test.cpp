#include "rebuild_abi.h"
#include <cstdio>
struct CLipSyncDataBank { bool IsDiskFile(unsigned long a, unsigned long b); };
bool CLipSyncDataBank::IsDiskFile(unsigned long a, unsigned long b){ return false; }
int main(){
    CLipSyncDataBank d;
    if(d.IsDiskFile(1,2)){std::printf("BAD\n");return 1;}
    std::printf("ISDISK_OK\n"); return 0;
}