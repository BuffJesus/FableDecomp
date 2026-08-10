struct CPixelShader {
    void* f0;
    struct Inner { char pad[0x1c]; unsigned long refs; } * f4;
};

unsigned long __fastcall CPixelShader_CountReferences(CPixelShader* self)
{
    return self->f4->refs;
}