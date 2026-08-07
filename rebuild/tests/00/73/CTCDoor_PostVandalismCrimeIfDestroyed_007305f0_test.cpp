#include "rebuild_abi.h"
#include <cstdio>
struct CVandalTarget { char pad00[0x38]; unsigned char m_flag; };
struct CTCDoor { char pad00[0x20]; CVandalTarget *m_target; bool PostVandalismCrimeIfDestroyed(); };
bool CTCDoor::PostVandalismCrimeIfDestroyed(){ CVandalTarget *t=m_target; if(t && t->m_flag) return true; return false; }
int main(){
    CTCDoor d; d.m_target=0;
    if(d.PostVandalismCrimeIfDestroyed()){std::printf("B1\n");return 1;}
    CVandalTarget t; t.m_flag=0; d.m_target=&t;
    if(d.PostVandalismCrimeIfDestroyed()){std::printf("B2\n");return 1;}
    t.m_flag=1;
    if(!d.PostVandalismCrimeIfDestroyed()){std::printf("B3\n");return 1;}
    std::printf("VANDAL_OK\n"); return 0;
}