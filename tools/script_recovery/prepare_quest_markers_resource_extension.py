"""Compose retained-Thing marker methods over the verified post-attack stage."""
import json
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose


def prepare():
    return compose(base=ROOT/'work/post_attack_resource_integration',
        output=ROOT/'work/quest_markers_resource_integration',
        methods=('AddCoreQuestMarker','RemoveCoreQuestMarker'),
        fragments=('manage_quest_core_markers_methods.inc',),patch_name='quest-markers-resource-integration.patch')


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
