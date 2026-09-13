// Actual PersistTransferBool body and staged OnPersist; storage API doubled.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#include "LuaQuestState.h"

std::string GetLogFilePath() { return {}; }
LuaQuestState::LuaQuestState(LuaQuestHost* parent,CGameScriptInterfaceBase* game)
    : m_pParentHost(parent),m_pGameInterface(game) {}
static std::map<std::string,bool> stateValues,storage;
static std::vector<std::string> transfers;
static bool reading=false;
static void* context=reinterpret_cast<void*>(0x123456);
static void __fastcall transfer(CPersistContext* ctx,void*,const char* name,bool* value,const bool* fallback) {
    check(ctx==context && value && fallback && !*fallback);
    transfers.push_back(name);
    if (reading) {
        auto found=storage.find(name);
        *value=found==storage.end()?*fallback:found->second;
    } else storage[name]=*value;
}
tCPersistContext_Transfer_bool CPersistContext_Transfer_bool_API=reinterpret_cast<tCPersistContext_Transfer_bool>(&transfer);
#include "persist-method.inc"
void LuaQuestState::Log(const std::string&) {}
#include "binding-method.inc"
static unsigned bindingsVerified=0;
struct ExpectedBinding {std::string name,path;int flags;};
static std::vector<ExpectedBinding> expectedBindings={{"GTDI_Maze","GuardianTrophyDealerInfo/Entities/GTDI_Maze",0}};
void LuaQuestState::FinalizeEntityBindings() {
    // Engine allocation is doubled; inspect the real method's queued operands.
    check(m_pendingBindings.size()==expectedBindings.size());
    for(unsigned i=0;i<expectedBindings.size();++i){
        const auto& binding=m_pendingBindings[i];const auto& expected=expectedBindings[i];
        if(binding.entityName!=expected.name || binding.scriptFile!=expected.path || binding.flags!=expected.flags)
            throw std::runtime_error("Port binding has wrong name/path/flags: "+binding.entityName+" / "+binding.scriptFile);
    }
    m_pendingBindings.clear();++bindingsVerified;
}

int main(int argc,char** argv) {
    try {
        check(argc==2);
        sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::package,sol::lib::table);
        auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
#include "persist-registration.inc"
#include "binding-registration.inc"
        questState_type["FinalizeEntityBindings"]=&LuaQuestState::FinalizeEntityBindings;
        questState_type["GetStateBool"]=[](LuaQuestState&,const std::string& key){return stateValues[key];};
        questState_type["SetStateBool"]=[](LuaQuestState&,const std::string& key,bool value){stateValues[key]=value;};
        LuaQuestState quest(nullptr,nullptr);lua["quest"]=&quest;lua["context"]=context;
        lua["expect_bindings"]=[](sol::table rows){
            expectedBindings.clear();
            for(unsigned i=1;i<=rows.size();++i){sol::table row=rows[i];
                expectedBindings.push_back({row.get<std::string>(1),row.get<std::string>(2),row.get<int>(3)});}
        };
        unsigned cases=0;
        lua["reset_case"]=[](bool a,bool b,bool read,sol::table saved){
            stateValues={{"SwordTaken",a},{"BookRead",b}};reading=read;storage.clear();transfers.clear();
            for(auto kv:saved)storage[kv.first.as<std::string>()]=kv.second.as<bool>();
        };
        lua["verify_case"]=[&](bool a,bool b,sol::table expected){
            check(stateValues.at("SwordTaken")==a && stateValues.at("BookRead")==b);
            check(transfers==std::vector<std::string>{"SwordTaken","BookRead"});
            std::map<std::string,bool> wanted;
            for(auto kv:expected)wanted[kv.first.as<std::string>()]=kv.second.as<bool>();
            check(storage==wanted);++cases;
        };
        unsigned guardianCases=0;
        lua["reset_guardian"]=[](bool initial,bool read,sol::table saved){
            stateValues={{"PieceOver",initial}};reading=read;storage.clear();transfers.clear();
            for(auto kv:saved)storage[kv.first.as<std::string>()]=kv.second.as<bool>();
        };
        lua["verify_guardian"]=[&](bool final,sol::table expected){
            check(stateValues.at("PieceOver")==final);
            check(transfers==std::vector<std::string>{"PieceOver"});
            std::map<std::string,bool> wanted;
            for(auto kv:expected)wanted[kv.first.as<std::string>()]=kv.second.as<bool>();
            check(storage==wanted);++guardianCases;
        };
        lua.script_file(argv[1]);check(cases==24 && guardianCases==8 && bindingsVerified==3);
        std::cout << "PASS: 32 native persistence comparisons and 3 packaged roots through actual AddEntityBinding\n";
        return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
