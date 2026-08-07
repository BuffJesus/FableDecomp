#include "rebuild_abi.h"
#include <cstdio>
static int g_h1=0,g_h2=0;
struct CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_656e3e(CTCMapwho *self){ g_h1=1; }
extern "C" void FABLE_CDECL FableH2_656e3e(CTCMapwho *self){ g_h2=1; }
struct CTCMapwho { char dummy; void OnDie(); };
void CTCMapwho::OnDie(){ if(!this) return; FableH1_656e3e(this); FableH2_656e3e(this); }
int main(){ CTCMapwho o; o.OnDie();
 if(!g_h1||!g_h2){std::printf("B1\n");return 1;}
 std::printf("OD_656e3e_OK\n"); return 0; }