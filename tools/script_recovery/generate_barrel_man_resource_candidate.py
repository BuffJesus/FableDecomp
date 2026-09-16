"""Generate disabled Barrel Man control lowering; report remaining ownership gaps."""
import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.generate_affair_man_resource_candidate import Rewrite
from tools.script_recovery.native_barrel_man_resources import verify as resource_map
from tools.script_recovery.native_barrel_man_temporary_things import verify as temporary_map
from tools.script_recovery.native_barrel_man_setup import recover as recover_setup
from tools.script_recovery.native_barrel_camera_cancellation import recover as recover_camera
from tools.script_recovery.native_barrel_man_interactions import recover as recover_interactions
from tools.script_recovery.native_barrel_man_allies import recover as recover_allies
from tools.script_recovery.native_barrel_man_health_branches import recover as recover_health_branches
from tools.script_recovery.native_barrel_failure_movie import recover as recover_failure_movie, HELPER as FAILURE_MOVIE_HELPER
from tools.script_recovery.native_barrel_man_movies import verify as movie_map
from tools.script_recovery.native_barrel_speech_movies import recover as recover_speech_movies
from tools.script_recovery.native_barrel_return_dialogue import recover as recover_return_dialogue
from tools.script_recovery.native_barrel_man_timer import verify as timer_map
from tools.script_recovery.native_barrel_initial_interaction import recover as recover_initial_interaction
from tools.script_recovery.native_barrel_departure_scopes import recover as recover_departure_scopes
from tools.script_recovery.native_barrel_walkoff_scope import recover as recover_walkoff_scope
from tools.script_recovery.native_barrel_teleport_scope import recover as recover_teleport_scope
from tools.script_recovery.native_barrel_return_walk import recover as recover_return_walk
from tools.script_recovery.native_barrel_return_encounter import recover as recover_return_encounter
from tools.script_recovery.native_barrel_conversations import recover as recover_conversations
from tools.script_recovery.native_barrel_overhear import recover as recover_overhear
from tools.script_recovery.readable_barrel_control import recover as recover_control
from tools.script_recovery.readable_barrel_phase import recover as recover_phase
from tools.script_recovery.readable_barrel_loop import recover as recover_loop
from tools.script_recovery.native_new_oakvale_conditions import recover as recover_entry
from tools.script_recovery.native_barrel_init_timer import recover as recover_init
from tools.script_recovery.native_barrel_man_markers import verify as marker_map

DRAFT = ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua'


