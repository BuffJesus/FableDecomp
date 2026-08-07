#include "rebuild_abi.h"
extern "C" int FABLE_FASTCALL Mode_4874d0();
extern "C" char FABLE_FASTCALL Is_4874d0(){ int m=Mode_4874d0(); if(m==0x16) return 1; return (char)(m==0x19); }