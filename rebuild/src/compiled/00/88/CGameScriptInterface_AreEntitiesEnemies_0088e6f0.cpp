
class CThing;
class CTCEnemy {
public:
    static bool __fastcall AreThingsEnemies(CThing* thing1, CThing* thing2);
};
struct CScriptThingVTbl {
    unsigned char pad_0000[0x2C];
    CThing* (__fastcall* GetThing)(const void* self);
    unsigned char pad_0030[0xFC];
    bool (__fastcall* HasThing)(const void* self);
};
struct CScriptThing { const CScriptThingVTbl* vftable; };
class CGameScriptInterface {
public:
    bool AreEntitiesEnemies(const CScriptThing& entity1, const CScriptThing& entity2) const;
};
bool CGameScriptInterface::AreEntitiesEnemies(const CScriptThing& entity1, const CScriptThing& entity2) const
{
    if (entity1.vftable->HasThing(&entity1)) {
        if (entity2.vftable->HasThing(&entity2)) {
            CThing* t2 = entity2.vftable->GetThing(&entity2);
            CThing* t1 = entity1.vftable->GetThing(&entity1);
            return CTCEnemy::AreThingsEnemies(t1, t2);
        }
    }
    return false;
}