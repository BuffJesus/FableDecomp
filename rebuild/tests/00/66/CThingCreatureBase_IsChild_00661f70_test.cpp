
#include <cstdio>
struct CTCBData {
    char pad[0xC4];
    int lifeStage;
};
struct CThingCreatureBase {
    char pad[0xE0];
    CTCBData* data;
};
bool __fastcall CThingCreatureBase_IsChild(const CThingCreatureBase* self) {
    return self->data->lifeStage == 1;
}
int main() {
    CTCBData d0; d0.lifeStage = 1;
    CTCBData d1; d1.lifeStage = 2;
    CThingCreatureBase a; a.data = &d0;
    CThingCreatureBase b; b.data = &d1;
    bool ok = true;
    if (CThingCreatureBase_IsChild(&a) != true) ok = false;
    if (CThingCreatureBase_IsChild(&b) != false) ok = false;
    d0.lifeStage = 0;
    if (CThingCreatureBase_IsChild(&a) != false) ok = false;
    if (ok) { std::printf("CThingCreatureBase_00661f70_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}