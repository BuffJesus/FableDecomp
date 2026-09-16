"""Exercise actual composed entity resource storage through real Lua."""
from tools.script_recovery.prepare_oakvale_entity_capabilities import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run

if __name__=='__main__':
    report=run(harness='oakvale_entity_storage_harness.cpp',output_name='oakvale_entity_storage_checks',
        prepare_fn=prepare,stage=OUTPUT,native_scope='Storage adaptation of independently recovered entity methods',
        limits='Common actual owner and Lua handle/storage semantics with engine doubles; full entity gameplay remains unverified.')
    print(report['testOutput'])
