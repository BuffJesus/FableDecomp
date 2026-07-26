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