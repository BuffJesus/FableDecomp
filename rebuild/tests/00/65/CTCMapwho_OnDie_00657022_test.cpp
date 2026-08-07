#include "rebuild_abi.h"
#include <cstdio>
static int g_h1=0,g_h2=0;
struct CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_657022(CTCMapwho *self){ g_h1=1; }
extern "C" void FABLE_CDECL FableH2_657022(CTCMapwho *self){ g_h2=1; }
struct CTCMapwho { char dummy; void OnDie(); };
void CTCMapwho::OnDie(){ if(!this) return; FableH1_657022(this); FableH2_657022(this); }
int main(){ CTCMapwho o; o.OnDie();
 if(!g_h1||!g_h2){std::printf("B1\n");return 1;}
 std::printf("OD_657022_OK\n"); return 0; }