#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_ime.h"
#include "fable_system_manager.h"
#include "fable_system_manager_state.h"
#include "fable_font_bank.h"
#include "fable_display_viewport.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
namespace NUISystem { struct CIMERecoveryTestAccess {
    static void Select(CIMEFont& font,CCharString& name,float& scale){font.GetFontNameAndScale(name,scale);}
}; }
static unsigned char* arena;static FILE* output;static unsigned mutation,refs,calls;
unsigned char g_IMEAlphaDefault_013750B0;
double g_FableVirtualWidth_013961E8=128.125,g_FableVirtualHeight_013961F0=256.25;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned value=0){
    unsigned header[]={kind,value};unsigned char copy[4096];memcpy(copy,arena,sizeof(copy));
    *(unsigned*)(copy+0x404)=0x2400f200;
    fwrite(header,4,2,output);fwrite(copy,1,sizeof(copy),output);
}
CCharString::CCharString(){storage_=0;event(1);}
CCharString::~CCharString(){event(9);}
const CCharString& CCharString::operator=(const char* text){
    if(strcmp(text,"ENG_ARIAL_"))event(99);event(2);
    strcpy((char*)(arena+0x600),text);storage_=(CCharStringData*)(arena+0x600);
    if(mutation==1)word(0x110)=28;
    return *this;
}
const CCharString& CCharString::operator+=(const char* text){
    event(3,(unsigned)atoi(text));strcat((char*)(arena+0x600),text);
    if(mutation==2)word(0x110)=7;
    return *this;
}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){
    event(4);++calls;
    if(mutation==3){word(0x108)=0x42800000+calls*0x10000;word(0x10c)=0x42000000+calls*0x10000;
        word(0x908)=(unsigned)(arena+((calls&1)?0xc00:0xb00));g_IMEAlphaDefault_013750B0=arena[0x850]=(unsigned char)(calls*37);}
    return (CSystemManager*)(arena+0x700);
}
CWideString::CWideString(const wchar_t* text){if(text!=(wchar_t*)(arena+0x680))event(99);*(unsigned*)this=(unsigned)(arena+0x680);event(12);}
CWideString::CWideString(const CWideString& other){if(*(const unsigned*)&other!=(unsigned)(arena+0x680))event(99);*(unsigned*)this=*(const unsigned*)&other;event(13);}
CWideString::~CWideString(){if(*(const unsigned*)this!=(unsigned)(arena+0x680))event(99);event(14);}
void CRenderManager2D::AddText(const C3DVector& position,float scale,const CWideString& text,const CFontBank& font,const CRGBColour& colour,unsigned long flags){
    memcpy(arena+0x800,&position,12);memcpy(arena+0x80c,&scale,4);memcpy(arena+0x810,&colour,4);
    word(0x814)=(unsigned)&font;word(0x818)=*(unsigned*)&text;word(0x81c)=flags;event(15,(unsigned)this);
}
template<> CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}
CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name){
    if(*(const unsigned*)&name!=(unsigned)(arena+0x600))event(99);event(5,(unsigned)this);
    CCountedPointer<CFontBank> result;result.Data=(CFontBank*)(arena+0x300);result.Info=refs?(CCPPointerInfo*)(arena+0x400):0;return result;
}
static C2DExtentsF* __fastcall extent(CFontBank* self,void*,C2DExtentsF* result,const wchar_t* text,bool flag){
    if(text!=(wchar_t*)(arena+0x680)||flag)event(99);event(6,(unsigned)self);
    memcpy(result,arena+0x620,8);return result;
}
static void FABLE_FASTCALL release(void* data){event(7,(unsigned)data);}
void __cdecl operator delete(void* data){event(8,(unsigned)data);}
int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x3004)=(unsigned)&extent;unsigned params[4],cases=0;const unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP};
    while(fread(params,4,4,input)==4){
        if(fread(arena,1,4096,input)!=4096)return 5;
        calls=0;g_IMEAlphaDefault_013750B0=arena[0x850];mutation=params[1];refs=word(0x400);word(0x404)=(unsigned)&release;_controlfp(modes[params[2]],_MCW_RC);
        NUISystem::CIMEFont* font=(NUISystem::CIMEFont*)(arena+0x100);
        if(params[0]==2)font->NUISystem::CIMEFont::DrawTextW((wchar_t*)(arena+0x680));
        else if(params[0])font->NUISystem::CIMEFont::GetTextExtent((wchar_t*)(arena+0x680),(unsigned long*)(arena+0x200),(unsigned long*)(arena+params[3]));
        else NUISystem::CIMERecoveryTestAccess::Select(*font,*(CCharString*)(arena+0x180),*(float*)(arena+0x200));
        event(10);++cases;
    }
    fclose(input);if(fclose(output))return 6;VirtualFree(arena,0,0x8000);
    printf("NATIVE_IME_FONT_MEASURE_PASS cases=%u\n",cases);return 0;
}
