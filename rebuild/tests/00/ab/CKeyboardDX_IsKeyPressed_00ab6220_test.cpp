// Behaviour fixture for CKeyboardDX::IsKeyPressed (provider CKeyboardDX_IsKeyPressed_00ab6220.cpp; it does not define
// it). The keyboard is raw storage at the retail offsets fable_keyboard_dx.h pins, driven by a qualified call.
#include "fable_keyboard_dx.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char storage[sizeof(CKeyboardDX)];
static void Map(int key, unsigned short dik) { memcpy(storage + 0x351C + key * 2, &dik, 2); }

int main()
{
    memset(storage, 0, sizeof(storage));
    CKeyboardDX* k = reinterpret_cast<CKeyboardDX*>(storage);
    Map(KB_A, 0x1E); Map(KB_B, 0x30); Map(KB_Y, 0xFF);
    storage[0x341C + 0x1E] = 0x80;              // DIK_A down (DirectInput sets the high bit)
    storage[0x341C + 0xFF] = 0x01;              // any non-zero state byte counts
    CHECK(k->CKeyboardDX::IsKeyPressed(KB_A));
    CHECK(!k->CKeyboardDX::IsKeyPressed(KB_B));
    CHECK(k->CKeyboardDX::IsKeyPressed(KB_Y));
    storage[0x341C + 0x1E] = 0;
    CHECK(!k->CKeyboardDX::IsKeyPressed(KB_A));
    printf(g_fail ? "FAIL\n" : "KEYBOARD_DX_ISKEYPRESSED_OK\n");
    return g_fail != 0;
}
