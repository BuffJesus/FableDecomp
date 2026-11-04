extern "C" __declspec(dllimport) void __cdecl free(void*);

extern "C" void __cdecl zcfree(void*, void* pointer)
{
    free(pointer);
}
