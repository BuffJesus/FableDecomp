struct RefTarget {
    char pad0[0x74];
    unsigned long refs; // +0x74
};
struct CPixelShader {
    char pad0[0x40];
    RefTarget* inner; // +0x40
};
unsigned long __fastcall CountReferences(CPixelShader* self) {
    return self->inner->refs;
}