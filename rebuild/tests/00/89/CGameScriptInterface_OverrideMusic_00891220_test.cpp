#include <cstdio>
#include <cstring>

enum EMusicSetType
{
    MUSIC_SET_NONE = 0,
    MUSIC_SET_TEST = 0x12345678
};

struct CGSIOverrideMusicCodeSection
{
    unsigned char pad00[0x0c];
    unsigned char useAlternate;
    unsigned char pad0d[0x17];
    unsigned long primaryValue;
    unsigned char pad28[0xbc];
    unsigned long alternateValue;
};

struct CGSIOverrideMusicEvent
{
    void AddUBYTE(unsigned char musicSetType);
    void SetOverrideMusic(EMusicSetType musicSetType, bool forcePlay);
};

struct CGSIOverrideMusicInner
{
    unsigned char pad00[0x6c];
    CGSIOverrideMusicEvent* event;
};

struct CGSIOverrideMusicEmitter
{
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0c();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1c();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2c();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3c();
    virtual void Slot40();
    virtual void Slot44();
    virtual void Slot48();
    virtual void Slot4c();
    virtual void Slot50();
    virtual void Slot54();
    virtual void Slot58();
    virtual void Slot5c();
    virtual void Slot60();
    virtual void Slot64();
    virtual void Slot68();
    virtual void Slot6c();
    virtual void Slot70();
    virtual void Slot74();
    virtual void Slot78();
    virtual void Slot7c();
    virtual void Slot80();
    virtual void Slot84();
    virtual void Slot88();
    virtual void Slot8c();
    virtual void Slot90();
    virtual void Slot94();
    virtual void Slot98();
    virtual void Slot9c();
    virtual void Slota0();
    virtual void Slota4();
    virtual void Slota8();
    virtual void Slotac();
    virtual void Slotb0();
    virtual void Slotb4();
    virtual void Stop(unsigned long command, unsigned long value, float fade);
};

struct CGameScriptInterface
{
    CGSIOverrideMusicInner* inner;

    virtual void OverrideMusic(
        EMusicSetType musicSetType,
        bool enabled,
        bool forcePlay) const;
};

extern CGSIOverrideMusicEmitter* CGSIOverrideMusic_Emitter;
extern CGSIOverrideMusicCodeSection* __cdecl
    CGSIOverrideMusic_GetCodeSection();

CGSIOverrideMusicEmitter* CGSIOverrideMusic_Emitter;
static CGSIOverrideMusicCodeSection g_section;
static int g_addCalls;
static int g_setCalls;
static unsigned char g_addedMusicSet;
static EMusicSetType g_setMusicSet;
static bool g_forcePlay;
static unsigned long g_command;
static unsigned long g_value;
static float g_fade;

CGSIOverrideMusicCodeSection* __cdecl
CGSIOverrideMusic_GetCodeSection()
{
    return &g_section;
}

void CGSIOverrideMusicEvent::AddUBYTE(unsigned char musicSetType)
{
    ++g_addCalls;
    g_addedMusicSet = musicSetType;
}

void CGSIOverrideMusicEvent::SetOverrideMusic(
    EMusicSetType musicSetType,
    bool forcePlay)
{
    ++g_setCalls;
    g_setMusicSet = musicSetType;
    g_forcePlay = forcePlay;
}

void __fastcall CGSIOverrideMusic_VirtualStop(
    CGSIOverrideMusicEmitter*,
    void*,
    unsigned long command,
    unsigned long value,
    float fade)
{
    g_command = command;
    g_value = value;
    g_fade = fade;
}

int main()
{
    void* vtable[47];
    std::memset(vtable, 0, sizeof(vtable));
    vtable[46] = (void*)&CGSIOverrideMusic_VirtualStop;
    void* emitterObject[1] = { vtable };
    CGSIOverrideMusicEvent event;
    CGSIOverrideMusicInner inner;
    CGameScriptInterface self;
    std::memset(&inner, 0, sizeof(inner));
    self.inner = &inner;
    inner.event = &event;

    CGSIOverrideMusic_Emitter = 0;
    self.OverrideMusic(MUSIC_SET_TEST, true, false);
    if (g_addCalls != 1 || g_setCalls != 0 ||
        g_addedMusicSet != 0x78)
        return 1;

    CGSIOverrideMusic_Emitter =
        (CGSIOverrideMusicEmitter*)emitterObject;
    std::memset(&g_section, 0, sizeof(g_section));
    g_section.primaryValue = 77;
    self.OverrideMusic(MUSIC_SET_TEST, true, true);
    if (g_command != 0x100 || g_value != 77 ||
        g_fade != 500.0f || g_addCalls != 2)
        return 2;

    g_section.useAlternate = 1;
    g_section.alternateValue = 99;
    self.OverrideMusic(MUSIC_SET_TEST, true, false);
    if (g_value != 99 || g_addCalls != 3)
        return 3;

    self.OverrideMusic(MUSIC_SET_TEST, false, true);
    if (g_addCalls != 3 || g_setCalls != 1 ||
        g_setMusicSet != MUSIC_SET_TEST || !g_forcePlay)
        return 4;

    self.OverrideMusic(MUSIC_SET_NONE, false, false);
    if (g_setCalls != 2 || g_setMusicSet != MUSIC_SET_NONE ||
        g_forcePlay)
        return 5;

    std::printf("FSE2_00891220_TEST PASS\n");
    return 0;
}
