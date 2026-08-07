#include "rebuild_abi.h"
#include <cstdio>
struct CT { char pad[0x91]; unsigned char m_flags; };
struct Wrap { CT *m_p; };
extern "C" char FABLE_FASTCALL FableIsAlive_654af5(Wrap *w){
    CT *p=w->m_p; if(p && !(p->m_flags&1)) return 1; return 0; }
int main(){ CT b; b.m_flags=0; Wrap w; w.m_p=&b;
 if(FableIsAlive_654af5(&w)!=1){std::printf("B1\n");return 1;}
 b.m_flags=1; if(FableIsAlive_654af5(&w)!=0){std::printf("B2\n");return 1;}
 w.m_p=0; if(FableIsAlive_654af5(&w)!=0){std::printf("B3\n");return 1;}
 std::printf("IA_654af5_OK\n"); return 0; }