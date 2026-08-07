#include "rebuild_abi.h"
#include <cstdio>
struct ConsThis { char pad[4]; char *m_p; void *Cons(void *arg); };
void *ConsThis::Cons(void *arg){ char *r=m_p+0x3c; if(arg) r=(char*)arg+0xc; return r; }
int main(){ ConsThis o; char buf[128]; o.m_p=buf; char other[64];
 if(o.Cons(0)!=buf+0x3c){std::printf("B1\n");return 1;}
 if(o.Cons(other)!=other+0xc){std::printf("B2\n");return 1;}
 std::printf("CV_c19680_OK\n"); return 0; }