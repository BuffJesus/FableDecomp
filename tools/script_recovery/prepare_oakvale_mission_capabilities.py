"""Compose mission ownership scopes over the verified lifecycle/deed stage."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose

OUTPUT=ROOT/'work/oakvale_mission_integration'


def prepare():
    return compose(base=ROOT/'work/oakvale_deed_integration',output=OUTPUT,
        methods=('TurnOakvaleHeroIntoChild','PrepareOakvaleHouseAndStartScreen',
                 'SetOakvaleHeroKillable','FinishOakvaleActiveQuest'),
        fragments=('retail_oakvale_mission.inc',),patch_name='oakvale-mission-integration.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
