#include <cstdio>
struct CDazedDef {
    char pad[0x84];
    int dazedThreshold; /* +0x84 */
};
struct CThingCreatureBase {
    char pad[0xe0];
    CDazedDef* pDef; /* +0xe0 */
};
float __fastcall CThingCreatureBase_GetDazedHealthThreshold(CThingCreatureBase* self)
{
    return (float)self->pDef->dazedThreshold;
}
int main()
{
    CDazedDef def;
    def.dazedThreshold = 42;
    CThingCreatureBase obj;
    obj.pDef = &def;
    float r = CThingCreatureBase_GetDazedHealthThreshold(&obj);
    if (r == 42.0f) {
        std::printf("CThingCreatureBase_00662090_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}