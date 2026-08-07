#include <cstdio>
#include <cstring>

extern "C" void* FableDrawRenderManager_013b8384;
extern "C" void* FableDrawDisplayManager_013b8390;
extern "C" unsigned long FableDrawRenderStateDefault_01396e68;
extern "C" double FableDrawViewportX_013961e8;
extern "C" double FableDrawViewportY_013961f0;
extern "C" float g_timeValue;

extern "C" void __fastcall
CNewFrontendGameComponent_Draw_0042df9e(void*, void*);

static unsigned char g_renderManager[0x4000];
static unsigned char g_displayManager[0x40];
static unsigned char g_component[0x200];
static unsigned char g_ime[0x40];
static unsigned long g_engineVtable[9];
static unsigned char g_engine[0x40];
static int g_ctor;
static int g_time;
static int g_onCreate;
static int g_visibleArea;
static int g_clear;
static int g_begin;
static int g_getManager;
static int g_draw;
static int g_engineUpdate;
static int g_drawBuffered;
static int g_renderList;
static int g_addChild;
static int g_imeRender;
static int g_end;
static int g_swap;

extern "C" void NoteCtor() { ++g_ctor; }
extern "C" void NoteTime() { ++g_time; }
extern "C" void NoteOnCreate() { ++g_onCreate; }
extern "C" void NoteVisibleArea() { ++g_visibleArea; }
extern "C" void NoteClear() { ++g_clear; }
extern "C" void NoteBegin() { ++g_begin; }
extern "C" void NoteGetManager() { ++g_getManager; }
extern "C" void NoteDraw() { ++g_draw; }
extern "C" void NoteEngineUpdate() { ++g_engineUpdate; }
extern "C" void NoteDrawBuffered() { ++g_drawBuffered; }
extern "C" void NoteRenderList() { ++g_renderList; }
extern "C" void NoteAddChild() { ++g_addChild; }
extern "C" void NoteImeRender() { ++g_imeRender; }
extern "C" void NoteEnd() { ++g_end; }
extern "C" void NoteSwap() { ++g_swap; }

extern "C" __declspec(naked) void FableDrawInterpolationInfoCtor_00415a60()
{
    __asm
    {
        call NoteCtor
        ret
    }
}
extern "C" __declspec(naked) void FableDrawTime_009e1bc0()
{
    __asm
    {
        call NoteTime
        fld dword ptr [g_timeValue]
        ret
    }
}
extern "C" __declspec(naked) void FableDrawOnCreate_00a0b560()
{
    __asm
    {
        call NoteOnCreate
        ret 4
    }
}
extern "C" __declspec(naked) void FableDrawVisibleArea_009bece0()
{
    __asm
    {
        call NoteVisibleArea
        ret 4
    }
}
extern "C" __declspec(naked) void FableDrawClearRenderTarget_009d8cf0()
{
    __asm
    {
        call NoteClear
        ret 8
    }
}
extern "C" __declspec(naked) void FableDrawBeginRender_009bef20()
{
    __asm
    {
        call NoteBegin
        ret
    }
}
extern "C" void* FableDrawGetManager_00595582()
{ ++g_getManager; return g_renderManager; }
extern "C" __declspec(naked) void FableDrawManagerDraw_00595222()
{
    __asm
    {
        call NoteDraw
        ret 8
    }
}
extern "C" void* FableDrawGetManager_0041e5f2()
{ ++g_getManager; return g_renderManager; }
extern "C" __declspec(naked) void FableDrawRenderError_0041d03c()
{
    __asm
    {
        ret 4
    }
}
extern "C" __declspec(naked) void FableDrawDrawBufferedPolys_009d9c80()
{
    __asm
    {
        call NoteDrawBuffered
        ret
    }
}
extern "C" __declspec(naked) void FableDrawRender2DDrawList_009da9f0()
{
    __asm
    {
        call NoteRenderList
        ret 4
    }
}
extern "C" void* FableDrawAddChildPrimitive_00404a80()
{ ++g_addChild; return g_ime; }
extern "C" void FableDrawImeRender_00404c00()
{ ++g_imeRender; }
extern "C" void FableDrawEndRender_009bef50()
{ ++g_end; }
extern "C" void FableDrawSwapScreens_009beeb0()
{ ++g_swap; }

extern "C" double FableDrawViewportX_013961e8 = 0.0;
extern "C" double FableDrawViewportY_013961f0 = 0.0;
extern "C" unsigned long FableDrawRenderStateDefault_01396e68 = 0;
extern "C" void* FableDrawRenderManager_013b8384 = g_renderManager;
extern "C" void* FableDrawDisplayManager_013b8390 = g_displayManager;
extern "C" float g_timeValue = 1.0f;

extern "C" __declspec(naked) void EngineUpdate()
{
    __asm
    {
        call NoteEngineUpdate
        ret 8
    }
}

int main()
{
    std::memset(g_renderManager, 0, sizeof(g_renderManager));
    std::memset(g_component, 0, sizeof(g_component));
    std::memset(g_engine, 0, sizeof(g_engine));
    g_engineVtable[8] = reinterpret_cast<unsigned long>(&EngineUpdate);
    *reinterpret_cast<unsigned long**>(g_engine) = g_engineVtable;
    *reinterpret_cast<void**>(g_component + 0x58) = g_engine;

    unsigned char interpolation[0x80];
    std::memset(interpolation, 0, sizeof(interpolation));
    __asm {
        lea ecx, g_component
        lea eax, interpolation
        push eax
        call CNewFrontendGameComponent_Draw_0042df9e
    }

    if (
        g_ctor != 1 || g_time != 1 || g_onCreate != 1 ||
        g_visibleArea != 1 || g_clear != 1 || g_begin != 1 ||
        g_getManager != 2 || g_draw != 1 || g_engineUpdate != 1 ||
        g_drawBuffered != 2 || g_renderList != 2 || g_addChild != 1 ||
        g_imeRender != 1 || g_end != 1 || g_swap != 1)
    {
        std::printf("FRONTEND_DRAW_0042DF9E_TEST FAIL\n");
        return 1;
    }
    std::printf("FRONTEND_DRAW_0042DF9E_TEST PASS\n");
    return 0;
}
