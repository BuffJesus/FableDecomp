"""Stage New Oakvale's empty Main section and native virtual Main thunk."""
import hashlib
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.compose_runtime_files import compose

OUTPUT=ROOT/'work/oakvale_main_registration_integration'


def prove():
    data=RData()
    regions=((0xdaace0,139,'862fa6b4c28044cbdad0deb7782abb0c6a832b10954c1aa1be40732d4a024b69'),
             (0xcdd440,5,'81794790c0ad77391e995d91903df71d532fdf23777b0a32cef0c4606e844a6f'))
    for address,size,digest in regions:
        if hashlib.sha256(data.bytes_at(address,size)).hexdigest()!=digest:raise ValueError('Native Main registration changed')
    if data.string_at(0x12c2f94)!='Main' or data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('Native Main registration names changed')
    return regions


def edit(bodies):
    regions=prove()
    source=bodies['LuaQuestHost.cpp'].decode();old='void LuaQuestHost::RegisterMain() { AutoRegisterMain(&this->base, GetMemberFunctionAddress(&LuaQuestHost::Main)); }'
    if source.count(old)!=1:raise ValueError('Host Main registration changed')
    new='''void LuaQuestHost::RegisterMain() {
    if(m_nativeLifetime!=NativeQuestLifetime::NewOakValeIntro) {
        AutoRegisterMain(&this->base, GetMemberFunctionAddress(&LuaQuestHost::Main));
        return;
    }
    if(!CCharString_Construct_Literal || !CCharString_Destroy || !CSpawnedFunc_Construct || !AddSpawnedFunction_func)
        throw std::runtime_error("Oakvale Main registration APIs unavailable");
    using Allocate=void*(__cdecl*)(size_t);
    using Free=void(__cdecl*)(void*);
    using Destroy=void(__thiscall*)(CSpawnedFunc*);
    auto* function=static_cast<CSpawnedFunc*>(ASLR<Allocate>(0x00BFEA1A)(sizeof(CSpawnedFunc)));
    // Native's downstream +24 access requires a non-null allocation.
    if(!function)throw std::bad_alloc();
    CCharString name{},section{};bool nameLive=false,sectionLive=false,constructed=false,handedOff=false;
    try {
        CCharString_Construct_Literal(&name,"Main",-1);nameLive=true;
        CSpawnedFunc_Construct(function,&name,0);constructed=true;
        function->pVTable=ASLR<void**>(0x012D7A3C);
        function->pThunkToMain=ASLR<void*>(0x00CDD440);
        function->pOwnerScript=&this->base;
        CCharString_Construct_Literal(&section,"",-1);sectionLive=true;
        // Keep ownership with the engine once registration begins: it may link
        // the object before a later engine operation fails.
        handedOff=true;AddSpawnedFunction_func(&this->base,function,&section);
        sectionLive=false;CCharString_Destroy(&section);
        nameLive=false;CCharString_Destroy(&name);
    }catch(...) {
        if(sectionLive){sectionLive=false;try{CCharString_Destroy(&section);}catch(...) {}}
        if(nameLive){nameLive=false;try{CCharString_Destroy(&name);}catch(...) {}}
        if(!handedOff){
            if(constructed){try{ASLR<Destroy>(0x00CDD4C0)(function);}catch(...) {}}
            try{ASLR<Free>(0x00BFE9BC)(function);}catch(...) {}
        }
        throw;
    }
}'''
    bodies['LuaQuestHost.cpp']=source.replace(old,new).encode()
    return dict(native=regions,mainName='Main',section='',nativeMainThunk='0x00CDD440',
        policy='NewOakValeIntro only',limits='Engine registration takes ownership at call entry. Full scheduler and spawned CreateThread allocation policies remain separate gates.')


def prepare():return compose(ROOT/'work/oakvale_progress_integration',OUTPUT,edit,'oakvale-main-registration.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
