#include "fable_display_engine.h"
#include <new>
#include <stdio.h>
#include <string.h>

static void* engineStorage[3];
static unsigned calls, errors;
static unsigned long expected[3];
static bool result;
static bool __fastcall Pre(void* self, void*) {
    if (self!=engineStorage) ++errors;
    ++calls; return result;
}
static bool __fastcall Post(void* self, void*, unsigned long w, unsigned long h, unsigned long d) {
    if (self!=engineStorage || w!=expected[0] || h!=expected[1] || d!=expected[2]) ++errors;
    ++calls; return result;
}
int main() {
    unsigned scenarios=0;
    for (unsigned pattern=0; pattern<256; ++pattern) {
        unsigned long initBuffer[11]; memset(initBuffer,pattern,sizeof(initBuffer));
        CDisplayEngineInit* init=new(&initBuffer[1]) CDisplayEngineInit;
        if (init->Component || init->DefinitionManager || init->PlayerManager || init->World ||
            init->MainGraphicBank || init->Width || init->Height || init->MeshBank || init->PixelFormat.FormatIndex!=-1) return 1;
        unsigned long guard=pattern*0x01010101UL;
        if (initBuffer[0]!=guard || initBuffer[10]!=guard) return 2;
        ++scenarios;
        unsigned long displayBuffer[66]; memset(displayBuffer,pattern,sizeof(displayBuffer));
        CDisplayEngine* display=reinterpret_cast<CDisplayEngine*>(&displayBuffer[1]);
        void* slots[17]; memset(slots,0,sizeof(slots));
        slots[15]=reinterpret_cast<void*>(&Pre); slots[16]=reinterpret_cast<void*>(&Post);
        engineStorage[0]=slots; engineStorage[1]=reinterpret_cast<void*>(guard); engineStorage[2]=reinterpret_cast<void*>(~guard);
        display->Engine3D=reinterpret_cast<CIEngine*>(engineStorage);
        unsigned long before[66]; memcpy(before,displayBuffer,sizeof(before));
        for (unsigned r=0; r<2; ++r) {
            result=r!=0; calls=errors=0;
            expected[0]=guard; expected[1]=~guard; expected[2]=pattern;
            display->PreChangeResolution();
            display->PostChangeResolution(expected[0],expected[1],expected[2]);
            display->ChangeTextureColourDepth(long(guard));
            if (calls!=2 || errors || memcmp(before,displayBuffer,sizeof(before)) ||
                engineStorage[1]!=reinterpret_cast<void*>(guard) || engineStorage[2]!=reinterpret_cast<void*>(~guard)) return 3;
            ++scenarios;
        }
    }
    printf("DISPLAY_ENGINE_LEAVES_BEHAVIOR_PASS scenarios=%u\n",scenarios); return 0;
}
