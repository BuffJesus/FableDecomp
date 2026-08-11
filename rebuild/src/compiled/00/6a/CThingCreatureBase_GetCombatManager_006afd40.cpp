struct HelperResult {
    char pad[0x20];
    void* f20;
};

struct CThingCreatureBase;

// helper at 0x686a80: __fastcall(this) -> HelperResult*
extern HelperResult* __fastcall helper_686a80(CThingCreatureBase* self);

void* __fastcall GetCombatManager(CThingCreatureBase* self)
{
    return helper_686a80(self)->f20;
}