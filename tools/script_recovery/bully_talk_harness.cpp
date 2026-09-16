#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0;
static CScriptThing bully{},heroThing{};
static CScriptThingVTable bullyTable{};
static CCPPointerInfo heroInfo{};
static bool talked,hasTeddy,nullHero;
static int failure,heroQueries;
static std::vector<std::string> events;
static bool __fastcall talk(CScriptThing* actor,void*,const CCharString* key){
    check(actor==&bully && strings.size()==1 && strings.at(const_cast<CCharString*>(key))=="SCRIPT_NAME_HERO");
    events.push_back("talk");if(failure==1)throw std::runtime_error("TALK");return talked;
}
static CScriptThing* __fastcall hero(CGameScriptInterfaceBase*,void*){
    check(strings.size()==2);++heroQueries;events.push_back("hero");return nullHero?nullptr:&heroThing;
}
static bool __fastcall possession(CGameScriptInterfaceBase*,void*,const CCharString* key,const CScriptThing* holder){
    check(strings.size()==2 && holder==(nullHero?nullptr:&heroThing));
    check(strings.at(const_cast<CCharString*>(key))=="OBJECT_TEDDY_BEAR_UNGIVEABLE");
    events.push_back("possession");if(failure==2)throw std::runtime_error("POSSESSION");return hasTeddy;
}
static void __fastcall trackedCtor(CCharString* self,void*,const char* text,int length){
    events.push_back(std::string("new:")+text);stringCtor(self,nullptr,text,length);
}
static void __fastcall trackedDtor(CCharString* self,void*){
    events.push_back("destroy:"+strings.at(self));stringDtor(self,nullptr);
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&hero);
tIsObjectInThingsPossession IsObjectInThingsPossession_API=reinterpret_cast<tIsObjectInThingsPossession>(&possession);
class BullyTalkHost {public:
    CGameScriptInterfaceBase* m_game=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void CheckOpen(){}
#include "bully_talk_predicate.inc"
};
int main(){try{
    bullyTable.MsgIsTalkedToBy=reinterpret_cast<tCScriptThing_MsgIsTalkedToBy>(&talk);
    bully.pVTable=reinterpret_cast<void**>(&bullyTable);bully.pImp.Data=nullptr;
    heroThing.pImp.Info=&heroInfo;heroInfo.RefCount=9;
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&trackedCtor);
    CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&trackedDtor);
    int cases=0;
    for(bool t:{false,true})for(bool item:{false,true})for(bool empty:{false,true})for(int error=0;error<3;++error){
        talked=t;hasTeddy=item;nullHero=empty;failure=error;heroQueries=0;events.clear();
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        auto type=lua.new_usertype<BullyTalkHost>("BullyPredicateHost",sol::no_constructor);
        type["BullyTalkedWithTeddy"]=&BullyTalkHost::BullyTalkedWithTeddy;
        type["WithRetailResources"]=[](BullyTalkHost& host,sol::protected_function cb){WithRetailResources(host.m_game,cb);};
        BullyTalkHost host;lua["host"]=&host;lua["me"]=&bully;
        auto result=lua.safe_script(R"(
            host:WithRetailResources(function(resources)
                local control=resources:NewResource()
                resources:PrepareResource(control)
                assert(resources:TryAcquire(control,me,4))
                answer=host:BullyTalkedWithTeddy(me)
            end)
        )",sol::script_pass_on_error);
        bool shouldError=error==1 || (error==2 && t);
        check(result.valid()!=shouldError && strings.empty() && locals.empty() && heroInfo.RefCount==9);
        std::vector<std::string> expected={"new:SCRIPT_NAME_HERO","talk"};
        if(t && error!=1)expected.insert(expected.end(),{"new:OBJECT_TEDDY_BEAR_UNGIVEABLE","hero","possession","destroy:OBJECT_TEDDY_BEAR_UNGIVEABLE"});
        expected.push_back("destroy:SCRIPT_NAME_HERO");check(events==expected);
        check(heroQueries==int(t && error!=1));
        if(shouldError){sol::error e=result;check(std::string(e.what()).find(error==1?"TALK":"POSSESSION")!=std::string::npos);}
        else check(lua["answer"].get<bool>()==(t&&item));
        ++cases;
    }
    std::cout<<"Bully talk x86: "<<cases<<" CString/raw-Hero Lua policies passed\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
