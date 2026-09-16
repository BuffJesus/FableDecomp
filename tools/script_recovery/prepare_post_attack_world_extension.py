"""Compose PostAttackStuff atomic world adapters over the timer integration."""
import json
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose

OUTPUT=ROOT/'work/post_attack_world_integration'


def prepare():
    return compose(base=ROOT/'work/start_barrel_timer_integration',output=OUTPUT,
        methods=('PostAttackStartIsAlive','TeleportToPostAttackStart',
                 'SetPostAttackVillageLimbo','PostAttackHeroNearTrigger'),
        fragments=('retail_post_attack_world.inc',),patch_name='post-attack-world-integration.patch')


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
