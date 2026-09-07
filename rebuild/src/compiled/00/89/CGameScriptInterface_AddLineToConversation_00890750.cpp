// CGameScriptInterface::AddLineToConversation @ 0x00890750 (retail 97 bytes, ret 0x14)
// Genuine VC7.1 C++ reconstruction - no asm, no naked, no byte bakes.

class CCharString;
class CThing;
struct CScriptThing;

// CScriptThing is polymorphic (fable_types.h: size 0xc, +0x00 vftable).
// Only two slots are used here, so the vtable is modelled explicitly.
struct CScriptThingVTable
{
    unsigned char m_Pad0000[0x2C];
    // +0x2C : CThing* GetPThing() const
    CThing* (__fastcall* m_GetPThing)(const CScriptThing* self);
    unsigned char m_Pad0030[0x12C - 0x2C - 4];
    // +0x12C : bool IsAvailable() const
    bool (__fastcall* m_IsAvailable)(const CScriptThing* self);
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;   // +0x00
};

// retail 0x006E5950 - declared only, so the call rel32 stays a relocation.
class CScriptConversationManager
{
public:
    void AddLineToConversation(
        long               conversationId,
        CThing*            pSpeaker,
        CThing*            pListener,
        const CCharString* pLine,
        bool               playDuringCutScenes);
};

// CWorld overlay: scoped_ptr<CScriptConversationManager> at +0x7C is a bare pointer.
struct CGSIAddLineToConversationWorld
{
    unsigned char               m_Pad0000[0x7C];
    CScriptConversationManager* m_pConversationManager;   // +0x7C
};

// CGameScriptInterface: +0x00 vftable slot modelled as raw padding, +0x04 CWorld*.
struct CGameScriptInterface
{
    unsigned char                    m_Pad0000[0x04];
    CGSIAddLineToConversationWorld*  m_pWorld;            // +0x04

    void AddLineToConversation(
        long                conversationId,
        const CCharString&  line,
        bool                playDuringCutScenes,
        const CScriptThing& speaker,
        const CScriptThing& listener) const;
};

void CGameScriptInterface::AddLineToConversation(
    long                conversationId,
    const CCharString&  line,
    bool                playDuringCutScenes,
    const CScriptThing& speaker,
    const CScriptThing& listener) const
{
    if (speaker.m_pVTable->m_IsAvailable(&speaker))
    {
        CThing* pListener = 0;

        if (listener.m_pVTable->m_IsAvailable(&listener))
        {
            pListener = listener.m_pVTable->m_GetPThing(&listener);
        }

        CScriptConversationManager* const pManager = m_pWorld->m_pConversationManager;

        pManager->AddLineToConversation(
            conversationId,
            speaker.m_pVTable->m_GetPThing(&speaker),
            pListener,
            &line,
            playDuringCutScenes);
    }
}