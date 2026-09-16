"""Stage live SCRIPT_DEF morality and the scoped good-deed objective."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose

OUTPUT=ROOT/'work/oakvale_deed_integration'


def prepare():
    return compose(base=ROOT/'work/oakvale_lifecycle_integration',output=OUTPUT,
        methods=('ApplyOakvaleDeedMorality','SetOakvaleDeedObjective'),
        fragments=('retail_oakvale_deeds.inc',),patch_name='oakvale-deed-integration.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
