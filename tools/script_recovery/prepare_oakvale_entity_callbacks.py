"""Use native engine predicate timing for opt-in converter entity hosts."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose
from tools.script_recovery.prepare_oakvale_parent_teardown import once
from tools.script_recovery.native_oakvale_entity_callbacks import prove

OUTPUT=ROOT/'work/oakvale_entity_callbacks_integration'


def edit(bodies):
    evidence=prove();source=bodies['LuaEntityHost.cpp'].decode()
    source=once(source,'        if (!IsBoundThingAlive()) {',
        '''        // Native CActiveEntityScriptBase forwards its predicate failure
        // through vtable slot 5; Main must not infer an earlier extra callback.
        if (m_pParentHost->GetNativeLifetime()!=NativeQuestLifetime::NewOakValeIntro && !IsBoundThingAlive()) {''')
    source=once(source,'    if (m_predicateFailDispatched) return;',
        '''    const bool native=m_pParentHost && m_pParentHost->GetNativeLifetime()==NativeQuestLifetime::NewOakValeIntro;
    if (m_predicateFailDispatched && !native) return;''')
    bodies['LuaEntityHost.cpp']=source.encode()
    return dict(status='native entity callback timing; legacy inferred callback retained',native=evidence,
        limits='Native bridges and actual-host callback dispatch checked separately; no gameplay claim.')


def prepare():
    return compose(ROOT/'work/oakvale_quiescence_integration',OUTPUT,edit,'oakvale-entity-callbacks.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
