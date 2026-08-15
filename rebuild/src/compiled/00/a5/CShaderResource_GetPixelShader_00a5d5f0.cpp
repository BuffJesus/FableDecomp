struct CCharString;
struct CPixelShader { int a, b, c, d; };
// 0x9ccdc0 : native __fastcall member (ecx=this, name on stack, no edx dummy)
struct CSubResource {
    void* lookup(CCharString* name);
};
struct CShaderResource {
    void* pad0; CSubResource* sub;
    // 0xa5d510 : native __fastcall member (ecx=this, (ret,found) on stack, no edx dummy)
    CPixelShader* emit(CPixelShader* ret, void* found);
};
CPixelShader* __fastcall GetPixelShader(CShaderResource* self, void* edx, CPixelShader* ret, void* name)
{
    void* found = self->sub->lookup((CCharString*)name);
    self->emit(ret, found);
    return ret;
}