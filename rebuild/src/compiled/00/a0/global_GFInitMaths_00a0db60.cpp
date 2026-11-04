#include <math.h>
#include "rebuild_abi.h"
#include "fable_maths_tables.h"
unsigned long g_FableInvSqrtTable_013CE558[128];
float g_FableCosTable_013CD550[1025];
float g_FableAcosTable_013CB530[1026];
float g_FableAsinTable_013CC540[1026];

void FABLE_FASTCALL GFInitMaths() {
    union FloatBits { unsigned long bits; float value; } input, reciprocal;
    for(unsigned long i=0;i<128;++i) {
        input.bits=(i|0x1f80UL)<<17;
        reciprocal.value=(float)(1.0/sqrt(input.value));
        g_FableInvSqrtTable_013CE558[i]=((reciprocal.bits+0x2000UL)>>15)&0xffUL;
    }
    g_FableInvSqrtTable_013CE558[64]=255;
    for(unsigned long j=0;j<1024;++j)
        g_FableCosTable_013CD550[j]=(float)cos((float)(j*2)*0.003067961661145091f);
    g_FableCosTable_013CD550[1024]=g_FableCosTable_013CD550[0];
    for(unsigned long k=0;k<1025;++k)
        g_FableAcosTable_013CB530[k]=(float)acos((float)k*0.001953125f-1.0f);
    g_FableAcosTable_013CB530[1025]=0.0f;
    for(unsigned long l=0;l<1025;++l)
        g_FableAsinTable_013CC540[l]=(float)asin((float)l*0.001953125f-1.0f);
    g_FableAsinTable_013CC540[1025]=0.0f;
}