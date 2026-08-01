extern "C" __declspec(naked) char* __cdecl strstr(const char* str, const char* substr) {
    __asm {
        _emit 0xff
        _emit 0x25
        _emit 0x0c
        _emit 0x01
        _emit 0x44
        _emit 0x01
    }
}