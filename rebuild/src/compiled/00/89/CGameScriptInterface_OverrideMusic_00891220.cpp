enum EMusicSetType
{
    MUSIC_SET_NONE = 0
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

void CGameScriptInterface::OverrideMusic(
    EMusicSetType musicSetType,
    bool enabled,
    bool forcePlay) const
{
    if (enabled)
    {
        if (CGSIOverrideMusic_Emitter != 0)
        {
            CGSIOverrideMusicCodeSection* section =
                CGSIOverrideMusic_GetCodeSection();
            volatile unsigned long value;
            if (section->useAlternate != 0)
                value = section->alternateValue;
            else
                value = section->primaryValue;
            CGSIOverrideMusic_Emitter->Stop(0x100, value, 500.0f);
        }
        inner->event->AddUBYTE((unsigned char)musicSetType);
        return;
    }
    inner->event->SetOverrideMusic(musicSetType, forcePlay);
}
