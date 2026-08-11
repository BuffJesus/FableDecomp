#include <stdio.h>

struct CScriptedMapBrush;

void* __stdcall ResolveMapBrush(CScriptedMapBrush* a, CScriptedMapBrush* b);

struct CWorldMap {
    void ApplyScriptedMapBrush(CScriptedMapBrush* brush);
    void ConsumeMapBrush(void* p);
};

// instrumentation
static void* g_lastResolveA;
static void* g_lastResolveB;
static void* g_consumed;
static int   g_consumeCalls;

void* __stdcall ResolveMapBrush(CScriptedMapBrush* a, CScriptedMapBrush* b)
{
    g_lastResolveA = a;
    g_lastResolveB = b;
    return (void*)0xABCD1234;
}

void CWorldMap::ConsumeMapBrush(void* p)
{
    g_consumed = p;
    g_consumeCalls++;
}

int main()
{
    CWorldMap w;
    CScriptedMapBrush* brush = (CScriptedMapBrush*)0x1000;

    w.ApplyScriptedMapBrush(brush);

    bool ok = (g_lastResolveA == (void*)0x1000)
           && (g_lastResolveB == (void*)0x1000)
           && (g_consumeCalls == 1)
           && (g_consumed == (void*)0xABCD1234);

    printf(ok ? "APPLYBRUSH_OK\n" : "APPLYBRUSH_FAIL\n");
    return ok ? 0 : 1;
}