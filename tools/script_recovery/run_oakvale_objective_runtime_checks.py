"""Exercise the atomic startup objective adapter with actual FSE types and Lua."""
import json
from tools.script_recovery.run_post_attack_scope_runtime_checks import run as run_scope


def run():
    return run_scope(harness='oakvale_objective_runtime_harness.cpp',
        output_name='oakvale_objective_runtime_checks',
        native_scope='test_native_oakvale_objective executes original startup caller and active-name getter; this run checks the compiled adapter.',
        limits='Actual staged resource class and Lua; objective APIs doubled. Constructor/getter failure is before successful output construction; arbitrary native SEH/throwing destructors and full quest engine execution are not established.')


if __name__=='__main__':print(json.dumps(run(),indent=2))
