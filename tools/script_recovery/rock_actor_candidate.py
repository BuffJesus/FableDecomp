"""Separate disabled complete actor Main composition; base modules stay unchanged."""
import hashlib
import json
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_state_candidate import generate as base_generate
from tools.script_recovery.rock_final_targeting import recover
from tools.script_recovery.rock_lifecycle import recover as recover_lifecycle

MAIN='''function Main(quest, me)
    WithRockTrollSelfPhase(quest, me, function(resources, seh_me)
        WithRockTrollRewardsPhase(quest, me, function()
            WithRockTrollExhumationPhase(quest, me, resources, seh_me, function()
                WithRockTrollAnimationPhase(quest, resources, seh_me, function()
                    RockTrollFinalTargetingPhase(quest, me)
                end)
            end)
        end)
    end)
end
'''


def generate(output=None):
    output=Path(output or ROOT/'work/rock_trigger_converter/actor_candidate').resolve()
    if not output.is_relative_to((ROOT/'work/rock_trigger_converter').resolve()):raise ValueError('Isolated actor output required')
    report=base_generate(output);targeting,evidence=recover();lifecycle,lifecycle_evidence=recover_lifecycle()
    phases=(output/'RockTrollFirstEncounter/Phases/RTFE_RockTroll.resources.lua').read_text()
    text='-- Disabled actor Main; host capabilities and entity lifecycle review remain pending.\n'+phases+'\n'+targeting+'\n'+lifecycle+'\n'+MAIN
    LuaRuntime().execute('return function()\n'+text+'\nend')
    name='RockTrollFirstEncounter/Entities/RTFE_RockTroll.lua';(output/name).write_text(text)
    report['functions']+=['RTFE_RockTroll.Main','RTFE_RockTroll.Init','RTFE_RockTroll.OnPredicateFail']
    report['remainingMainFunctions']=[]
    report['missingFunctions']=[]
    report['native']['entityLifecycle']=lifecycle_evidence
    report['requiredCapabilities'].append('InitializeRockTrollMarker(boundThing)')
    report['files'][name]=hashlib.sha256((output/name).read_bytes()).hexdigest()
    report['native']['finalTargeting']=evidence
    report['requiredCapabilities'].append('TargetRockTrollAtHero(boundThing)')
    report['runtimeGaps']=[
        'InitializeRockTrollMarker adapter has a separate actual-FSE x86/Lua gate and remains unapplied; existing persistence wrapper filters null Data. Required host capabilities remain uninstalled; copied-Thing scopes and retained-thread scheduling need implementation/validation.',
        'TargetRockTrollAtHero preserves two fresh raw GetHero outputs, including null; current generic targeting wrappers skip null and accept shared-pointer actors.',
        'Animation consumer proposal remains uncompiled; reward and movie/map gates are compiled but unapplied.',
        'PersistTransferIntDefault must preserve native zero default; shared state/restore ordering require host review.',
        'Native entry timing, alive-condition clones, retained argument teardown and manager no-future-dispatch are still unproven integration gates.']
    report['converterGaps']=[
        'Entity destructor delegates to native F35B40; scheduler teardown remains unverified.',
        'TrollAwake parent+4C has no proved writer/initialization in the recovered code. Do not invent one.']
    report['integrationGates']=[gap for gap in report['integrationGates'] if 'absent RockTroll actor' not in gap and 'missing actor' not in gap.lower()]
    report['integrationGates']+=report['runtimeGaps']+report['converterGaps']
    report['markerAdapterGate']='work/rock_trigger_converter/lifecycle_proposal/proposal.json; verify current source hashes before integration'
    report['copiedThingCoreGate']='work/rock_trigger_converter/copied_thing_proposal/proposal.json; ownership core only, not a complete WithCopiedThreadThing capability'
    report['actorComposition']='All recovered Main phases composed; conditional on explicit pending host capabilities. This is not a complete gameplay port.'
    report['targetingAdapterGate']='work/rock_trigger_converter/final_targeting_proposal/proposal.json; compiled x86/Lua evidence, unapplied'
    (output/'COVERAGE.json').write_text(json.dumps(report,indent=2)+'\n')
    (output/'proposals/rock_final_targeting_adapter.inc').write_text(Path(__file__).with_name('rock_final_targeting_adapter.inc').read_text())
    (output/'ACTOR_STATUS.md').write_text('''# Actor Main composition — disabled

Reproduce: python -m tools.script_recovery.rock_actor_candidate.
The actor file embeds the verified phase functions so it needs no implicit
cross-file globals. Main composes self acquisition, rewards, exhumation,
animations/control release, fresh-Hero targeting and the final cancellation
loop. The final loop always yields before its first termination query.

Main function recovery is complete only relative to the declared host contracts.
The port is not gameplay-complete. COVERAGE.json lists remaining runtime and
entity-lifecycle gaps separately. In particular, native retained thread args,
copied Thing scopes, animation binding, integer persistence defaults and final
targeting still need host integration. The final targeting adapter has a separate
actual-FSE x86/Lua gate under final_targeting_proposal; its raw targets, null
outputs and unchanged native reference counts are checked. No TrollAwake writer
is invented.

The base REVIEW/MAIN_GAPS documents describe the earlier phase work; this file
and COVERAGE.json supersede their statement that actor Main is absent. All
registration remains disabled, and canonical/runtime/game files are unchanged.
''')
    (output/'proposals/rock_lifecycle_adapter.inc').write_text(Path(__file__).with_name('rock_lifecycle_adapter.inc').read_text())
    return report


if __name__=='__main__':print(generate()['actorComposition'])
