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

extern CGSIStopSoundEmitter* CGSIStopSound_Emitter;

void CGameScriptInterface::StopSound(unsigned long soundId) const
{
    CGSIStopSound_Emitter->StopSound(soundId);
}
