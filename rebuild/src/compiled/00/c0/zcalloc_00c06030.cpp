extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int);

extern "C" void* __cdecl zcalloc(void*, unsigned int items, unsigned int size)
{
    return malloc(items * size);
}
