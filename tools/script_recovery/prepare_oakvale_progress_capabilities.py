"""Compose native gold/attack objective string scopes."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose

OUTPUT=ROOT/'work/oakvale_progress_integration'


def prepare():
    return compose(base=ROOT/'work/oakvale_mission_integration',output=OUTPUT,
        methods=('SetOakvaleProgressObjective',),fragments=('retail_oakvale_progress.inc',),
        patch_name='oakvale-progress-integration.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
