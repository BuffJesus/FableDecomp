"""Generate unapplied registry -> allocator -> state timer-owner integration."""
import difflib,hashlib,json,shutil
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT

def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender'),base=ROOT/'work/post_attack_resource_integration',out=ROOT/'work/oakvale_timer_host_proposal'):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    source=Path(__file__).parent;before={};report={'status':'unapplied; loader/owner proposal','inputs':{}}
    def read(name):
        p=base/name if (base/name).exists() else runtime/name
        report['inputs'][str(p)]=hashlib.sha256(p.read_bytes()).hexdigest();return p.read_text()
    def replace(s,old,new):
        if s.count(old)!=1:raise ValueError('Timer integration anchor changed: '+old[:100])
        return s.replace(old,new)
    for p in base.glob('*.h'):shutil.copyfile(p,out/p.name)
    for name in ('retail_oakvale_timers.h','oakvale_timer_host_policy.h'):shutil.copyfile(source/name,out/name)
    for name in ('dllmain.cpp','LuaQuestHost.h','LuaQuestHost.cpp','LuaQuestState.h','LuaQuestState.cpp'):
        s=read(name);before[name]=s
        if name=='dllmain.cpp':
            s=replace(s,'#include "LuaQuestHost.h"','#include "LuaQuestHost.h"\n#include "oakvale_timer_host_policy.h"')
            s=replace(s,'std::vector<std::string> g_questScriptFileNames;','std::vector<NativeQuestSlot> g_questScriptFileNames;')
            s=replace(s,'std::vector<EntityScriptDefinition> entityScripts; };','std::vector<EntityScriptDefinition> entityScripts; NativeQuestLifetime nativeLifetime=NativeQuestLifetime::None; };')
            s=replace(s,'const std::string& scriptName = g_questScriptFileNames[N];','const auto& slot = g_questScriptFileNames[N];\n    const std::string& scriptName = slot.file;')
            s=replace(s,'new LuaQuestHost(pData, pInterface, scriptName)','new LuaQuestHost(pData, pInterface, scriptName, slot.lifetime)')
            s=replace(s,'g_questScriptFileNames.push_back(file);','g_questScriptFileNames.push_back({file, ParseNativeQuestLifetime(entry["nativeLifetime"])});')
            s=replace(s,'*questName, *questFile, *questId\n                });','*questName, *questFile, *questId\n                });\n            g_questDefinitions.back().nativeLifetime = ParseNativeQuestLifetime(qd["nativeLifetime"]);')
            s=replace(s,'g_questScriptFileNames.push_back(q.file);','g_questScriptFileNames.push_back({q.file, q.nativeLifetime});')
        elif name=='LuaQuestHost.h':
            s=replace(s,'#pragma once','#pragma once\n#include "oakvale_timer_host_policy.h"')
            s=replace(s,'const std::string& scriptName);','const std::string& scriptName, NativeQuestLifetime nativeLifetime=NativeQuestLifetime::None);')
        elif name=='LuaQuestHost.cpp':
            s=replace(s,'const std::string& scriptName)\n','const std::string& scriptName, NativeQuestLifetime nativeLifetime)\n')
            s=replace(s,'m_pQuestState = std::make_unique<LuaQuestState>(this, interface);','m_pQuestState = std::make_unique<LuaQuestState>(this, interface);\n    m_pQuestState->ConfigureNativeLifetime(nativeLifetime);')
        elif name=='LuaQuestState.h':
            s=replace(s,'#pragma once','#pragma once\n#include "oakvale_timer_host_policy.h"')
            s=replace(s,'    CGameScriptInterfaceBase* GetGameInterface()', '    void ConfigureNativeLifetime(NativeQuestLifetime policy) { m_nativeTimers.Configure(policy); }\n    CGameScriptInterfaceBase* GetGameInterface()')
            s=replace(s,'    std::string GetNamespacedKey(const std::string& key);','    std::string GetNamespacedKey(const std::string& key);\n    // Declared last: timers close before maps, speech lists and state base members.\n    NativeQuestTimerOwner m_nativeTimers;')
        else:
            s=replace(s,'void LuaQuestState::SetStateInt(const std::string& key, int value) {','void LuaQuestState::SetStateInt(const std::string& key, int value) {\n    m_nativeTimers.RejectOwnedWrite(key);')
            s=replace(s,'int LuaQuestState::GetStateInt(const std::string& key) {','int LuaQuestState::GetStateInt(const std::string& key) {\n    int nativeValue;\n    if(m_nativeTimers.Read(key,nativeValue))return nativeValue;')
        (out/name).write_text(s)
    patch=''.join(''.join(difflib.unified_diff(old.splitlines(True),(out/name).read_text().splitlines(True),fromfile='a/'+name,tofile='b/'+name)) for name,old in before.items())
    (out/'oakvale-timer-host.patch').write_text(patch)
    for name in ('retail_oakvale_timers.h','oakvale_timer_host_policy.h'):report['inputs'][str(source/name)]=hashlib.sha256((source/name).read_bytes()).hexdigest()
    report['limits']=['No runtime changes applied.','Owner initialization precedes Lua Init; state member order closes timers before speech lists.','Engine thread quiescence, host Lua VM/state destruction order, and save-restore timing remain explicit integration gates.','Unknown metadata rejects configuration; existing absent-metadata ports retain their old persistent timer behavior.']
    (out/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return out,report
if __name__=='__main__':print(prepare()[0])
