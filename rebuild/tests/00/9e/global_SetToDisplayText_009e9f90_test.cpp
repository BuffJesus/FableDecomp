#include "rebuild_abi.h"
#include <cstdio>
static void* seen=0;
struct M { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void Disp(void* a){ seen=a; } };
static M gm; M* g_mgr_9e9f90=&gm;
struct C { void Set(); };
void C::Set(){ if(g_mgr_9e9f90) g_mgr_9e9f90->Disp(this); }
int main(){ C o; o.Set(); if(seen!=&o){std::printf("B1");return 1;} g_mgr_9e9f90=0; seen=0; o.Set(); if(seen!=0){std::printf("B2");return 1;} std::printf("RB_9e9f90_OK\n"); return 0; }