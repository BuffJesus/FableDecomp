#include <cstdio>

extern "C" void* __fastcall Dest_val_music_resolve(void* pairPtr)
{
    // Pretend the "resolved" sub-object pointer is pairPtr + 4 (past the long).
    return (char*)pairPtr + 4;
}

static void* g_dtorArg = 0;

extern "C" void __fastcall Dest_val_music_dtor(void* self)
{
    g_dtorArg = self;
}

extern "C" void __fastcall Dest_val_pair_music(void* pairPtr, void* alloc)
{
    Dest_val_music_dtor(Dest_val_music_resolve(pairPtr));
}

int main()
{
    char buf[16] = {0};
    void* allocDummy = (void*)0x1234;
    Dest_val_pair_music(buf, allocDummy);
    if (g_dtorArg == (void*)(buf + 4))
    {
        std::printf("DEST_VAL_PAIR_MUSIC_OK\n");
        return 0;
    }
    std::printf("FAIL %p %p\n", g_dtorArg, (void*)(buf + 4));
    return 1;
}