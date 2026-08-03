
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

#include <cstdio>
static int g_hasA=1,g_hasB=1;
static void* g_thingA=(void*)0x1111,*g_thingB=(void*)0x2222;
static int g_getA=0,g_getB=0;
static void* g_eA=0,*g_eB=0; static bool g_res=true;
static CThing* __fastcall GA(const void*){g_getA++;return (CThing*)g_thingA;}
static CThing* __fastcall GB(const void*){g_getB++;return (CThing*)g_thingB;}
static bool __fastcall HA(const void*){return g_hasA!=0;}
static bool __fastcall HB(const void*){return g_hasB!=0;}
bool __fastcall CTCEnemy::AreThingsEnemies(CThing* a,CThing* b){g_eA=(void*)a;g_eB=(void*)b;return g_res;}
static CScriptThingVTbl vA,vB; static CScriptThing oA,oB;
static void setup(){vA.GetThing=GA;vA.HasThing=HA;vB.GetThing=GB;vB.HasThing=HB;oA.vftable=&vA;oB.vftable=&vB;}
int main(){
    setup(); CGameScriptInterface g;
    g_hasA=1;g_hasB=1;g_res=true;g_eA=g_eB=0;
    if(!g.AreEntitiesEnemies(oA,oB)){printf("FAIL r1\n");return 1;}
    if(g_eA!=g_thingA||g_eB!=g_thingB){printf("FAIL order\n");return 1;}
    g_res=false; if(g.AreEntitiesEnemies(oA,oB)){printf("FAIL r2\n");return 1;}
    g_hasA=0;g_hasB=1;g_getA=g_getB=0;g_res=true;
    if(g.AreEntitiesEnemies(oA,oB)){printf("FAIL r3\n");return 1;}
    if(g_getA||g_getB){printf("FAIL e1\n");return 1;}
    g_hasA=1;g_hasB=0;g_getA=g_getB=0;
    if(g.AreEntitiesEnemies(oA,oB)){printf("FAIL r4\n");return 1;}
    if(g_getA||g_getB){printf("FAIL e2\n");return 1;}
    printf("ENEMIES_OK\n"); return 0;
}