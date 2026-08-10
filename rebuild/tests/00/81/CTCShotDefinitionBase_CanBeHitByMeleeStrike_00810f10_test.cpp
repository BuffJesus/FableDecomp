#include <cstdio>
struct Inner {
    char pad[0x1b];
    bool flag;
};
struct CTCShotDefinitionBase {
    char pad0[8];
    Inner* f8;
};
bool __fastcall CanBeHitByMeleeStrike(CTCShotDefinitionBase* self);
int main() {
    Inner inner;
    inner.flag = true;
    CTCShotDefinitionBase obj;
    obj.f8 = &inner;
    bool r = CanBeHitByMeleeStrike(&obj);
    inner.flag = false;
    bool r2 = CanBeHitByMeleeStrike(&obj);
    if (r && !r2) printf("OK_00810f10\n");
    else printf("FAIL %d %d\n", (int)r, (int)r2);
    return 0;
}