"""Compose only proved Bully phases; keep incomplete Main absent and disabled."""
import hashlib
import json
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_initial_phases import recover as initial
from tools.script_recovery.bully_retained_phases import recover as retained
from tools.script_recovery.bully_initial_candidate import correspondence
from tools.script_recovery.bully_initial_candidate import DRAFT
from tools.script_recovery.bully_control_flow import recover_flow,RETRY_PHASE
from tools.script_recovery.bully_predicate_masks import recover_masks

def generate():
    first,initial_evidence=initial();second,lifetime=retained()
    source=first+'\n'+second+'\n'+RETRY_PHASE
    LuaRuntime().execute('return function()\n'+source+'\nend')
    out=ROOT/'work/bully_converter/scoped_candidate';out.mkdir(parents=True,exist_ok=True)
    (out/'BULLY_PHASES.lua').write_text(source)
    (out/'quests.lua').write_text('-- Disabled native recovery phase candidate.\nreturn {}\n')
    draft,flow=recover_flow(DRAFT.read_text());draft,masks=recover_masks(draft)
    draft=draft.replace('function Main(quest, me)','function UnfinishedBullyMain(quest, me, resources)',1)
    draft='-- DISABLED unfinished full-body draft: scopes not wired; not an actor entrypoint.\n'+draft
    LuaRuntime().execute('return function()\n'+draft+'\nend')
    (out/'UNFINISHED_BODY.lua').write_text(draft)
    report={'enabled':False,'gameplayComplete':False,'fullMainComposed':False,
        'sourceSha256':hashlib.sha256(source.encode()).hexdigest(),'initialEvidence':initial_evidence,
        'lifetimeInventory':lifetime,'guardedUnappliedDraftEdits':correspondence(),
        'bodyFlowRecovery':flow,'bodyPredicateRecovery':masks,
        'entry':'RunBullyRecoveredEntry(quest, me, continuation)',
        'integrationGates':[
            'Caller continuation begins at native DBB588 after retained Victim post-query. It must retain self control and Victim throughout the interaction loop.',
            'Late WithBullyRunoffControls starts DBC8FA only after self-control reacquisition and final query. Its continuation enters actor-map construction DBC9EE.',
            'TryAcquireThing(resourceId, thingId, priority) needs an atomic resource-scope binding; never pass a numeric Thing handle to raw TryAcquire.',
            'WithBullyPausedMovie represents all five verified empty-key movie scopes; caller must place dialogue/macro/state actions and early exits at their native locations.',
            'UNFINISHED_BODY has restored termination/retry loops and scoped predicates, but its resource/movie wrappers are not yet replaced by phase calls. Do not invoke it as Main.',
            'Presented-item output, further missing actor operands and omitted movie cleanup joins still prevent full-body composition.',
            'Actor maps, string maps, macro setup flags and native controlled actor consumers require separate operand proof before composing the runoff body.',
            'All resource/movement/distance capabilities remain unapplied; callback-error fallback is host policy, not native exception or manager teardown proof.']}
    (out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return report

if __name__=='__main__':print(generate()['entry'])
