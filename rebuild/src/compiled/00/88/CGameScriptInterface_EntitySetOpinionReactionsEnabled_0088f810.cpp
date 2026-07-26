class CScriptThing;

struct CCharString
{
    void* m_pRep; // 0x00
    CCharString(const char* pText, int length);
    ~CCharString();
};

struct ISlot
{
    virtual void Slot(const void* entity, const CCharString* tcName, int enabled) = 0;
};

struct CGameScriptInterface
{
    void** vt; // +0x0
    void EntitySetOpinionReactionsEnabled(
        const CScriptThing& entity, int enabled) const;
};

typedef void (ISlot::*SlotMFP)(const void* entity, const CCharString* tcName, int enabled);

extern const char kName[];

void CGameScriptInterface::EntitySetOpinionReactionsEnabled(
    const CScriptThing& entity, int enabled) const
{
    CCharString tcName(kName, -1);
    ISlot* obj = (ISlot*)this;
    SlotMFP mfp;
    *(void**)&mfp = this->vt[0x938 / 4];
    (obj->*mfp)((const void*)&entity, &tcName, enabled);
}