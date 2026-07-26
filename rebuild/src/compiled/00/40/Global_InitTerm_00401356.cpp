typedef void (__cdecl* InitTermFunction)();

extern "C" __declspec(naked) void __cdecl initterm(
    InitTermFunction*, InitTermFunction*)
{
    __asm {
        _emit 0xff
        _emit 0x25
        _emit 0x90
        _emit 0x01
        _emit 0x44
        _emit 0x01
    }
}
