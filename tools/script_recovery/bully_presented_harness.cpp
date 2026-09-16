#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0;
static CCharString* outputBuffer;
static CScriptThing actor{};
static CScriptThingVTable actorTable{};
static int created,closed,pollCount,mode;
static CCharString* __fastcall outputCtor(CCharString* self,void*) {
    check(!outputBuffer);outputBuffer=self;self->pStringData=nullptr;strings[self]="";++created;return self;
}
static bool __fastcall notEqual(CCharString* self,void*,const char* name){check(self==outputBuffer);return strings.at(self)!=name;}
static void __fastcall outputDtor(CCharString* self,void*) {
    if(self==outputBuffer){check(!movies && !paused);++closed;outputBuffer=nullptr;}
    stringDtor(self,nullptr);
}
static bool __fastcall pollOutput(CScriptThing* source,void*,CCharString* out){
    check(source==&actor && out==outputBuffer);++pollCount;
    if(pollCount==1){check(strings.at(out).empty());strings[out]="OBJECT_TEDDY_BEAR_UNGIVEABLE";out->pStringData=reinterpret_cast<void*>(1);return false;}
    check(strings.at(out)=="OBJECT_TEDDY_BEAR_UNGIVEABLE");
    if(mode==2)throw std::runtime_error("POLL");
    strings[out]="";out->pStringData=nullptr;return true;
}
static void jump(unsigned char* at,void* target){at[0]=0xe9;*reinterpret_cast<int*>(at+1)=reinterpret_cast<unsigned char*>(target)-(at+5);}
int main(){try{
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,4096,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));check(arena!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(arena)-(0x99e000-0x400000);
    jump(arena+0x4b0,reinterpret_cast<void*>(&outputCtor));jump(arena+0x960,reinterpret_cast<void*>(&notEqual));
    actorTable.MsgIsPresentedWithItem=reinterpret_cast<tCScriptThing_MsgIsPresentedWithItem>(&pollOutput);
    actor.pVTable=reinterpret_cast<void**>(&actorTable);actor.pImp.Data=nullptr;
    CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&outputDtor);
    for(mode=0;mode<4;++mode){
        created=closed=pollCount=0;sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        lua["me"]=&actor;lua["mode"]=mode;
        auto result=lua.safe_script(R"(
            scope(function(resources)
                savedResources=resources
                local control=resources:NewResource()
                resources:PrepareResource(control)
                assert(resources:TryAcquire(control,me,4))
                local item=resources:NewPresentedItemOutput(me)
                savedItem=item
                assert(resources:PollPresentedItem(item)==false)
                assert(resources:PresentedItemMatches(item,'OBJECT_TEDDY_BEAR_UNGIVEABLE'))
                if mode==1 then return end -- cancellation leaves populated output
                local movie=resources:StartMovie('')
                resources:Pause(true)
                assert(resources:PollPresentedItem(item)==true) -- true+empty must survive
                assert(not resources:PresentedItemMatches(item,'OBJECT_TEDDY_BEAR_UNGIVEABLE'))
                if mode==3 then error('BODY') end
                resources:Pause(false)
                resources:DestroyMovie(movie)
                resources:DestroyPresentedItemOutput(item)
                assert(not pcall(function() resources:PollPresentedItem(item) end))
            end)
            assert(not pcall(function() savedResources:PollPresentedItem(savedItem) end))
        )",sol::script_pass_on_error);
        check(result.valid()==(mode<2));
        if(mode>=2){sol::error e=result;check(std::string(e.what()).find(mode==2?"POLL":"BODY")!=std::string::npos);}
        check(created==1 && closed==1 && !outputBuffer && strings.empty() && locals.empty() && !movies && !paused);
    }
    VirtualFree(arena,0,MEM_RELEASE);
    std::cout<<"Bully presented x86: 4 retained-output/movie/error Lua policies passed\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
