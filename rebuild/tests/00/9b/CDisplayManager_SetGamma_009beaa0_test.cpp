#include "fable_display_viewport.h"
#include "fable_gamma_ramp.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4],device[1],table[22];
static CDisplayManager* manager=reinterpret_cast<CDisplayManager*>(storage);
static unsigned calls,errors;
static unsigned long expectedBits;
static bool calibrate,mutate;
static unsigned short expected[256];
static unsigned long Bits(const float& f) { unsigned long v;memcpy(&v,&f,4);return v; }
static void FABLE_STDCALL SetRamp(IDirect3DDevice9* receiver,UINT chain,DWORD flags,const D3DGAMMARAMP* ramp) {
    ++calls;
    if(receiver!=reinterpret_cast<IDirect3DDevice9*>(device) || chain!=0 || flags!=unsigned(calibrate) || Bits(manager->Gamma)!=expectedBits) ++errors;
    for(unsigned i=0;i<256;++i)
        if(ramp->red[i]!=expected[i] || ramp->green[i]!=expected[i] || ramp->blue[i]!=expected[i]) ++errors;
    if(mutate) manager->Gamma=2.25f;
}
int main() {
    const unsigned long inputs[]={0,0x80000000,1,0x80000001,0x00800000,0xbf800000,
        0x3e999999,0x3e99999a,0x3e99999b,0x3f800000,0x3fa00000,0x40200000,
        0x40bfffff,0x40c00000,0x40c00001,0x41800000,0x7f800000,0xff800000,
        0x7fc12345,0xffc12345,0x7f812345};
    table[21]=reinterpret_cast<unsigned long>(&SetRamp);device[0]=reinterpret_cast<unsigned long>(table);
    unsigned scenarios=0;
    for(unsigned n=0;n<sizeof(inputs)/sizeof(inputs[0]);++n)
    for(unsigned flags=0;flags<4;++flags) {
        memset(storage,0xa5,sizeof(storage));manager->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(device);
        unsigned char before[sizeof(storage)];memcpy(before,storage,sizeof(storage));
        calibrate=(flags&1)!=0;mutate=(flags&2)!=0;
        float value;memcpy(&value,inputs+n,4);expectedBits=inputs[n];
        bool nan=(inputs[n]&0x7fffffffUL)>0x7f800000UL;
        double bounded=0;
        if(!nan) {
            bounded=value;
            if(value<0.3f) {expectedBits=0x3e99999a;bounded=static_cast<double>(0.3f);}
            else if(value>6.0f) {expectedBits=0x40c00000;bounded=6.0;}
        }
        for(unsigned i=0;i<256;++i) {
            double scaled=bounded*(i*256);
            if(scaled>65280.0) scaled=65280.0;
            expected[i]=nan?0:static_cast<unsigned short>(scaled);
        }
        calls=0;manager->SetGamma(value,calibrate);++scenarios;
        if(calls!=1 || Bits(manager->Gamma)!=(mutate?0x40100000UL:expectedBits)) ++errors;
        for(unsigned b=0;b<sizeof(storage);++b)
            if((b<0x2c4 || b>=0x2c8) && reinterpret_cast<unsigned char*>(storage)[b]!=before[b]) ++errors;
        if(errors) {printf("SET_GAMMA_FAIL input=%08lx flags=%u errors=%u\n",inputs[n],flags,errors);return 1;}
    }
    printf("SET_GAMMA_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
