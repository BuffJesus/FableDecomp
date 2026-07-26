#include <stdio.h>
#include <string.h>
#include "candidates/mouse_dx_accessors_compiled.h"

static CCountedSurfaceRep g_rep;
static CSurface* g_copiedSurface;
static CMousePrimitiveHierarchy g_hierarchy;
static CMousePrimitiveSlotOwner g_owner;
static CMouseEnginePrimitive g_primitive;
static CMousePrimitiveDispatch g_dispatch;
static int g_resetCalls, g_resolveCalls, g_surfaceCalls, g_enableCalls;

CSurface::CSurface(const CSurface& other) { memcpy(Storage, other.Storage, sizeof(Storage)); }

void CCountedSurfacePointer::Reset(CSurface* surface)
{
    ++g_resetCalls;
    g_copiedSurface = surface;
    g_rep.Value = surface;
    Rep = &g_rep;
}

CMousePrimitiveHierarchy* ResolveMousePrimitiveHierarchy()
{
    ++g_resolveCalls;
    return &g_hierarchy;
}

static void __stdcall SetSurface(
    CMouseEnginePrimitive* primitive, int zeroA, int zeroB, void* surface)
{
    ++g_surfaceCalls;
    if (primitive != &g_primitive || zeroA != 0 || zeroB != 0 || surface != g_copiedSurface)
        g_surfaceCalls = -100;
}

static void __stdcall SetEnabled(CMouseEnginePrimitive* primitive, int enabled)
{
    ++g_enableCalls;
    if (primitive != &g_primitive || enabled != 1)
        g_enableCalls = -100;
}

int main()
{
    fable_u8 mouseStorage[0x4860];
    CMouseDX* mouse = reinterpret_cast<CMouseDX*>(mouseStorage);
    fable_u8 sourceStorage[sizeof(CSurface)];
    CSurface& source = *reinterpret_cast<CSurface*>(sourceStorage);
    memset(source.Storage, 0x5A, sizeof(source.Storage));
    memset(&g_dispatch, 0, sizeof(g_dispatch));
    g_dispatch.SetCursorSurface = SetSurface;
    g_dispatch.SetCursorEnabled = SetEnabled;
    g_primitive.Dispatch = &g_dispatch;
    g_owner.Primitive = &g_primitive;
    g_hierarchy.Owner = &g_owner;
    g_resetCalls = g_resolveCalls = g_surfaceCalls = g_enableCalls = 0;

    mouse->DisableCursorWarp = 0;
    mouse->SetMouseCursor(source);
    if (g_resetCalls != 1 || g_resolveCalls != 0 || g_copiedSurface == 0) return 1;
    if (memcmp(g_copiedSurface->Storage, source.Storage, sizeof(source.Storage)) != 0) return 2;
    delete g_copiedSurface;

    mouse->DisableCursorWarp = 1;
    mouse->SetMouseCursor(source);
    if (g_resetCalls != 2 || g_resolveCalls != 2) return 3;
    if (g_surfaceCalls != 1 || g_enableCalls != 1) return 4;
    delete g_copiedSurface;
    printf("MOUSE_DX_SET_MOUSE_CURSOR_TEST PASS\n");
    return 0;
}
