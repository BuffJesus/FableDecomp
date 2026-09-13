// The included proposal fragment is the production candidate, not a reimplementation.
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
namespace sol {
struct proxy {
    const std::map<std::string,float>& values;
    std::string key;
    template<class T> T get() const { return static_cast<T>(values.at(key)); }
};
struct table {
    std::map<std::string,float> values;
    proxy operator[](const char* key) const { return {values,key}; }
};
}
struct C3DVector { float x,y,z; };
enum EScriptEntityMoveType { WALK=0, RUN=1 };
struct CCharString { std::string value; };
static int liveStrings=0;
struct FableString {
    CCharString text;
    explicit FableString(const char* value):text{value} { ++liveStrings; }
    ~FableString() { --liveStrings; }
    const CCharString* get() const { return &text; }
};
struct Expert { void* pVTable; int identity; };
struct CScriptGameResourceObjectScriptedThingBaseVTable {
    void(*MoveToPosition)(Expert*,const C3DVector*,float,EScriptEntityMoveType,bool,bool);
    void(*PlayAnimation)(Expert*,const CCharString*,bool,bool,bool,bool,bool,bool,bool);
    void(*ClearCommands)(Expert*);
    void(*ClearAllActions)(Expert*);
};
struct Entry { struct { struct { Expert* Data; } pImp; } resource; bool live; };
struct Scope {
    enum class Kind { Resource };
    std::vector<Entry> entries;
    bool open=true;
    Entry& Get(unsigned id,Kind) {
        if (!open || !id || id>entries.size() || !entries[id-1].live)
            throw std::runtime_error("closed resource");
        return entries[id-1];
    }
#include "retail_resource_actions.inc"
};
static std::vector<std::string> events;
static C3DVector position;
static float radius;
static int moveType,identity;
static std::array<bool,7> flags;
static void move(Expert* e,const C3DVector* p,float r,EScriptEntityMoveType t,bool a,bool b) {
    events.push_back("move"); identity=e->identity; position=*p; radius=r; moveType=t; flags[0]=a; flags[1]=b;
}
static void animation(Expert* e,const CCharString* name,bool a,bool b,bool c,bool d,bool f,bool g,bool h) {
    assert(liveStrings==1); events.push_back(name->value); identity=e->identity; flags={a,b,c,d,f,g,h};
}
static void clear(Expert* e) { events.push_back("commands"); identity=e->identity; }
static void clearAll(Expert* e) { events.push_back("actions"); identity=e->identity; }
template<class F> void throws(F f) { bool caught=false; try { f(); } catch(const std::exception&) { caught=true; } assert(caught); }
int main() {
    CScriptGameResourceObjectScriptedThingBaseVTable table{move,animation,clear,clearAll};
    Expert first{&table,11},second{&table,22};
    Scope scope{{{{{&first}},true},{{{&second}},true},{{{nullptr}},true}}};
    sol::table pos{{{"x",17.5f},{"y",-4.0f},{"z",8.25f}}};
    for(unsigned id: {1u,2u}) {
        for(int mask=0;mask<4;++mask) {
            scope.MoveToPosition(id,pos,0.25f,RUN,mask&1,mask&2);
            assert(identity==int(id)*11 && position.x==17.5f && position.y==-4 && position.z==8.25f);
            assert(radius==0.25f && moveType==RUN && flags[0]==bool(mask&1) && flags[1]==bool(mask&2));
        }
        for(int mask=0;mask<128;++mask) {
            scope.PlayAnimation(id,"ANIMATION",mask&1,mask&2,mask&4,mask&8,mask&16,mask&32,mask&64);
            assert(identity==int(id)*11 && liveStrings==0);
            for(int bit=0;bit<7;++bit) assert(flags[bit]==bool(mask&(1<<bit)));
        }
        scope.ClearAllActions(id); assert(events.back()=="actions" && identity==int(id)*11);
        scope.ClearCommands(id); assert(events.back()=="commands" && identity==int(id)*11);
    }
    auto count=events.size();
    scope.MoveToPosition(3,{},0,WALK,false,true);
    scope.PlayAnimation(3,"unused",false,false,false,false,false,false,false);
    scope.ClearCommands(3); scope.ClearAllActions(3);
    assert(events.size()==count && liveStrings==0);
    throws([&]{scope.MoveToPosition(1,{},0,WALK,false,true);});
    table.MoveToPosition=nullptr; throws([&]{scope.MoveToPosition(1,pos,0,WALK,false,true);});
    table.PlayAnimation=nullptr; throws([&]{scope.PlayAnimation(1,"x",false,false,false,false,false,false,false);});
    table.ClearCommands=nullptr; throws([&]{scope.ClearCommands(1);});
    table.ClearAllActions=nullptr; throws([&]{scope.ClearAllActions(1);});
    scope.entries[0].live=false; throws([&]{scope.ClearCommands(1);});
    scope.open=false; throws([&]{scope.ClearCommands(3);});
    assert(events.size()==count && liveStrings==0);
}
