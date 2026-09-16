#include <cstring>

// Native parent coordinates are 32-bit floats. Preserve their bits through the
// existing namespaced integer store instead of truncating coordinates to ints.
void LuaQuestState::SetStateFloat(const std::string& key,float value) {
    static_assert(sizeof(float)==sizeof(int),"Retail state requires 32-bit float/int");
    int bits;std::memcpy(&bits,&value,sizeof(bits));SetStateInt(key,bits);
}
float LuaQuestState::GetStateFloat(const std::string& key) {
    const int bits=GetStateInt(key);float value;std::memcpy(&value,&bits,sizeof(value));return value;
}

void LuaQuestState::FaceThingByScriptName(CScriptThing* actor,const std::string& name,bool snap) {
    if(!m_pGameInterface || !GetThingWithScriptName_ByName_API || !EntitySetFacingAngleTowardsThing_API ||
       !CCharString_Construct_Literal || !CCharString_Destroy || !RetailThing_Destroy_API)
        throw std::runtime_error("Named facing APIs unavailable");
    CCharString key{};CScriptThing target{};bool keyLive=false,targetLive=false;
    try {
        CCharString_Construct_Literal(&key,name.c_str(),-1);keyLive=true;
        auto* returned=GetThingWithScriptName_ByName_API(m_pGameInterface,&target,&key);targetLive=true;
        // BookTrader DB4215/DB44E9/DB4CB4 use EAX, not the owned output address.
        EntitySetFacingAngleTowardsThing_API(m_pGameInterface,actor,returned,snap);
        targetLive=false;RetailThing_Destroy_API(&target);
        keyLive=false;CCharString_Destroy(&key);
    }catch(...){
        if(targetLive){targetLive=false;try{RetailThing_Destroy_API(&target);}catch(...) {}}
        if(keyLive){keyLive=false;try{CCharString_Destroy(&key);}catch(...) {}}
        throw;
    }
}
