#include <cstdio>
struct CInner { int val; void EndLetterBox(){ val = 42; } };
struct CGameScriptInterface { char pad[0xc]; CInner* inner; };
static void __fastcall CGameScriptInterface_EndLetterBox(CGameScriptInterface* self)
{
    self->inner->EndLetterBox();
}
int main(){
    CInner inner; inner.val = 0;
    CGameScriptInterface obj; obj.inner = &inner;
    CGameScriptInterface_EndLetterBox(&obj);
    if (inner.val == 42) { std::printf("CGameScriptInterface_006e7290_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}