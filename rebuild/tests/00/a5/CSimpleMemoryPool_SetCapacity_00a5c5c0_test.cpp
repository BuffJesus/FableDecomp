#include "rebuild_abi.h"
#include <cstdio>
struct CSimpleMemoryPool { char pad00[8]; unsigned int capacity; void SetCapacity(unsigned int n); };
void CSimpleMemoryPool::SetCapacity(unsigned int n) { capacity = ((n + 0xf) / 16) * 16; }
int main(){
    CSimpleMemoryPool p; p.capacity=0;
    p.SetCapacity(1); if(p.capacity!=16){std::printf("BAD1\n");return 1;}
    p.SetCapacity(16); if(p.capacity!=16){std::printf("BAD2\n");return 1;}
    p.SetCapacity(17); if(p.capacity!=32){std::printf("BAD3\n");return 1;}
    std::printf("SETCAP_OK\n"); return 0;
}