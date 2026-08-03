struct CPixelShaderRes { char pad[0x20]; int m_refCount; };
struct CPixelShader { char pad[0x24]; CPixelShaderRes* m_pRes; };
int __fastcall CountReferences(CPixelShader* this_)
{
    return this_->m_pRes->m_refCount;
}