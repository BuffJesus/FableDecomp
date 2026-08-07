#include "rebuild_abi.h"
#include <cstdio>
struct CObservable { char pad00[0x38]; void *m_observer; void AddExclusiveObserver(void *obs); };
void CObservable::AddExclusiveObserver(void *obs){ m_observer = obs; }
int main(){
    CObservable o; o.m_observer=0; o.AddExclusiveObserver((void*)0x9);
    if(o.m_observer!=(void*)0x9){std::printf("BAD\n");return 1;}
    std::printf("ADDOBS_OK\n"); return 0;
}