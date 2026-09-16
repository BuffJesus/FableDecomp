"""Compile all registrations in the common stage including quest markers."""
import json
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_quest_markers_resource_extension import prepare
from tools.script_recovery.run_post_attack_registration_compile import run as compile_registration


def run():
    return compile_registration(prepare_fn=prepare,out=ROOT/'work/quest_markers_resource_integration')


if __name__=='__main__':print(json.dumps(run(),indent=2))
