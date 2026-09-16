"""Compile the combined resource registrations including StartBarrelTimer."""
import json

from tools.script_recovery.prepare_start_barrel_timer_extension import OUTPUT, prepare
from tools.script_recovery.run_post_attack_registration_compile import run


if __name__ == '__main__':
    print(json.dumps(run(prepare_fn=prepare, out=OUTPUT), indent=2))
