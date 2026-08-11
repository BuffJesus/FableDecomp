#include <stdio.h>

struct CSoundPair {
    unsigned long a;
    unsigned long b;
};

struct SoundEntry {
    unsigned long first;
    CSoundPair second;
};

void __fastcall _Fill_SoundEntry(SoundEntry* _First, SoundEntry* _Last, const SoundEntry& _Val);

int main()
{
    SoundEntry buf[5];
    for (int i = 0; i < 5; ++i) { buf[i].first = 0xDEAD; buf[i].second.a = 0; buf[i].second.b = 0; }

    SoundEntry val;
    val.first = 0x11111111;
    val.second.a = 0x22222222;
    val.second.b = 0x33333333;

    // fill middle 3 [1,4)
    _Fill_SoundEntry(&buf[1], &buf[4], val);

    int ok = 1;
    if (buf[0].first != 0xDEAD) ok = 0;
    if (buf[4].first != 0xDEAD) ok = 0;
    for (int i = 1; i <= 3; ++i) {
        if (buf[i].first != 0x11111111) ok = 0;
        if (buf[i].second.a != 0x22222222) ok = 0;
        if (buf[i].second.b != 0x33333333) ok = 0;
    }

    // empty range = no-op
    SoundEntry one; one.first = 7; one.second.a = 7; one.second.b = 7;
    _Fill_SoundEntry(&one, &one, val);
    if (one.first != 7) ok = 0;

    printf(ok ? "FILL_PASS_OK\n" : "FILL_FAIL\n");
    return ok ? 0 : 1;
}