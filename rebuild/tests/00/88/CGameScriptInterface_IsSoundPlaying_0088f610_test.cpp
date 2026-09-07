#include <cstdio>

struct SoundProvider
{
    void** vtable;
};

typedef bool (__fastcall *FnIsSoundPlaying)(SoundProvider*, unsigned long);

static SoundProvider* g_provider;
static SoundProvider* g_seenProvider;
static unsigned long g_seenId;
static unsigned long g_calls;

static bool __fastcall MockIsSoundPlaying(SoundProvider* provider, unsigned long soundId)
{
    g_seenProvider = provider;
    g_seenId = soundId;
    ++g_calls;
    return soundId == 0xC0FFEEUL;
}

static bool Dispatch(unsigned long soundId)
{
    return ((FnIsSoundPlaying)g_provider->vtable[15])(g_provider, soundId);
}

int main()
{
    void* slots[16] = {0};
    SoundProvider provider;
    slots[15] = (void*)&MockIsSoundPlaying;
    provider.vtable = slots;
    g_provider = &provider;

    if (!Dispatch(0xC0FFEEUL) || Dispatch(7UL)) return 1;
    if (g_calls != 2 || g_seenProvider != &provider || g_seenId != 7UL) return 2;
    std::printf("IS_SOUND_PLAYING_PASS\n");
    return 0;
}