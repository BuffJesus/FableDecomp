#include <stdio.h>
#include <new>
#include <string.h>
#include <float.h>
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_2d_vector.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
struct CRenderManager2DTestAccess {
    static void Add(CRenderManager2D& r,const C3DVector& p,float scale,const CWideString& s,const CFontBank& f,const CRGBColour& c,unsigned long flags){r.AddText(p,scale,s,f,c,flags);}
};
static unsigned char* arena;static FILE* output;static unsigned calls,mutation;
double g_FableVirtualWidth_013961E8=128.125,g_FableVirtualHeight_013961F0=256.25;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned k,unsigned v=0){unsigned h[]={k,v};fwrite(h,4,2,output);fwrite(arena,1,0x6000,output);}
CWideString::CWideString(){*(unsigned*)this=0;if((unsigned)this<(unsigned)arena||(unsigned)this>=(unsigned)(arena+0x6000))event(1);}
CWideString& CWideString::operator=(const CWideString& s){
    if(&s!=(CWideString*)(arena+0x840))event(99);*(unsigned*)this=*(const unsigned*)&s;event(2);
    if(mutation==2){word(0x848)=0xabcdefff;word(0x850)=0x43800000;}return *this;
}
CWideString::~CWideString(){event(3);}
void CTexture::Uninitialise(){if(PD3DTexture||ByteLength||AllocationSource)event(99);event(4);}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){
    event(5);++calls;if(mutation==1){word(0x804)=0x42000000+calls*0x10000;word(0x808)=0x3f000000+calls*0x10000;word(0x294)=640+calls*79;word(0x298)=480+calls*53;}
    return (CSystemManager*)(arena+0x700);
}
CWideString::CWideString(const CWideString& source){*(unsigned*)this=*(const unsigned*)&source;event(6,(unsigned)this);if(mutation==2)word(0x828)=0x43480000;}
static unsigned long __stdcall addRef(void* self){if(self!=(void*)(arena+0xa00))event(99);event(9,(unsigned)self);++word(0xa04);if(mutation==1){word(0x834)=0xfedcba98;word(0x838)=(unsigned)(arena+0x980);}return word(0xa04);}
extern "C" void* __cdecl FableQueueTestMalloc(size_t size){event(7,(unsigned)size);return arena+0x5300;}
extern "C" void __cdecl FableQueueTestFree(void* value){event(10,(unsigned)value);}

int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x20000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x16004)=(unsigned)&addRef;unsigned params[4],cases=0;const unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP};
    while(fread(params,4,4,input)==4){
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;mutation=params[0];calls=0;_controlfp(modes[params[1]],_MCW_RC);
        if(params[3]==3){_STL::vector<CQuickDrawTriInfo>& v=*(_STL::vector<CQuickDrawTriInfo>*)(arena+0x4e94);v._M_insert_overflow(v.begin()+word(0x860),*(CQuickDrawTriInfo*)word(0x86c),_STL::__false_type(),word(0x864),word(0x868)!=0);}
        else if(params[3]==1)new(arena+0x900)CQuickDrawTriInfo(*(CQuickDrawTriInfo*)(arena+0x800));
        else if(params[3]==2)new(arena+0x900)CTexture(*(CTexture*)(arena+0x800));
        else CRenderManager2DTestAccess::Add(*(CRenderManager2D*)(arena+0x1000),*(C3DVector*)(arena+0x800),*(float*)(arena+0x850),*(CWideString*)(arena+0x840),*(CFontBank*)(arena+0x900),*(CRGBColour*)(arena+0x848),0xfedcba98);
        event(8);++cases;
    }
    fclose(input);if(fclose(output))return 6;VirtualFree(arena,0,0x8000);printf("NATIVE_ADD_TEXT_PASS cases=%u\n",cases);return 0;
}
