struct CShaderPreParser {
    char pad[0x7d0];
    char* begin;
    char* end;
};

unsigned long __fastcall CShaderPreParser_GetConstantCount(const CShaderPreParser* self)
{
    return (unsigned long)((self->end - self->begin) >> 3);
}