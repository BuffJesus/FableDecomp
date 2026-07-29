#include <cstdio>
#include <cstring>

class CGSIStopSoundEmitter
{
public:
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
    virtual void StopSound(unsigned long soundId);
};

class CGameScriptInterface
{
public:
    virtual void StopSound(unsigned long soundId) const;
};

CGSIStopSoundEmitter* CGSIStopSound_Emitter;
static CGSIStopSoundEmitter* g_ObservedEmitter;
static unsigned long g_ObservedSoundId;
static unsigned long g_CallCount;

void __fastcall CGSIStopSound_VirtualStop(
    CGSIStopSoundEmitter* emitter,
    void*,
    unsigned long soundId)
{
    g_ObservedEmitter = emitter;
    g_ObservedSoundId = soundId;
    ++g_CallCount;
}

int main()
{
    void* vtable[15];
    std::memset(vtable, 0, sizeof(vtable));
    vtable[14] = (void*)&CGSIStopSound_VirtualStop;
    void* emitterObject[1] = { vtable };
    CGameScriptInterface interfaceObject;
    CGSIStopSound_Emitter =
        (CGSIStopSoundEmitter*)emitterObject;

    interfaceObject.StopSound(0);
    if (g_CallCount != 1 ||
        g_ObservedEmitter != CGSIStopSound_Emitter ||
        g_ObservedSoundId != 0)
        return 1;

    interfaceObject.StopSound(0xfedcba98);
    if (g_CallCount != 2 ||
        g_ObservedEmitter != CGSIStopSound_Emitter ||
        g_ObservedSoundId != 0xfedcba98)
        return 2;

    std::printf("FSE2_0088f660_TEST PASS\n");
    return 0;
}
