struct CInner {
    char pad[0x124];
    unsigned long refCount;
};

struct CPixelShader {
    char pad[0xe0];
    CInner* inner;
};

unsigned long __fastcall CPixelShader_CountReferences(CPixelShader* self)
{
    return self->inner->refCount;
}