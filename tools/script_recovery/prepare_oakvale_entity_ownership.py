"""Retain each native entity's Thing and return a single owning script reference."""
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose
from tools.script_recovery.prepare_oakvale_parent_teardown import once
from tools.script_recovery.native_oakvale_entity_ownership import prove

OUTPUT=ROOT/'work/oakvale_entity_ownership_integration'


def edit(bodies):
    evidence=prove()
    header=bodies['LuaEntityHost.h'].decode()
    header=once(header,'private:',
        'private:\n    bool m_ownsNativeThing=false;\n    void ReleaseNativeThing() noexcept;')
    bodies['LuaEntityHost.h']=header.encode()
    source=bodies['LuaEntityHost.cpp'].decode()
    before='    memcpy(&this->m_Me, thing, sizeof(CScriptThing));'
    after='''    if(pParentQuest->GetNativeLifetime()==NativeQuestLifetime::NewOakValeIntro) {
        if(!thing)throw std::runtime_error("Native entity Thing is unavailable");
        using Copy=CScriptThing*(__thiscall*)(CScriptThing*,const CScriptThing*);
        ASLR<Copy>(0x004ABE90)(&m_Me,thing);m_ownsNativeThing=true;
    }else memcpy(&this->m_Me,thing,sizeof(CScriptThing));'''
    source=once(source,before,after)
    source=once(source,'    LuaManager::GetInstance().RegisterEntityScriptData(this, scriptName, pParentQuest->GetQuestState());',
        '    try { LuaManager::GetInstance().RegisterEntityScriptData(this,scriptName,pParentQuest->GetQuestState()); }\n'
        '    catch(...) { ReleaseNativeThing();throw; }')
    unregister='    LuaManager::GetInstance().UnregisterEntityScriptData(this);'
    if source.count(unregister)!=2:raise ValueError('Entity VM teardown paths changed')
    source=source.replace(unregister,unregister+'\n    ReleaseNativeThing();')
    source+='''
void LuaEntityHost::ReleaseNativeThing() noexcept {
    if(!m_ownsNativeThing)return;
    m_ownsNativeThing=false;
    using Destroy=void(__thiscall*)(CScriptThing*);
    try { ASLR<Destroy>(0x004AA840)(&m_Me); }catch(...) {}
}
'''
    bodies['LuaEntityHost.cpp']=source.encode()
    dll=bodies['dllmain.cpp'].decode()
    anchor='    LuaQuestHost* pParentHost = reinterpret_cast<LuaQuestHost*>(parent);'
    replacement=anchor+'''
    if(pParentHost && pParentHost->GetNativeLifetime()==NativeQuestLifetime::NewOakValeIntro) {
        if(!a1 || !thing)throw std::runtime_error("Native entity allocator output/Thing unavailable");
        auto* result=static_cast<DWORD*>(a1);result[0]=result[1]=0;
        auto entity=std::make_unique<LuaEntityHost>(pdata,pParentHost,thing,scriptName);
        using Allocate=void*(__cdecl*)(size_t);
        auto* management=static_cast<DWORD*>(ASLR<Allocate>(0x00BFEA1A)(0x0c));
        if(!management)throw std::bad_alloc();
        management[0]=1;management[1]=reinterpret_cast<DWORD>(&CustomEntityHostDeleter);
        management[2]=reinterpret_cast<DWORD>(entity.get());
        result[0]=reinterpret_cast<DWORD>(entity.release());result[1]=reinterpret_cast<DWORD>(management);
        return a1;
    }
'''
    dll=once(dll,anchor,replacement);bodies['dllmain.cpp']=dll.encode()
    return dict(status='native entity Thing retention and single-reference script ownership',
        native=evidence,
        evidence=['Barrel allocator DB7D00 retains Thing via 4ABE90 and initializes script refcount to one',
                  'Base entity destruction F35B40 releases retained Thing; 4AA840 is the shared Thing destructor'],
        limits='Opt-in parent policy. Exact caller/copy/destructor checks and compiled allocator tests are separate; engine scheduling remains pending.')


def prepare():
    return compose(ROOT/'work/oakvale_parent_teardown_integration',OUTPUT,edit,'oakvale-entity-ownership.patch',
        extra_sources={'LuaEntityHost.h':Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaEntityHost.h')})


if __name__=='__main__':print(prepare()['candidateSha256'])
