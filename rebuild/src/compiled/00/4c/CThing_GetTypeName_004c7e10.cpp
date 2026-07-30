struct CThingTypeEntry { int a,b,c,d; char name[4]; };   // 0x14 bytes
struct CThingTypeManager { int pad; CThingTypeEntry *entries; };
extern CThingTypeManager *g_thingTypeManager;
struct CThing { char pad[0x10]; unsigned char typeIndex; };
char * __fastcall GetTypeName(CThing *self)
{
    unsigned int idx = self->typeIndex;
    return g_thingTypeManager->entries[idx].name;
}