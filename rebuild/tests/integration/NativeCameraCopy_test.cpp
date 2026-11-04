extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
#include <stdio.h>
#include <string.h>
#include <float.h>
#include "camera_copy_retail.h"
extern "C" void* __fastcall CopyCamera(void*,void*,const void*);
typedef void* (__fastcall *CopyFn)(void*,void*,const void*);
static unsigned char arena[512], expected[512], result[512];
static const unsigned bits[]={0,0x80000000,1,0x7fffff,0x800000,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffffffff};
int main() {
    void* code=VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);
    if(!code)return 1;
    memcpy(code,retailCode,sizeof(retailCode));
    const unsigned controls[]={0x37f,0x27f,0x7f,0x77f,0xb7f,0xf7f};
    unsigned cases=0;
    for(unsigned pattern=0;pattern<13;++pattern)for(unsigned flags=0;flags<4;++flags)
    for(unsigned position=0;position<3;++position)for(unsigned fill=0;fill<4;++fill)
    for(unsigned control=0;control<6;++control) {
        unsigned src=64,dst=position==0?128:position==1?256:64;
        for(unsigned run=0;run<2;++run) {
            for(unsigned i=0;i<512;++i)arena[i]=(unsigned char)(i*37+fill*53);
            for(i=0;i<14;++i)memcpy(arena+src+4*i,&bits[(pattern+i)%13],4);
            arena[src+36]=(unsigned char)(flags==3?255:flags&1);
            arena[src+37]=(unsigned char)(flags==3?255:flags>>1);
            memcpy(expected,arena,512);
            // Copy named members only; bytes 38 and 39 are padding.
            for(i=0;i<56;++i)if(i!=38&&i!=39)expected[dst+i]=arena[src+i];
            _control87(controls[control],0xffff);
            void* out=(run?CopyCamera:(CopyFn)code)(arena+dst,0,arena+src);
            if(out!=arena+dst||memcmp(arena,expected,512))return 2;
            if(run&&memcmp(arena,result,512))return 3;
            memcpy(result,arena,512);
        }
        ++cases;
    }
    VirtualFree(code,0,0x8000);
    printf("NATIVE_CAMERA_COPY_PASS cases=%u snapshots=%u\n",cases,cases*2);
    return 0;
}
