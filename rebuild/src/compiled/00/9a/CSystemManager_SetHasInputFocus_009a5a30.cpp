// CSystemManager::SetHasInputFocus @ 009a5a30
// void __fastcall (CSystemManager* this, bool focus)
//   this->m_hasInputFocus = focus;
//   if (this->m_focusSink) this->m_focusSink->NotifyFocus(focus);   // __fastcall tail-jmp

struct FocusSink
{
    void NotifyFocus(bool focus);   // external __fastcall member (direct jmp -> 0x9f4e20)
};

struct CSystemManager
{
    char pad0[0xa];
    bool m_hasInputFocus;   // +0xa
    char pad1[0x58 - 0xa - 1];
    FocusSink* m_focusSink;  // +0x58
};

void __fastcall CSystemManager_SetHasInputFocus(CSystemManager* self, int edx, bool focus)
{
    self->m_hasInputFocus = focus;
    FocusSink* sink = self->m_focusSink;
    if (sink)
        sink->NotifyFocus(focus);
}