#include <math.h>
#include <stdio.h>
#include <string.h>
#include "rebuild_abi.h"
#include "fable_maths_tables.h"
void FABLE_FASTCALL GFInitMaths();
// Independent double-precision libm oracle, compiled /Od. Float outputs permit
// two float ULPs near pi for x87/libm rounding; endpoints and sentinels are exact.
int main(){
 for(int pass=0;pass<2;++pass){
  memset(g_FableInvSqrtTable_013CE558,0xcc,sizeof(g_FableInvSqrtTable_013CE558));
  memset(g_FableCosTable_013CD550,0xcc,sizeof(g_FableCosTable_013CD550));
  memset(g_FableAcosTable_013CB530,0xcc,sizeof(g_FableAcosTable_013CB530));
  memset(g_FableAsinTable_013CC540,0xcc,sizeof(g_FableAsinTable_013CC540));
  GFInitMaths();unsigned long hash=2166136261UL;
  for(int i=0;i<128;++i){if(g_FableInvSqrtTable_013CE558[i]>255)return 1;hash=(hash^g_FableInvSqrtTable_013CE558[i])*16777619UL;}
  if(hash!=0xcd6f07e5UL || g_FableInvSqrtTable_013CE558[64]!=255)return 2;
  for(int j=0;j<1024;++j){double expected=cos(j*0.006135923322290182);if(fabs(g_FableCosTable_013CD550[j]-expected)>0.00000024)return 3;}
  for(int k=0;k<=1024;++k){double x=(k-512)/512.0;
   if(fabs(g_FableAcosTable_013CB530[k]-acos(x))>0.00000024 || fabs(g_FableAsinTable_013CC540[k]-asin(x))>0.00000024)return 4;
   if(k && (g_FableAcosTable_013CB530[k]>g_FableAcosTable_013CB530[k-1] || g_FableAsinTable_013CC540[k]<g_FableAsinTable_013CC540[k-1]))return 5;
  }
  if(g_FableCosTable_013CD550[0]!=1.0f || g_FableCosTable_013CD550[1024]!=1.0f || g_FableAcosTable_013CB530[1024]!=0.0f || g_FableAsinTable_013CC540[512]!=0.0f || g_FableAcosTable_013CB530[1025]!=0.0f || g_FableAsinTable_013CC540[1025]!=0.0f)return 6;
 }
 puts("MATH_TABLES_PASS");return 0;
}