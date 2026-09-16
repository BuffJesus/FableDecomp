"""Compose timer HUD methods over the common opt-in timer owner stage."""
import json

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose

OUTPUT = ROOT/'work/start_barrel_timer_integration'


def prepare():
    return compose(base=ROOT/'work/oakvale_timer_integration', output=OUTPUT,
        methods=('ReadBarrelWatchTimer', 'AddBarrelTimerBar', 'IsHeroNearBarrelGuard',
                 'ColourBarrelTimer', 'UpdateBarrelTimer', 'RemoveBarrelTimer'),
        fragments=('start_barrel_timer_methods.inc',),
        patch_name='start-barrel-timer-integration.patch')


if __name__ == '__main__':
    print(json.dumps(prepare(), indent=2))
