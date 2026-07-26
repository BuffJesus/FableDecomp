// Behaviour test for RES_00ab4700 (CMouseDX::SetPos reconstruction).
// Standalone: redeclares the externs the reconstruction needs, defines
// recording stubs for the engine helpers/globals, drives the function with
// crafted inputs and checks observable behaviour. Prints exactly:
//   RES_00ab4700_TEST PASS
// on success and returns 0; returns nonzero on any failed check.
//
// Must NOT redefine CMouseDX_SetPos itself (that lives in the reconstruction
// TU; here we forward-declare it and link against it).

#include <cstdio>

typedef unsigned char u8;

struct C2DVector { float X; float Y; };
struct CMousePoint { long x; long y; };
struct CMouseRect { long left; long top; long right; long bottom; };

struct CMouseWindowOwner
{
    u8   pad00[0x94];
    void* Window;
};

struct CMouseDX
{
    u8    pad000[0x3414];
    float PositionX;
    float PositionY;
    u8    pad341C[0x18];
    float CachedPositionX;
    float CachedPositionY;
    u8    DisableCursorWarp;
};

// The reconstruction under test.
void __fastcall CMouseDX_SetPos(CMouseDX* self, void* edx, const C2DVector* position);

// ---- recording stubs for engine helpers/globals -------------------------

static CMouseWindowOwner g_owner;
static int g_resolveCalls = 0;

extern "C" CMouseWindowOwner* __cdecl ResolveMouseWindowOwner(void)
{
    ++g_resolveCalls;
    return &g_owner;
}

static void*      g_ctsWindow;
static CMousePoint* g_ctsPoint;
static int        g_ctsCalls;

static int __stdcall stub_ClientToScreen(void* w, CMousePoint* p)
{
    ++g_ctsCalls;
    g_ctsWindow = w;
    g_ctsPoint = p;
    // Simulate a window whose client origin sits at screen (100, 200).
    p->x = 100;
    p->y = 200;
    return 1;
}

static void*      g_grcWindow;
static int        g_grcCalls;

static int __stdcall stub_GetClientRect(void* w, CMouseRect* r)
{
    ++g_grcCalls;
    g_grcWindow = w;
    // Local client rect (0,0)-(640,480).
    r->left = 0; r->top = 0; r->right = 640; r->bottom = 480;
    return 1;
}

static int g_offDx, g_offDy, g_offCalls;

static int __stdcall stub_OffsetRect(CMouseRect* r, int dx, int dy)
{
    ++g_offCalls;
    g_offDx = dx; g_offDy = dy;
    r->left += dx; r->top += dy; r->right += dx; r->bottom += dy;
    return 1;
}

static int g_setX, g_setY, g_setCalls;

static int __stdcall stub_SetCursorPos(int x, int y)
{
    ++g_setCalls;
    g_setX = x; g_setY = y;
    return 1;
}

int (__stdcall *FableClientToScreen)(void*, CMousePoint*) = stub_ClientToScreen;
int (__stdcall *FableGetClientRect)(void*, CMouseRect*)   = stub_GetClientRect;
int (__stdcall *FableOffsetRect)(CMouseRect*, int, int)   = stub_OffsetRect;
int (__stdcall *FableSetCursorPos)(int, int)              = stub_SetCursorPos;

// -------------------------------------------------------------------------

static int fail(const char* why)
{
    std::printf("FAIL: %s\n", why);
    return 1;
}

int main()
{
    // Case 1: warp enabled (DisableCursorWarp == 0). Full path runs.
    {
        static CMouseDX m;
        for (int i = 0; i < (int)sizeof(m); ++i) ((u8*)&m)[i] = 0;
        m.DisableCursorWarp = 0;

        g_owner.Window = (void*)0xDEADBEEF;
        g_resolveCalls = g_ctsCalls = g_grcCalls = g_offCalls = g_setCalls = 0;

        C2DVector p;
        p.X = 12.75f;   // fractional -> exercises float add + ftol truncation
        p.Y = -3.5f;
        CMouseDX_SetPos(&m, 0, &p);

        // Position + cached fields written.
        if (m.PositionX != 12.75f) return fail("PositionX");
        if (m.CachedPositionX != 12.75f) return fail("CachedPositionX");
        if (m.PositionY != -3.5f) return fail("PositionY");
        if (m.CachedPositionY != -3.5f) return fail("CachedPositionY");

        // Helper wiring.
        if (g_resolveCalls != 1) return fail("resolve calls");
        if (g_ctsCalls != 1 || g_ctsWindow != (void*)0xDEADBEEF) return fail("ClientToScreen");
        if (g_grcCalls != 1 || g_grcWindow != (void*)0xDEADBEEF) return fail("GetClientRect");
        // OffsetRect got the screen client origin from ClientToScreen.
        if (g_offCalls != 1 || g_offDx != 100 || g_offDy != 200) return fail("OffsetRect args");

        // rect.left after offset = 0 + 100 = 100; x = ftol(100 + 12.75) = 112.
        // rect.top after offset  = 0 + 200 = 200; y = ftol(200 - 3.5) = 196.
        if (g_setCalls != 1) return fail("SetCursorPos calls");
        if (g_setX != 112) return fail("SetCursorPos x");
        if (g_setY != 196) return fail("SetCursorPos y");
    }

    // Case 2: warp disabled (DisableCursorWarp != 0). Only field writes; no
    // window/cursor calls happen.
    {
        static CMouseDX m;
        for (int i = 0; i < (int)sizeof(m); ++i) ((u8*)&m)[i] = 0;
        m.DisableCursorWarp = 1;

        g_resolveCalls = g_ctsCalls = g_grcCalls = g_offCalls = g_setCalls = 0;

        C2DVector p;
        p.X = 7.0f;
        p.Y = 9.0f;
        CMouseDX_SetPos(&m, 0, &p);

        if (m.PositionX != 7.0f || m.CachedPositionX != 7.0f) return fail("case2 X");
        if (m.PositionY != 9.0f || m.CachedPositionY != 9.0f) return fail("case2 Y");
        if (g_resolveCalls != 0 || g_ctsCalls != 0 || g_grcCalls != 0 ||
            g_offCalls != 0 || g_setCalls != 0)
            return fail("case2 early-return");
    }

    std::printf("RES_00ab4700_TEST PASS\n");
    return 0;
}
