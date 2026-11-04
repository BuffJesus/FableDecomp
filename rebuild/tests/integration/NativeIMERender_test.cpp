#include <stdio.h>
#include "fable_ime.h"
#include "fable_nui_manager.h"
#include "fable_nui_sprite.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern unsigned char g_IMEAlphaDefault_013750B0;
static unsigned char* arena; static FILE* output; static unsigned mutation;
static unsigned& word(unsigned offset) { return *(unsigned*)(arena+offset); }
static void event(unsigned kind,unsigned value=0) {
    unsigned header[]={kind,value};fwrite(header,4,2,output);
    fwrite(arena,1,4096,output);fwrite(&g_IMEAlphaDefault_013750B0,1,1,output);
}
static const CRGBColour& __fastcall colour(void* self,void*) {
    event(1,(unsigned)self);
    if(mutation&1)word(0x14c)=0;
    return *(CRGBColour*)(arena+((mutation&1)?0x420:0x400));
}
void FABLE_FASTCALL ImeUi_RenderUI(bool indicator,bool composition) {
    event(2,(unsigned)indicator|((unsigned)composition<<8));
    if(mutation&2)word(0x5b8)=word(0x5b8)?0:(unsigned)(arena+0x780);
}
NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance() {
    event(3);return (CManager*)(arena+((mutation&4)?0x900:0x500));
}
void NUISystem::CSprite::DrawWithRenderManager() { event(4,(unsigned)this); }
int main(int argc,char** argv) {
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    if(g_IMEAlphaDefault_013750B0!=255)return 4;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 5;
    word(0x306c)=(unsigned)&colour;unsigned params[2],cases=0;
    while(fread(params,4,2,input)==2) {
        if(fread(arena,1,4096,input)!=4096)return 6;
        mutation=params[0];g_IMEAlphaDefault_013750B0=(unsigned char)params[1];
        ((NUISystem::CIME*)(arena+0x100))->Render();event(5);++cases;
    }
    fclose(input);if(fclose(output))return 7;VirtualFree(arena,0,0x8000);
    printf("NATIVE_IME_RENDER_PASS cases=%u\n",cases);return 0;
}
