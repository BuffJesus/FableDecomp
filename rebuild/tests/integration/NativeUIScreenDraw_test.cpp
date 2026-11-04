#include <stdio.h>
#include <string.h>
#include "fable_nui_manager.h"

extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*, unsigned long, unsigned long);
static unsigned char* arena;
static FILE* output;
static unsigned count, mutation, calls;
static unsigned& word(unsigned offset) { return *(unsigned*)(arena+offset); }
static void event(unsigned kind, unsigned a=0, unsigned b=0, unsigned c=0) {
    unsigned data[] = {kind, a, b, c};
    fwrite(data, 4, 4, output);
    fwrite(arena, 1, 4096, output);
}
static void __fastcall draw(void* self, void*, CIEngine& engine,
    CEnginePrimitiveHandle* handle, long layer, long* counter, NUISystem::CComponent* viewport)
{
    if (layer || counter || viewport) { event(99); return; }
    event(1, (unsigned)self, (unsigned)&engine, (unsigned)handle);
    unsigned index = ((unsigned char*)self-arena-0x800)/16;
    if (mutation == 1 && index+1 < count)
        word(0x400+(index+1)*32+20) = 0;
    if (mutation == 2 && index+1 < count)
        word(0x154) = (unsigned)(arena+0x400+(index+1)*32);
    ++calls;
}
int main(int argc, char** argv)
{
    if (argc != 3) return 2;
    FILE* input = fopen(argv[1], "rb"); output = fopen(argv[2], "wb");
    if (!input || !output) return 3;
    arena = (unsigned char*)VirtualAlloc((void*)0x24000000, 65536, 0x3000, 4);
    if (arena != (void*)0x24000000) return 4;
    word(0x3008) = (unsigned)&draw;
    unsigned parameters[4], cases=0;
    while (fread(parameters, 4, 4, input) == 4) {
        if (fread(arena, 1, 4096, input) != 4096) return 5;
        count=parameters[0]; mutation=parameters[1]; calls=0;
        ((NUISystem::CFrontEndManager*)(arena+0x100))->Draw(
            *(CIEngine*)(arena+0xa00+parameters[2]*16),
            parameters[3] ? (CEnginePrimitiveHandle*)(arena+0xa80) : 0);
        event(2, calls); ++cases;
    }
    fclose(input); if (fclose(output)) return 6;
    VirtualFree(arena, 0, 0x8000);
    printf("NATIVE_UI_SCREEN_DRAW_PASS cases=%u\n", cases);
    return 0;
}
