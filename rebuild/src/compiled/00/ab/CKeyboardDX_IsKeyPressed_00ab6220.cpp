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