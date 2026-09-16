void InitializeAffairManActor(CScriptThing* actor) {
    CheckOpen();
    if(!actor || !g_pCScriptThingVTable || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
       !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetIsPushableByHero_API ||
       !EntitySetAsUseMovementInActions_API || !SetThingHasInformation_API)
        throw std::runtime_error("AffairMan Init API unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    CScriptThing copy=*actor;copy.pVTable=g_pCScriptThingVTable;
    if(copy.pImp.Info) ++copy.pImp.Info->RefCount;
    SetIsPushableByHero_API(m_game,copy,false); // Callee consumes copy.
    EntitySetAsUseMovementInActions_API(m_game,actor,false);
    SetThingHasInformation_API(m_game,actor,false,false,false);
}
void PlayAffairManAnimation(unsigned control,const std::string& name,bool second) {
    auto& e=Get(control,Kind::Resource);
    struct Key {
        CCharString value{};
        explicit Key(const char* text){CCharString_Construct_Literal(&value,text,-1);}
        ~Key(){CCharString_Destroy(&value);}
    } key(name.c_str());
    const unsigned char raw=*ASLR<const volatile unsigned char*>(0x1375748);
    auto* expert=e.resource.pImp.Data;
    if(!expert) return;
    auto* table=reinterpret_cast<void**>(expert->pVTable);
    if(!table || !table[0x48/4]) throw std::runtime_error("AffairMan animation slot unavailable");
    using RawAnimation=void(__thiscall*)(void*,const CCharString*,bool,bool,bool,bool,unsigned char,bool,bool);
    reinterpret_cast<RawAnimation>(table[0x48/4])(expert,&key.value,false,second,false,true,raw,false,false);
}
