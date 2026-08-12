// CTCDoor::ScriptTriggerOpen  retail 0x007305d0
// void __fastcall CTCDoor::ScriptTriggerOpen(CTCDoor* this, bool open)

struct CDoorInner
{
    void Toggle();   // direct call 0x4c7c40 (defined in another TU)
};

struct CTCDoor
{
    void*        m0;      // +0x00
    CDoorInner*  m4;      // +0x04
    char         pad8[8]; // +0x08 .. +0x0f
    bool         m10;     // +0x10  current open state
    void ScriptTriggerOpen(bool open);
};

void CTCDoor::ScriptTriggerOpen(bool open)
{
    if (this->m10 != open)
    {
        this->m4->Toggle();
    }
    this->m10 = open;
}