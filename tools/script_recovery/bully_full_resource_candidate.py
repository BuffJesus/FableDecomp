"""Emit a disabled owned-resource full body; unresolved branch operands stay visible."""
import hashlib
import json
import re
from collections import Counter
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_initial_candidate import DRAFT,correspondence
from tools.script_recovery.bully_initial_phases import recover as initial
from tools.script_recovery.bully_control_flow import recover_flow
from tools.script_recovery.bully_predicate_masks import recover_masks
from tools.script_recovery.bully_lifetime_inventory import inventory
from tools.script_recovery.bully_presented_output import prove
from tools.script_recovery.bully_runoff_movie import lower as runoff
from tools.script_recovery.bully_actor_inputs import lower as actor_inputs
from tools.script_recovery.bully_animation import lower as animation
from tools.script_recovery.bully_hit_conversation import lower as hit_conversation
from tools.script_recovery.bully_proximity import lower as proximity
from tools.script_recovery.bully_dialogue_joins import lower as dialogue_joins
from tools.script_recovery.bully_literal_scopes import lower as literal_scopes
from tools.script_recovery.bully_main_dialogue import lower as main_dialogue
from tools.script_recovery.bully_lifecycle import lower as lifecycle
from tools.script_recovery.native_new_oakvale_conditions import recover as condition

