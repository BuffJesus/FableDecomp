"""Drain native process callbacks while quest and entity Lua VMs remain alive."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose
from tools.script_recovery.prepare_oakvale_parent_teardown import once
from tools.script_recovery.native_oakvale_quiescence import prove

OUTPUT=ROOT/'work/oakvale_quiescence_integration'


def edit(bodies):
    evidence=prove()
    host=bodies['LuaQuestHost.cpp'].decode()
    host=once(host,'    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro)m_closing=true;',
        '''    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro) {
        m_closing=true;
        // Native process destruction can resume suspended callbacks. Drain them
        // before unanchoring Lua stacks or closing child/parent VMs and state.
        using TerminateProcesses=void(__thiscall*)(CScriptBase_Retail*);
        ASLR<TerminateProcesses>(0x00CB7F60)(&this->base);
    }''')
    bodies['LuaQuestHost.cpp']=host.encode()
    return dict(status='native process termination precedes Lua cleanup',native=evidence,
        limits='Original traversal and Windows fiber host checks; real-game callback ordering remains unverified.')


def prepare():
    return compose(ROOT/'work/oakvale_entity_ownership_integration',OUTPUT,edit,'oakvale-quiescence.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
