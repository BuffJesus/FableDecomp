"""Check the exact added quest method bodies with actual FSE types and Lua."""
from tools.script_recovery.prepare_oakvale_quest_capabilities import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run

if __name__=='__main__':
    print(run(harness='oakvale_quest_adapter_harness.cpp',output_name='oakvale_quest_adapter_checks',
        prepare_fn=prepare,stage=OUTPUT,
        native_scope='BookTrader facing/conversation witnesses and IEEE float state transport',
        limits='Exact method bodies with FSE types and Lua, thin quest-state receiver and integer-store double; full DLL compiled separately. Persistence ordering and live gameplay remain pending.')['testOutput'])
