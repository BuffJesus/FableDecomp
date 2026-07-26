#include <cstdio>

struct CSubField { int x; };

struct CSerializer {
    CSubField* lastArg;
    void Transfer(CSubField* p) { lastArg = p; }
};

struct CDragonActionHoverDef {
    char pad[0x28];
    CSubField field28;
};

void __fastcall CDragonActionHoverDef_Transfer(CDragonActionHoverDef* self, void* /*edx*/, CSerializer* s)
{
    s->Transfer(&self->field28);
}

int main()
{
    CDragonActionHoverDef def;
    CSerializer s;
    s.lastArg = 0;
    CDragonActionHoverDef_Transfer(&def, 0, &s);
    if (s.lastArg == &def.field28) {
        std::printf("CDragonActionHoverDef_00452ccd_TEST PASS\n");
        return 0;
    }
    std::printf("CDragonActionHoverDef_00452ccd_TEST FAIL\n");
    return 1;
}