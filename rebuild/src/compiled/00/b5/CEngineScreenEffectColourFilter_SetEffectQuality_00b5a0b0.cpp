struct CEngineVideoOptionsDef {
    char pad[0x72];
    unsigned char flag72;   // +0x72
};

struct CEngineScreenEffectColourFilter {
    char pad0[8];
    unsigned char enabled;  // +0x08
    char pad1[0x44 - 9];
    unsigned char field44;  // +0x44
};

// __fastcall modeled: self in ecx, one stack arg (def). __fastcall with a
// dummy edx placeholder pushes the real arg onto the stack -> ret 4.
void __fastcall SetEffectQuality(CEngineScreenEffectColourFilter* self, int /*edx*/, CEngineVideoOptionsDef* def)
{
    if (def->flag72 && self->field44)
        self->enabled = 1;
    else
        self->enabled = 0;
}