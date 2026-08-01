extern "C" __declspec(dllimport) int __cdecl real_puts_target(const char* str);
extern "C" int __cdecl puts(const char* s) { return real_puts_target(s); }