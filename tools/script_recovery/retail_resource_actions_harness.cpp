// The included proposal fragment is the production candidate, not a reimplementation.
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
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
struct CScriptThing { int identity; void* pVTable = nullptr; };
enum EHeroAbility { TEST_ABILITY = 14 };
struct CScriptThingVTable {
    bool(*MsgIsHitBy)(CScriptThing*,const CCharString*);
    bool(*MsgIsHitByAnySpecialAbilityFrom)(CScriptThing*,const CCharString*);
    bool(*MsgIsHitBySpecialAbilityFrom)(CScriptThing*,EHeroAbility,const CCharString*);
};
typedef unsigned long DWORD;
#ifndef __fastcall
#define __fastcall
#endif
// Stub for the runtime's ASLR<T>(absolute) relocation helper: the harness resolves
// only the Thing distance helper, and returns null for any other address.
static bool __fastcall distanceStub(CScriptThing*,const C3DVector*,float);
static bool distanceAvailable=true;
static volatile unsigned char animationArgument=0;
static bool animationArgumentAvailable=true;
template<class T> T ASLR(DWORD address) {
    if constexpr (std::is_same_v<T,const volatile unsigned char*>) {
        return address==0x01375748 && animationArgumentAvailable ? &animationArgument : nullptr;
    } else {
        return address==0x00CBE45C && distanceAvailable ? reinterpret_cast<T>(&distanceStub) : T(nullptr);
    }
}
struct CScriptGameResourceObjectScriptedThingBaseVTable {
    void(*MoveToPosition)(Expert*,const C3DVector*,float,EScriptEntityMoveType,bool,bool);
    void(*PlayAnimation)(Expert*,const CCharString*,bool,bool,bool,bool,bool,bool,bool);
    void(*ClearCommands)(Expert*);
    void(*ClearAllActions)(Expert*);
};
struct Entry { struct { struct { Expert* Data; } pImp; } resource; bool live; CScriptThing thing; };
struct Scope {
    enum class Kind { Resource, Thing };
    std::vector<Entry> entries;
    bool open=true;
    std::vector<Kind> kinds;
    void CheckOpen() { if (!open) throw std::runtime_error("closed scope"); }
    Entry& Get(unsigned id,Kind kind) {
        if (!open || !id || id>entries.size() || !entries[id-1].live)
            throw std::runtime_error("closed resource");
        if (id<=kinds.size() && kinds[id-1]!=kind) throw std::runtime_error("wrong resource kind");
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
static float distanceLimit;
static bool hitOrdinary, hitAny, hitExcluded;
static std::vector<int> hitOrder;
static bool ordinaryHit(CScriptThing*,const CCharString* name) {
    assert(liveStrings==1 && name->value=="SCRIPT_NAME_HERO"); hitOrder.push_back(1); return hitOrdinary;
}
static bool anyHit(CScriptThing*,const CCharString* name) {
    assert(liveStrings==2 && name->value=="SCRIPT_NAME_HERO"); hitOrder.push_back(2); return hitAny;
}
static bool excludedHit(CScriptThing*,EHeroAbility ability,const CCharString* name) {
    assert(liveStrings==3 && ability==TEST_ABILITY && name->value=="SCRIPT_NAME_HERO");
    hitOrder.push_back(3); return hitExcluded;
}
static bool __fastcall distanceStub(CScriptThing* thing,const C3DVector* p,float limit) {
    events.push_back("distance"); identity=thing->identity; position=*p; distanceLimit=limit;
    return limit<1.0f;
}
template<class F> void throws(F f) { bool caught=false; try { f(); } catch(const std::exception&) { caught=true; } assert(caught); }
int main() {
    CScriptGameResourceObjectScriptedThingBaseVTable table{move,animation,clear,clearAll};
    Expert first{&table,11},second{&table,22};
    Scope scope;
    CScriptThingVTable hitTable{ordinaryHit,anyHit,excludedHit};
    CScriptThing actor{42,&hitTable};
    for (unsigned bits=0; bits<8; ++bits) {
        hitOrdinary=bits&1; hitAny=bits&2; hitExcluded=bits&4; hitOrder.clear();
        assert(scope.IsHitByHeroExceptAbility(&actor,14)==(hitOrdinary || (hitAny && !hitExcluded)));
        const auto expected=hitOrdinary ? std::vector<int>{1} : hitAny ? std::vector<int>{1,2,3} : std::vector<int>{1,2};
        assert(hitOrder==expected && liveStrings==0);
    }
    throws([&]{scope.IsHitByHeroExceptAbility(nullptr,14);});
    for (unsigned value: {0u, 1u, 2u, 255u, 0u}) {
        animationArgument=static_cast<unsigned char>(value);
        assert(scope.ReadAnimationArgument5()==(value!=0));
    }
    animationArgumentAvailable=false;
    throws([&]{scope.ReadAnimationArgument5();});
    animationArgumentAvailable=true;
    scope.entries={{{{&first}},true,{0}},{{{&second}},true,{0}},{{{nullptr}},true,{0}},{{{nullptr}},true,{44}}};
    scope.kinds={Scope::Kind::Resource,Scope::Kind::Resource,Scope::Kind::Resource,Scope::Kind::Thing};
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
    // The Thing query forwards the Thing entry, exact coordinates and limit, and returns the native answer.
    assert(!scope.ThingIsDistanceFromPositionOver(4,pos,2.0f) && identity==44 && distanceLimit==2.0f);
    assert(position.x==17.5f && position.y==-4 && position.z==8.25f && events.back()=="distance");
    assert(scope.ThingIsDistanceFromPositionOver(4,pos,0.5f) && distanceLimit==0.5f);
    throws([&]{scope.ThingIsDistanceFromPositionOver(1,pos,2.0f);});   // a resource is not a Thing
    throws([&]{scope.ThingIsDistanceFromPositionOver(4,{},2.0f);});    // missing coordinate is an error
    distanceAvailable=false; throws([&]{scope.ThingIsDistanceFromPositionOver(4,pos,2.0f);}); distanceAvailable=true;
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
    scope.entries[3].live=false; throws([&]{scope.ThingIsDistanceFromPositionOver(4,pos,2.0f);});
    scope.open=false; throws([&]{scope.ClearCommands(3);});
    throws([&]{scope.ReadAnimationArgument5();});
    assert(events.size()==count && liveStrings==0);
}
