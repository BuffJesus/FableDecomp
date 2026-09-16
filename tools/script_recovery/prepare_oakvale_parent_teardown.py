"""Close entity VMs while their shared quest state and timers still exist."""
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose

OUTPUT=ROOT/'work/oakvale_parent_teardown_integration'


def once(source,before,after):
    if source.count(before)!=1:raise ValueError('Parent teardown insertion changed: '+before[:70])
    return source.replace(before,after)


def edit(bodies):
    header=bodies['LuaQuestHost.h'].decode()
    header=once(header,'    NativeQuestLifetime m_nativeLifetime;',
        '    bool m_closing=false;\n    NativeQuestLifetime m_nativeLifetime;')
    header=once(header,'    const std::string& GetScriptName() const;',
        '    bool IsClosing() const { return m_closing; }\n    const std::string& GetScriptName() const;')
    bodies['LuaQuestHost.h']=header.encode()
    host=bodies['LuaQuestHost.cpp'].decode()
    host=once(host,'LuaQuestHost::~LuaQuestHost() {',
        'LuaQuestHost::~LuaQuestHost() {\n    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro)m_closing=true;')
    host=once(host,'        // VM finalizers may borrow quest state: release the VM while it is live.',
        '        LuaManager::GetInstance().UnregisterQuestEntityScripts(m_pQuestState.get());\n'
        '        // VM finalizers may borrow quest state: release the VM while it is live.')
    for signature in ('void LuaQuestHost::RegisterMain() {','void LuaQuestHost::CreateThread(const std::string& luaFunctionName, const std::string& regionName, const std::vector<sol::object>& args) {'):
        host=once(host,signature,signature+'\n    if(m_closing)throw std::runtime_error("Quest is closing; thread registration rejected");')
    for signature in ('void LuaQuestHost::Main() {','template<int N> void LuaQuestHost::ThreadRunner() {'):
        host=once(host,signature,signature+'\n    if(m_closing)return;')
    bodies['LuaQuestHost.cpp']=host.encode()
    state=bodies['LuaQuestState.cpp'].decode()
    anchor='if(m_pParentHost && m_pParentHost->GetNativeLifetime()==NativeQuestLifetime::NewOakValeIntro) {'
    if state.count(anchor)!=2:raise ValueError('Native frame overload gates changed')
    state=state.replace(anchor,anchor+'\n        if(m_pParentHost->IsClosing())throw std::runtime_error("Quest is closing; frame advance rejected");')
    bodies['LuaQuestState.cpp']=state.encode()
    manager_header=bodies['LuaManager.h'].decode()
    manager_header=once(manager_header,'    void UnregisterEntityScriptData(LuaEntityHost* pHost);',
        '    void UnregisterEntityScriptData(LuaEntityHost* pHost);\n    void UnregisterQuestEntityScripts(LuaQuestState* quest);')
    bodies['LuaManager.h']=manager_header.encode()
    manager=bodies['LuaManager.cpp'].decode()
    manager=once(manager,'#include "LuaManager.h"','#include "LuaManager.h"\n#include "LuaQuestHost.h"')
    manager=once(manager,'void LuaManager::RegisterEntityScriptData(LuaEntityHost* pHost, const std::string& scriptName, LuaQuestState* pQuestState) {',
        'void LuaManager::RegisterEntityScriptData(LuaEntityHost* pHost, const std::string& scriptName, LuaQuestState* pQuestState) {\n'
        '    if(pHost && pHost->m_pParentHost && pHost->m_pParentHost->IsClosing())throw std::runtime_error("Quest is closing; entity VM registration rejected");')
    helper='''void LuaManager::UnregisterQuestEntityScripts(LuaQuestState* quest) {
    if(!quest)return;
    for(;;) {
        auto found=m_entityScriptDataMap.begin();
        while(found!=m_entityScriptDataMap.end() && found->second.pQuestState!=quest)++found;
        if(found==m_entityScriptDataMap.end())break;
        LuaEntityHost* entity=found->first;
        {
            // Remove the registry entry before finalizers run, preventing a
            // reentrant unregister from destroying the same VM twice.
            auto owned=m_entityScriptDataMap.extract(found);
            // Detach before closing: a finalizer may cause the engine to release
            // the entity host. Do not touch that host again after Lua closes.
            entity->m_pParentHost=nullptr;entity->pInterface=nullptr;
            EntityScriptData& data=owned.mapped();
            if(data.pLuaState)quest->AbandonMovieSequence(data.pLuaState->lua_state());
        }
    }
}

'''
    manager=once(manager,'void LuaManager::UnregisterEntityScriptData(LuaEntityHost* pHost) {',helper+'void LuaManager::UnregisterEntityScriptData(LuaEntityHost* pHost) {')
    bodies['LuaManager.cpp']=manager.encode()
    entity=bodies['LuaEntityHost.cpp'].decode()
    entity=once(entity,'void LuaEntityHost::Main() {','void LuaEntityHost::Main() {\n    if(!m_pParentHost)return;')
    entity=once(entity,'    return &this->m_pParentHost->base;',
        '    return m_pParentHost ? &m_pParentHost->base : nullptr;')
    bodies['LuaEntityHost.cpp']=entity.encode()
    return dict(status='opt-in quest closing guard and parent-driven entity VM teardown',
        order=['mark closing','release thread anchors','close matching entity VMs','close quest VM','close quest state','native base destruction'],
        limits='Engine owns entity-host allocation and scheduler quiescence. This stage closes VM/state borrowing before parent state destruction; live engine callback ordering remains pending.')


def prepare():
    return compose(ROOT/'work/oakvale_entity_frame_integration',OUTPUT,edit,'oakvale-parent-teardown.patch',
        extra_sources={'LuaManager.h':Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaManager.h')})


if __name__=='__main__':print(prepare()['candidateSha256'])
