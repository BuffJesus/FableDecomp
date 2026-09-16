#include "LuaRetailResources.h"
#include "retail_villager_speech_lists.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
DWORD g_fableBase=0;
struct Data { std::string text;unsigned refs; };
static std::set<Data*> strings;
static std::set<void*> buffers;
static std::vector<std::string> destroyed;
static bool recording=false;
static void check(bool ok){if(!ok)throw std::runtime_error("speech-list check failed");}
static Data* value(const CCharString* s){return reinterpret_cast<Data*>(s->pStringData);}
static void set(CCharString* s,Data* data){s->pStringData=reinterpret_cast<decltype(s->pStringData)>(data);}
static void __fastcall literal(CCharString* out,void*,const char* text,int length){
    check(length==-1);auto* data=new Data{text,1};strings.insert(data);set(out,data);
}
static void __fastcall destroy(CCharString* out,void*){
    auto* data=value(out);if(!data)return;check(strings.count(data)==1 && data->refs>0);
    if(recording)destroyed.push_back(data->text);
    if(--data->refs==0){strings.erase(data);delete data;}set(out,nullptr);
}
static CCharString* __fastcall copy(CCharString* out,void*,const CCharString* source){
    auto* data=value(source);check(!data || strings.count(data)==1);set(out,data);if(data)++data->refs;return out;
}
static CCharString* __fastcall assign(CCharString* out,void*,const CCharString* source){
    if(out!=source){destroy(out,nullptr);copy(out,nullptr,source);}return out;
}
static void __cdecl release(void* memory){check(buffers.erase(memory)==1);std::free(memory);}
struct Vector {CCharString* begin;CCharString* end;CCharString* capacity;};
static void __fastcall insert(Vector* v,void*,CCharString* position,const CCharString* source,void*,unsigned count,bool append){
    check(position==v->end && v->end==v->capacity && count==1 && append);
    auto size=(reinterpret_cast<uintptr_t>(v->end)-reinterpret_cast<uintptr_t>(v->begin))/4;
    auto capacity=size+(size?size:1);
    auto* next=static_cast<CCharString*>(std::calloc(capacity,sizeof(CCharString)));check(next!=nullptr);buffers.insert(next);
    for(unsigned i=0;i<size;++i)copy(next+i,nullptr,v->begin+i);
    copy(next+size,nullptr,source);
    for(auto* item=v->begin;item!=v->end;++item)destroy(item,nullptr);
    if(v->begin)release(v->begin);
    v->begin=next;v->end=next+size+1;v->capacity=next+capacity;
}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&literal);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
struct TextScope {
    enum class Kind {Text};
    struct Entry {CCharString text{};} entry;
    Entry& Get(unsigned id,Kind){if(id!=7)throw std::runtime_error("HANDLE");return entry;}
#include "retail_villager_speech_list_actions.inc"
};
static void jump(DWORD address,void* target){auto* code=ASLR<unsigned char*>(address);code[0]=0xE9;
    DWORD delta=reinterpret_cast<DWORD>(target)-(reinterpret_cast<DWORD>(code)+5);std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);}
int main(){try{
    auto* memory=VirtualAlloc(nullptr,0x800000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);check(memory!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(memory);
    jump(0x99EC30,reinterpret_cast<void*>(&copy));jump(0x99EFB0,reinterpret_cast<void*>(&assign));
    jump(0x433530,reinterpret_cast<void*>(&insert));jump(0xBFEA14,reinterpret_cast<void*>(&release));
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<RetailVillagerSpeechLists>("Lists",sol::no_constructor);
    type["Append"]=&RetailVillagerSpeechLists::Append;type["Count"]=&RetailVillagerSpeechLists::Count;
    type["Close"]=&RetailVillagerSpeechLists::Close;
    auto scopeType=lua.new_usertype<TextScope>("TextScope",sol::no_constructor);
    scopeType["AssignVillagerSpeechText"]=&TextScope::AssignVillagerSpeechText;
    int cases=0;
    for(int repeats:{1,2,5}){
        RetailVillagerSpeechLists lists;TextScope scope;auto& selected=scope.entry.text;lua["lists"]=&lists;lua["resources"]=&scope;
        lua.script("function select(category,male,index) resources:AssignVillagerSpeechText(7,lists,category,male,index) end");
        lua.set_function("selected",[&](){return value(&selected)->text;});
        std::map<std::pair<std::string,bool>,std::vector<std::string>> expected;
        for(int repeat=0;repeat<repeats;++repeat)for(bool male:{false,true})for(const char* category:{"none","good","both","bad"})for(int index=0;index<6;++index){
            std::string text=std::string(category)+(male?"_MALE_":"_FEMALE_")+std::to_string(repeat*6+index);
            lua["category"]=category;lua["male"]=male;lua["text"]=text;lua["size"]=repeat*6+index+1;
            lua.script("lists:Append(category,male,text);assert(lists:Count(category,male)==size)");
            expected[{category,male}].push_back(text);
            // Retain an entry, then force subsequent growth while that text is held.
            lua.script("select(category,male,0);assert(selected()==category..(male and '_MALE_0' or '_FEMALE_0'))");
        }
        lua.script("local ok=pcall(select,'good',true,-1);assert(not ok)");
        lua.script("local ok=pcall(function() resources:AssignVillagerSpeechText(99,lists,'good',true,0) end);assert(not ok);ok=pcall(function() resources:AssignVillagerSpeechText(7,nil,'good',true,0) end);assert(not ok)");
        recording=true;destroyed.clear();lists.Close();recording=false;
        std::vector<std::string> order;
        for(bool male:{false,true})for(const char* category:{"none","both","bad","good"}){
            auto& entries=expected[{category,male}];order.insert(order.end(),entries.begin(),entries.end());
        }
        check(destroyed==order && buffers.empty() && strings.size()==1 && value(&selected)->refs==1);
        lua.script("assert(selected()=='bad_MALE_0');local ok=pcall(function() lists:Count('good',true) end);assert(not ok);lists:Close()");
        destroy(&selected,nullptr);check(strings.empty());++cases;
    }
    std::shared_ptr<RetailVillagerSpeechLists> borrowed;
    {
        RetailVillagerSpeechListOwner questOwner;
        borrowed=questOwner.Get();check(borrowed==questOwner.Get());
        borrowed->Append("good",true,"shared");
        check(questOwner.Get()->Count("good",true)==1);
    }
    check(strings.empty() && buffers.empty());
    bool rejected=false;try {borrowed->Count("good",true);}catch(const std::runtime_error&){rejected=true;}
    check(rejected);borrowed.reset();++cases;
    std::cout<<"PASS: "<<cases<<" repeated-init/growth/retained-selection/quest-owner teardown scenarios through actual FSE types and Lua\n";
    VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
