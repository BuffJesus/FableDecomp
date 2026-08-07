struct Sub; extern void __fastcall gtm_sub(Sub*);
struct CThingManager; extern CThingManager* g_thingmgr;
struct S { char pad[8]; };
CThingManager* __fastcall GetThingManager(S* self)
{
    gtm_sub(reinterpret_cast<Sub*>(reinterpret_cast<char*>(self) + 8));
    return g_thingmgr;
}