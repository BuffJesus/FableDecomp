#include <stdio.h>
#include <string.h>
#include "candidates/mouse_dx_accessors_compiled.h"

static CMousePrimitiveOwner g_owner;
static int g_clientCalls, g_rectCalls, g_offsetCalls, g_cursorCalls;
static int g_cursorX, g_cursorY;

CMousePrimitiveOwner* CMousePositionContext::ResolvePrimitiveOwner() { return &g_owner; }

static int __stdcall ClientToScreenStub(void*, CMousePoint* point)
{
    ++g_clientCalls;
    point->X = 100;
    point->Y = 200;
    return 1;
}

static int __stdcall GetClientRectStub(void*, CMouseRect* rect)
{
    ++g_rectCalls;
    rect->Left = 10;
    rect->Top = 20;
    rect->Right = 640;
    rect->Bottom = 480;
    return 1;
}

static int __stdcall OffsetRectStub(CMouseRect* rect, int x, int y)
{
    ++g_offsetCalls;
    rect->Left += x; rect->Right += x;
    rect->Top += y; rect->Bottom += y;
    return 1;
}

static int __stdcall SetCursorPosStub(int x, int y)
{
    ++g_cursorCalls;
    g_cursorX = x;
    g_cursorY = y;
    return 1;
}

CMouseClientToScreenFunction FableClientToScreen = ClientToScreenStub;
CMouseGetClientRectFunction FableGetClientRect = GetClientRectStub;
CMouseOffsetRectFunction FableOffsetRect = OffsetRectStub;
CMouseSetCursorPosFunction FableSetCursorPos = SetCursorPosStub;

void CMouseDX::GetPos(C2DVector*) const {}

int main()
{
    CMouseDX mouse;
    C2DVector position;
    memset(&g_owner, 0, sizeof(g_owner));
    g_owner.Window = &g_owner;
    g_clientCalls = g_rectCalls = g_offsetCalls = g_cursorCalls = 0;

    position.X = 3.75f;
    position.Y = 5.5f;
    mouse.DisableCursorWarp = 1;
    mouse.SetPos(&position);
    if (mouse.PositionX != position.X || mouse.PositionY != position.Y) return 1;
    if (mouse.CachedPositionX != position.X || mouse.CachedPositionY != position.Y) return 2;
    if (g_cursorCalls != 0) return 3;

    position.Y = 0.0f;
    mouse.DisableCursorWarp = 0;
    mouse.SetPos(&position);
    if (g_clientCalls != 1 || g_rectCalls != 1 || g_offsetCalls != 1 || g_cursorCalls != 1) return 4;
    if (g_cursorX != 113 || g_cursorY != 220) return 5;

    printf("MOUSE_DX_SETPOS_TEST PASS\n");
    return 0;
}