def generate(out=None, *, draft=DRAFT, data=None):
    data = data or RData()
    source = Path(draft).read_text()
    expected = Path(__file__).with_name('barrel_man_candidate_draft.sha256').read_text().strip()
    if hashlib.sha256(source.encode()).hexdigest() != expected:
        raise ValueError('Barrel resource draft correspondence changed')
    _, entry_condition = recover_entry('NOVI_BarrelMan', source, data)
    unit = json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    function = next(f for f in unit['functions'] if int(f['address'],16)==0xDB5330)
    resources = resource_map(function,data)
    temporaries = temporary_map(function,data)
    movies = movie_map(function,data)
    timer = timer_map(function,data)
    markers = marker_map(function,data)
    source, setup = recover_setup(source,data)
    source, camera = recover_camera(source,data)
    source, interactions = recover_interactions(source,data)
    source, allies = recover_allies(source,data)
    source, health_branches = recover_health_branches(source,data)
    source, failure_movie = recover_failure_movie(source,data)
    counts = Counter(e['name'] for e in resources['events']); edits = {}
    def replace(name, pattern, replacement, count):
        nonlocal source
        source, edits[name] = Rewrite(name,pattern,replacement,count).apply(source)
    replace('preparation',r'(?m)^(?P<i> *)-- TODO\(native\): bVar3 = C3DMeshInfo::HasPhysicsMesh[^\n]+\n(?P=i)if bVar3 then\n(?P=i)end',
            r'\g<i>resources:PrepareResource(barrel_resource)',counts['has'])
    replace('acquire',r'me:AcquireControl\(4\)', 'resources:TryAcquire(barrel_resource, me, 4)',counts['acquire'])
    replace('task',r'me:IsPerformingScriptTask\(\)', 'resources:IsPerformingScriptTask(barrel_resource)',counts['task'])
    replace('speech',r'me:Speak\(', 'resources:Speak(barrel_resource, ',counts['speak'])
    for position in ('native_arg_walkoff_position','native_arg_man_start_position'):
        replace(position+' movement',re.escape('me:MoveToPosition('+position+', 0.0, 1, false, false)'),
                'resources:MoveToPosition(barrel_resource, '+position+', 0.0, 1, false, false)',1)
        replace(position+' distance',re.escape('bVar3 = (me ~= nil and me:IsDistanceFromPositionOver('+position+', fVar25))'),
                'bVar3 = controlled_distance('+position+', 2.0)',2)
    replace('hero movement',re.escape('me:MoveToPosition(pCVar7, SUB41(me,0), SUB41(pCVar22,0))'),
            'resources:MoveToPosition(barrel_resource, pCVar7, 2.0, 1, false, true)',1)
    replace('health',r'quest:GetHealth\(me\)', 'controlled_health()',7)
    replace('construct',re.escape('    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")'),
            '    barrel_resource = resources:NewResource()\n    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")',1)
    replace('normal release',r'(    ::LAB_00db6afd::\n)',
            r'\g<1>    resources:ReleaseResource(barrel_resource); barrel_resource = nil\n',1)
    replace('entry',r'function Main\(quest, me\)\n','''local function __resource_main(quest, me, resources)
    local barrel_resource
    local function controlled_health()
        local thing = resources:NewThingFromResource(barrel_resource)
        local health = resources:ThingHealth(thing)
        resources:DestroyThing(thing)
        return health
    end
    local function controlled_distance(position, distance)
        local thing = resources:NewThingFromResource(barrel_resource)
        local isOver = resources:ThingIsDistanceFromPositionOver(thing, position, distance)
        resources:DestroyThing(thing)
        return isOver
    end
''',1)
    header='local function __resource_main(quest, me, resources)\n'
    source=source.replace(header,header+FAILURE_MOVIE_HELPER,1)
    source, speech_movies = recover_speech_movies(source,data)
    source, return_dialogue = recover_return_dialogue(source,data)
    source, initial_interaction = recover_initial_interaction(source,data)
    source, departure_scopes = recover_departure_scopes(source,data)
    source, walkoff_scope = recover_walkoff_scope(source,data)
    source, teleport_scope = recover_teleport_scope(source,data)
    source, return_walk = recover_return_walk(source,data)
    source, return_encounter = recover_return_encounter(source,data)
    source, conversations = recover_conversations(source,data)
    source, overhear = recover_overhear(source,data)
    source, control_flow = recover_control(source)
    source, phase_flow = recover_phase(source)
    source, main_loop = recover_loop(source)
    source, init_recovery = recover_init(source,data)
    entry_anchor='    quest:NewScriptFrame(me)\n    if quest:IsActiveThreadTerminating() then return end\n'
    if source.count(entry_anchor)!=1:
        raise ValueError('Barrel structured entry correspondence changed')
    source=source.replace(entry_anchor,'    quest:'+entry_condition['method']+'()\n'+entry_anchor,1)
    timer_bytes=data.bytes_at(0xDB552D,0xDB5547-0xDB552D)
    if timer_bytes is None or hashlib.sha256(timer_bytes).hexdigest()!='7021d167d997ac79250bf21df3ee3b0c57f8405ba971633734b4858bc9b6c225':
        raise ValueError('Barrel phase-one global timer instructions changed')
    timer_call='quest:SetTimer(quest:GetStateInt("WatchTimer"), 45)'
    if source.count(timer_call)!=1:raise ValueError('Barrel phase-one timer source changed')
    source=source.replace(timer_call,'resources:SetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))',1)
    source += '''
function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
'''
    source = '-- DISABLED REVIEW CANDIDATE: structured Main; staged runtime adapters and Init/lifecycle validation remain.\n'+source
    syntax = LuaSyntaxChecker().check({'barrel.lua':source})
    if not syntax['ok']: raise ValueError('Barrel resource candidate syntax failed: '+repr(syntax))
    report = dict(status='disabled-incomplete-control-candidate',sha256=hashlib.sha256(source.encode()).hexdigest(),
                  rewrites=edits,resourceMap=resources,temporaryMap=temporaries,movieMap=movies,timerMap=timer,markerMap=markers,walkoffScope=walkoff_scope,teleportScope=teleport_scope,initialInteraction=initial_interaction,departureScopes=departure_scopes,failureMovie=failure_movie,speechMovies=speech_movies,returnDialogue=return_dialogue,setup=setup,camera=camera,interactions=interactions,allies=allies,healthBranches=health_branches,syntax=syntax,
                  remaining=['Init timer/actor APIs and entity-state lifecycle still require native/runtime review.',
                             'Full-loop phase transitions and composed runtime behavior need broader validation.',
                             'Combined staged adapters: work/barrel_resource_integration; isolated ABI/Lua checks and registration compilation pass, full DLL/gameplay remain pending.',
                             'Scope cleanup on cancellation/errors is a host policy; whole native cleanup order remains unproven.',
                             'Entry condition is restored and verified; staged resource adapters remain unapplied and gameplay unvalidated.'])
    report["returnWalk"] = return_walk
    report["returnEncounter"] = return_encounter
    report["conversations"] = conversations
    report["overhear"] = overhear
    report["controlFlow"] = control_flow
    report["phaseFlow"] = phase_flow
    report["mainLoop"] = main_loop
    report["entryCondition"] = entry_condition
    report["init"] = init_recovery
    if out is not None:
        out=Path(out);out.mkdir(parents=True,exist_ok=True)
        (out/'NOVI_BarrelMan.resource_candidate.lua').write_text(source)
        (out/'NOVI_BarrelMan.resource_candidate.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',type=Path,default=ROOT/'work/barrel_man_candidate')
    args=parser.parse_args();_,report=generate(args.out)
    print(json.dumps({'status':report['status'],'rewrites':report['rewrites'],'syntax':report['syntax']},indent=2))
