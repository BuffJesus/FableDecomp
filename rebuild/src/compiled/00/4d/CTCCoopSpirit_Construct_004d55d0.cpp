#pragma optimize("s",on)
#include <new>

struct CThing;

struct CTCCoopSpirit
{
    unsigned char storage[0x34];
    CTCCoopSpirit(CThing* thing);
    static CTCCoopSpirit* __fastcall Construct(CThing* thing);
};

CTCCoopSpirit* __fastcall CTCCoopSpirit::Construct(CThing* thing)
{
    void* memory = ::operator new(0x34);
    if (memory != 0) {
        return new (memory) CTCCoopSpirit(thing);
    }
    return 0;
}