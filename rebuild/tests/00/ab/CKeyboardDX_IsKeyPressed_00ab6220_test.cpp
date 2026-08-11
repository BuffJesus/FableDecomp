#include <cstdio>
#include <cstring>

struct CKeyboardDX {
    char pad0[0x341c];
    unsigned char keyStates[0x100];   // at 0x341c
    unsigned short keyMap[0x100];      // at 0x351c
    bool IsKeyPressed(int key);
};

bool CKeyboardDX::IsKeyPressed(int key)
{
    if (this->keyStates[ this->keyMap[key] ])
        return true;
    return false;
}

int main()
{
    CKeyboardDX kb;
    memset(&kb, 0, sizeof(kb));

    // key 5 maps to slot 42, which is pressed
    kb.keyMap[5] = 42;
    kb.keyStates[42] = 1;

    // key 7 maps to slot 99, which is NOT pressed
    kb.keyMap[7] = 99;
    kb.keyStates[99] = 0;

    // key 9 maps to slot 100, pressed with value 0x80
    kb.keyMap[9] = 100;
    kb.keyStates[100] = 0x80;

    bool ok = true;
    if (kb.IsKeyPressed(5) != true)  ok = false;
    if (kb.IsKeyPressed(7) != false) ok = false;
    if (kb.IsKeyPressed(9) != true)  ok = false;
    if (kb.IsKeyPressed(0) != false) ok = false;

    if (ok) printf("ISKEYPRESSED_OK\n");
    else    printf("ISKEYPRESSED_FAIL\n");
    return ok ? 0 : 1;
}