#include <cstdio>
static int g_base_called = 0;
struct CDragonActionBaseDef {
    unsigned char _pad_0x0[0x28];
    void Copy(const CDragonActionBaseDef* other);
};
struct CDragonActionHoverDef : public CDragonActionBaseDef {
    int field28;
};
void CDragonActionBaseDef::Copy(const CDragonActionBaseDef* other)
{
    g_base_called = 1;
    (void)other;
}
void __fastcall CDragonActionHoverDef_Copy(CDragonActionHoverDef* self, void* edx, const CDragonActionHoverDef* other)
{
    self->CDragonActionBaseDef::Copy(other);
    self->field28 = other->field28;
}
int main()
{
    CDragonActionHoverDef a, b;
    b.field28 = 0x1234;
    a.field28 = 0;
    CDragonActionHoverDef_Copy(&a, 0, &b);
    if (g_base_called == 1 && a.field28 == 0x1234) {
        std::printf("CDragonActionHoverDef_0045406a_TEST PASS\n");
        return 0;
    }
    std::printf("CDragonActionHoverDef_0045406a_TEST FAIL\n");
    return 1;
}