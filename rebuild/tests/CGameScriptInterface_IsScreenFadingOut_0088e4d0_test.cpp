#include <cstdio>

struct GSISub { char pad0[0xbc]; unsigned char b_bc; char pad1[0xc]; unsigned char b_c9; };
struct CGameScriptInterface { char pad0[0xc]; GSISub* sub; };

int __fastcall CGameScriptInterface_IsScreenFadingOut(const CGameScriptInterface* self)
{
    const GSISub* s = self->sub;
    if (s->b_bc) {
        if (s->b_c9) {
            return 1;
        }
    }
    return 0;
}

int main()
{
    GSISub node;
    CGameScriptInterface obj;
    obj.sub = &node;
    const CGameScriptInterface* p = &obj;

    node.b_bc = 1; node.b_c9 = 1;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 1) { std::printf("FAIL both-set\n"); return 1; }

    node.b_bc = 1; node.b_c9 = 0;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL c9-zero\n"); return 1; }

    node.b_bc = 0; node.b_c9 = 1;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL bc-zero\n"); return 1; }

    node.b_bc = 0; node.b_c9 = 0;
    if (CGameScriptInterface_IsScreenFadingOut(p) != 0) { std::printf("FAIL both-zero\n"); return 1; }

    std::printf("CGameScriptInterface_0088e4d0_TEST PASS\n");
    return 0;
}