def generate(*, draft_path=DRAFT):
    lifetime,_,_,_=inventory()
    if not lifetime['cfgLifetimeProved']:raise ValueError('Bully ownership proof failed')
    draft_source=draft_path.read_text()
    output_proof=prove();source,flow=recover_flow(draft_source);source,masks=recover_masks(source)
    operand_edits=correspondence(source=draft_source);counts=Counter(e['old'] for e in operand_edits)
    for edit in {e['old']:e for e in operand_edits}.values():
        if source.count(edit['old'])!=counts[edit['old']]:raise ValueError('Bully home/health insertion changed')
        source=source.replace(edit['old'],edit['new'])
    source,entry=condition('NOVI_Bully',source)
    source=source.replace('function Main(quest, me)\n','local function resourceBody(quest, me, resources)\n',1)
    declarations='    local bully_control, bully_hero_control, bully_victim_control, bully_movie, bully_presented\n'
    helpers='''    local function finish_bully_movie()
        assert(bully_movie ~= nil, "Native Bully movie must be live at cleanup")
        resources:Pause(false)
        resources:DestroyMovie(bully_movie)
        bully_movie = nil
    end
    local function release_presented()
        if bully_presented ~= nil then
            resources:DestroyPresentedItemOutput(bully_presented)
            bully_presented = nil
        end
    end
'''
    source=source.replace('local function resourceBody(quest, me, resources)\n','local function resourceBody(quest, me, resources)\n'+declarations+helpers,1)
    pattern=r'(?m)^(?P<i> *)-- TODO\(native\): bVar3 = C3DMeshInfo::HasPhysicsMesh[^\n]*\n(?P=i)if bVar3 then\n(?P=i)end\n'
    prepares=list(re.finditer(pattern,source))
    if len(prepares)!=7:raise ValueError('Bully preparation mapping changed')
    for index,m in reversed(list(enumerate(prepares))):
        name='bully_control' if index<5 else ('bully_hero_control' if index==5 else 'bully_victim_control')
        new=(m['i']+name+' = resources:NewResource()\n' if index in (0,5,6) else '')
        new+=m['i']+'resources:PrepareResource('+name+')\n';source=source[:m.start()]+new+source[m.end():]
    acquisitions=list(re.finditer(r'me:AcquireControl\(4\)',source))
    if len(acquisitions)!=14:raise ValueError('Bully acquisition mapping changed')
    for index,m in reversed(list(enumerate(acquisitions))):
        if index<10:replacement='resources:TryAcquire(bully_control, me, 4)'
        elif index<12:replacement='resources:TryAcquire(bully_hero_control, '+('r13' if index==10 else 'r14')+', 4)'
        else:replacement='resources:TryAcquireThing(bully_victim_control, r1, 4)'
        source=source[:m.start()]+replacement+source[m.end():]
    source=source.replace('quest:GetThingWithScriptName("NOVI_Victim")','resources:NewThingFromScriptName("NOVI_Victim")')
    source=source.replace('me:IsPerformingScriptTask()','resources:IsPerformingScriptTask(bully_control)')
    source=source.replace('me:Speak(','resources:Speak(bully_control, ')
    source=source.replace('me:PlayAnimation(','resources:PlayAnimation(bully_control, ')
    source=source.replace('DAT_01375748','resources:ReadAnimationArgument5()')
    source=source.replace('                native_arg_bully_talked_with_teddy = resources:BullyTalkedWithTeddy(me)',
        '                bully_presented = resources:NewPresentedItemOutput(me)\n                native_arg_bully_talked_with_teddy = resources:BullyTalkedWithTeddy(me)')
    if source.count('me:MsgIsPresentedWithItem()')!=2:raise ValueError('Bully poll mapping changed')
    source=source.replace('me:MsgIsPresentedWithItem()','resources:PollPresentedItem(bully_presented)')
    source=source.replace('g_PresentedItemName == "OBJECT_TEDDY_BEAR_UNGIVEABLE"','resources:PresentedItemMatches(bully_presented, "OBJECT_TEDDY_BEAR_UNGIVEABLE")')
    starts=list(re.finditer(r'quest:StartMovieSequence\(\)',source));pauses=list(re.finditer(r'quest:PauseAllNonScriptedEntities\([^\n]*\)',source))
    if len(starts)!=5 or len(pauses)!=19:raise ValueError('Bully movie call correspondence changed')
    start_pauses={next(p.start() for p in pauses if p.start()>m.start()) for m in starts}
    edits=[(m.start(),m.end(),'bully_movie = resources:StartMovie("")') for m in starts]
    edits += [(m.start(),m.end(),'resources:Pause(true)' if m.start() in start_pauses else 'finish_bully_movie()') for m in pauses]
    for start,end,value in sorted(edits,reverse=True):source=source[:start]+value+source[end:]
    # Native BD7E/BD83 tails finish movie and presented string then continue at BD8C.
    for target in ('00dbbd7e','00dbbd83'):
        source=source.replace('-- TODO(native): goto LAB_'+target,'goto finishBullyItemBranch')
    # Native health>0 greets first, then both paths join at the question B7C8.
    # The decompiler put the question before the greeting and omitted its backedge.
    anchor='                    if not BullyControlledHealthAboveThreshold(resources, bully_control) then\n'
    start=source.index(anchor);question_start=start+len(anchor)
    split=source.index('                    end\n                    aVar5 = 0x0\n                    pCVar25 = 0x1',question_start)
    greeting_start=split+len('                    end\n')
    greeting_end=source.index('                    ::LAB_00dbc78b::\n',greeting_start)
    question=source[question_start:split]
    greeting=source[greeting_start:greeting_end]
    old='                    if alive then return end  -- TODO(native): goto LAB_00dbb7c8\n'
    if greeting.count(old)!=1:raise ValueError('Bully greeting/question join changed')
    greeting=greeting.replace(old,'                    if not alive then finish_bully_movie(); goto LAB_00dbccdd end\n')
    greeting=greeting.replace('if not alive then return end  -- TODO(native): goto LAB_00dbc7ac',
                              'if not alive then finish_bully_movie(); goto LAB_00dbccdd end')
    greeting=''.join('    '+line if line.strip() else line for line in greeting.splitlines(True))
    question=''.join(line[4:] if line.startswith('    ') else line for line in question.splitlines(True))
    source=(source[:start]+'                    if BullyControlledHealthAboveThreshold(resources, bully_control) then\n'
            +greeting+'                    end\n'+question+source[greeting_end:])
    source=source.replace('goto LAB_00dbbd8c','goto finishBullyItemBranch')
    source=source.replace('        ::LAB_00dbbd8c::','        ::finishBullyItemBranch::\n        release_presented()\n        ::LAB_00dbbd8c::')
    source=source.replace('            ::LAB_00dbcc2a::','            ::LAB_00dbcc2a::\n            resources:ReleaseResource(bully_victim_control)\n            bully_victim_control = nil')
    source=source.replace('        ::LAB_00dbcc33::','        ::LAB_00dbcc33::\n        resources:ReleaseResource(bully_hero_control)\n        bully_hero_control = nil')
    source=source.replace('    ::LAB_00dbccdd::','    ::LAB_00dbccdd::\n    release_presented()')
    source=source.replace('    ::LAB_00dbcce2::','    ::LAB_00dbcce2::\n    resources:DestroyThing(r1)')
    source=source.replace('    ::LAB_00dbcceb::','    ::LAB_00dbcceb::\n    resources:ReleaseResource(bully_control)')
    source,runoff_proof=runoff(source)
    source,actor_proof=actor_inputs(source)
    source,animation_proof=animation(source)
    source,conversation_proof=hit_conversation(source)
    source,proximity_proof=proximity(source)
    source,dialogue_proof=dialogue_joins(source)
    source,literal_proof=literal_scopes(source)
    source,main_dialogue_proof=main_dialogue(source)
    source,lifecycle_proof=lifecycle(source)
    # Phase definitions precede the body; this wrapper supplies the actual scope.
    phases,_=initial()
    # The standalone entry fixture is not called by this composed Main. Keep
    # only its two operand helpers, so registration has a single owner.
    phases=phases[phases.index('function BullyReturnHomePhase('):]
    source=phases+'\n'+source+'''
function Main(quest, me)
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end
'''
    LuaRuntime().execute('return function()\n'+source+'\nend')
    out=ROOT/'work/bully_converter/full_candidate';out.mkdir(parents=True,exist_ok=True)
    (out/'NOVI_Bully.lua').write_text(source)
    (out/'quests.lua').write_text('-- Disabled incomplete native candidate\nreturn {}\n')
    report={'enabled':False,'gameplayComplete':False,'fullBodyEmitted':True,'nativeLifetime':lifetime,
        'presentedOutput':output_proof,'runoff':runoff_proof,'actorInputs':actor_proof,'animation':animation_proof,'hitConversation':conversation_proof,'proximity':proximity_proof,'dialogueJoins':dialogue_proof,'literalScopes':literal_proof,'mainDialogue':main_dialogue_proof,'lifecycle':lifecycle_proof,'flow':flow,'predicates':masks,'entryCondition':entry,
        'sourceSha256':hashlib.sha256(source.encode()).hexdigest(),
        'executableMissingOperandCount':source.count('nil --[[missing]]'),
        'unappliedProposalDirectories':['work/bully_converter/talk_proposal','work/bully_converter/presented_proposal','work/bully_converter/runoff_proposal'],
        'limits':['Owned-resource body emitted for review, not certified full native behavior.',
                  'Main has no missing-operand placeholders or omitted executable goto stubs; this is not whole-engine equivalence.',
                  'Native/Lua phase traces and actual-body composition do not replace scheduler, state/save-load and callback integration checks.',
                  'Init/GivenTeddy native effects and copied-Thing lifecycle are verified offline; state ABI/save-load integration remains pending.',
                  'All required resource extensions are pending integration; merge staged header diffs, never overwrite one with another.',
                  'No canonical/runtime/game edits.']}
    (out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report

if __name__=='__main__':print(generate()[1]['fullBodyEmitted'])
