#include <cstdio>
#include <cstdlib>

struct CPixelShaderRes { char pad[0x20]; int m_refCount; };
struct CPixelShader { char pad[0x24]; CPixelShaderRes* m_pRes; };

int __fastcall CountReferences(CPixelShader* this_)
{
    return this_->m_pRes->m_refCount;
}

int main()
{
    CPixelShaderRes res;
    res.m_refCount = 4242;
    CPixelShader obj;
    obj.m_pRes = &res;

    CPixelShader* pObj = &obj;
    int r;
    __asm {
        mov ecx, pObj
        call CountReferences
        mov r, eax
    }
    if (r != 4242) { printf("FAIL got %d\n", r); return 1; }

    res.m_refCount = -7;
    __asm {
        mov ecx, pObj
        call CountReferences
        mov r, eax
    }
    if (r != -7) { printf("FAIL got %d\n", r); return 1; }

    printf("OK_0x0051e480\n");
    return 0;
}