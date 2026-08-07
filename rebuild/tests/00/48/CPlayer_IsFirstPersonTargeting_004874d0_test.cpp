#include "rebuild_abi.h"
#include <cstdio>
static int mode=0; extern "C" int FABLE_FASTCALL Mode_4874d0(){ return mode; }
extern "C" char FABLE_FASTCALL Is_4874d0(){ int m=Mode_4874d0(); if(m==0x16) return 1; return (char)(m==0x19); }
int main(){ mode=0x16; if(Is_4874d0()!=1){std::printf("B1");return 1;} mode=0x19; if(Is_4874d0()!=1){std::printf("B2");return 1;} mode=3; if(Is_4874d0()!=0){std::printf("B3");return 1;} std::printf("RB_4874d0_OK\n"); return 0; }