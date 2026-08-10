__declspec(naked) void GetActionName_2()
{
    __asm {
        _emit 0x56
        _emit 0x6A
        _emit 0xFF
        _emit 0x68
        _emit 0xCC
        _emit 0xB9
        _emit 0x23
        _emit 0x01
        _emit 0x8B
        _emit 0xF1
        _emit 0xE8
        _emit 0x98
        _emit 0xA9
        _emit 0x4C
        _emit 0x00
        _emit 0x8B
        _emit 0xC6
        _emit 0x5E
        _emit 0xC3
    }
}