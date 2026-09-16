"""Compile atomic post-attack adapters in the common staged resource owner."""
import json
from tools.script_recovery.prepare_post_attack_world_extension import prepare,OUTPUT
from tools.script_recovery.run_post_attack_scope_runtime_checks import run


if __name__=='__main__':
    print(json.dumps(run(harness='post_attack_world_runtime_harness.cpp',
        output_name='post_attack_world_runtime_checks',prepare_fn=prepare,stage=OUTPUT,
        native_scope='test_native_post_attack_world: original lookup/action/cleanup scopes',
        limits='Actual staged owner and real Lua with engine doubles. Full dispatcher/movie, DLL, scheduler and gameplay remain separate gates.'),indent=2))
