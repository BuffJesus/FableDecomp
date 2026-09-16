#include <retail_wife_argument_key.h>
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <iostream>

static std::map<const CCharString*,std::string> live;
static std::vector<std::string> events;
static const CCharString* queried=nullptr;
static bool textExists=true;
static bool failConcat=false;
static CGameScriptInterfaceBase* game=reinterpret_cast<CGameScriptInterfaceBase*>(0x123400);
static CScriptThing* wife=reinterpret_cast<CScriptThing*>(0x234500);
static CScriptThing* husband=reinterpret_cast<CScriptThing*>(0x345600);
static CCharString* __fastcall Number(CCharString* result,int value) {
    assert(!live.count(result));live[result]=std::to_string(value);events.push_back("number");return result;
}
static void __fastcall Literal(CCharString* result,void*,const char* value,int length) {
    assert(length==-1&&!live.count(result));live[result]=value;events.push_back("prefix");
}
static CCharString* __fastcall Concat(CCharString* result,const CCharString* left,const CCharString* right) {
    assert(live.count(left)&&live.count(right)&&!live.count(result));
    if(failConcat)throw std::runtime_error("concat failure");
    live[result]=live.at(left)+live.at(right);events.push_back("concat");return result;
}
static void __fastcall Destroy(CCharString* value,void*) {
    assert(live.count(value));events.push_back("destroy:"+live.at(value));live.erase(value);
}
static CCharString* __fastcall Assign(CCharString* value,void*,const char* text) {
    assert(live.count(value));live[value]=text;events.push_back("assign");return value;
}
static bool __fastcall Exists(CGameScriptInterfaceBase* actual,void*,const CCharString* key) {
    assert(actual==game&&live.count(key));queried=key;events.push_back("exists");return textExists;
}
static void __fastcall Line(CGameScriptInterfaceBase* actual,void*,int id,const CCharString* key,bool flag,
                            const CScriptThing* speaker,const CScriptThing* listener) {
    assert(actual==game&&id==-7&&key==queried&&!flag&&speaker==wife&&listener==husband);
    assert(live.count(key));events.push_back("line:"+live.at(key));
}
int main() {
    WifeArgumentKeyAPIs api{Number,reinterpret_cast<tCCharString_Constructor_Literal>(Literal),Concat,
        reinterpret_cast<tCCharString_Destructor>(Destroy),reinterpret_cast<tCCharString_AssignmentLiteral>(Assign),
        reinterpret_cast<tTextEntryExists>(Exists),reinterpret_cast<tAddLineToConversation>(Line)};
    for(bool exists:{false,true}) {
        events.clear();textExists=exists;
        RetailWifeArgumentKey key(game,50,api);
        assert(live.size()==1);
        assert((std::vector<std::string>(events.begin(),events.end())==std::vector<std::string>{
            "number","prefix","concat","destroy:TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_","destroy:50"}));
        assert(key.Exists()==exists);
        if(!exists)key.ResetToFirst();
        key.AddLine(-7,wife,husband);
        assert(events.back()==std::string("line:TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_")+(exists?"50":"10"));
        key.Close();key.Close();assert(live.empty());
        bool rejected=false;try{key.Exists();}catch(const std::runtime_error&){rejected=true;}assert(rejected);
    }
    events.clear();failConcat=true;
    bool failed=false;try{RetailWifeArgumentKey key(game,10,api);}catch(const std::runtime_error&){failed=true;}
    assert(failed&&live.empty());
    assert(events[events.size()-2]=="destroy:TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_"&&events.back()=="destroy:10");
    failConcat=false;
    try{RetailWifeArgumentKey key(game,20,api);throw std::runtime_error("callback failure");}catch(const std::runtime_error&){}
    assert(live.empty());
    std::cout<<"PASS: native-order key construction, temporary destruction, live text lookup, shared key identity, reset, closed-scope rejection and error cleanup\n";
    return 0;
}
