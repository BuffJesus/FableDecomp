// CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes @ 0x00890b50
struct Inner {
    char pad[0x1c];
    void* f1c;      // +0x1c
};
struct CGameScriptInterface {
    char pad[8];
    Inner* f8;      // +0x8
};

// helper @ 0x449970 : __fastcall(void* that) -> void* (object)
void* __fastcall helper_449970(void* that);

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int /*edx*/, bool keep)
{
    unsigned char* obj = (unsigned char*)helper_449970(self->f8->f1c);
    obj[0x224] = (unsigned char)keep;
}