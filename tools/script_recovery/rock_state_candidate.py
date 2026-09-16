"""Compose verified Rock functions only; missing actors are not emitted as stubs."""
import hashlib
import json
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,CLUSTERS,RData
from tools.script_recovery.rock_root_recovery import recover as root_recover
from tools.script_recovery.rock_trigger_recovery import recover as trigger_recover
from tools.script_recovery.rock_state_program import FIELDS,PERSISTED,functions
from tools.script_recovery.rock_sparrow_recovery import recover as sparrow_recover
from tools.script_recovery.rock_region_exit import recover as region_exit_recover
from tools.script_recovery.rock_killed import recover as killed_recover
from tools.script_recovery.rock_hit import recover as hit_recover
from tools.script_recovery.rock_actor_phases import recover as actor_phases_recover
from tools.script_recovery.rock_rewards import recover as rewards_recover
from tools.script_recovery.rock_animation import recover as animation_recover
from tools.script_recovery.rock_exhumation import recover as exhumation_recover


def verify(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('rock_state_witness.json').read_text())
    cluster=json.loads((CLUSTERS/'V_RockTrollFirstEncounter.json').read_text())
    for role,digest in witness['sources'].items():
        source=next(f['decompile'] for f in cluster['lifecycle'] if f['role']==role)
        if hashlib.sha256(source.encode()).hexdigest()!=digest:raise ValueError('Rock state source changed')
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock state native bytes changed')
    for address,value in witness['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Rock state key changed')
    pdb=ROOT/witness['pdb']['path']
    if hashlib.sha256(pdb.read_bytes()).hexdigest()!=witness['pdb']['sha256']:raise ValueError('Rock original member role evidence changed')
    return witness


def generate(output=None):
    output=Path(output or ROOT/'work/rock_trigger_converter/state_candidate').resolve()
    if not output.is_relative_to((ROOT/'work/rock_trigger_converter').resolve()):raise ValueError('Isolated output required')
    evidence=verify();root,root_evidence=root_recover();trigger,trigger_evidence=trigger_recover()
    sparrow,sparrow_evidence=sparrow_recover()
    region_exit,region_exit_evidence=region_exit_recover()
    killed,killed_evidence=killed_recover()
    hit,hit_evidence=hit_recover()
    actor_phases,actor_phases_evidence=actor_phases_recover()
    rewards,rewards_evidence=rewards_recover()
    animation,animation_evidence=animation_recover()
    exhumation,exhumation_evidence=exhumation_recover()
    files={'RockTrollFirstEncounter/RockTrollFirstEncounter.lua':functions()+'\nfunction Main(quest)\n'+root+'end\n\nfunction WatchForRegionExit(quest)\n'+region_exit+'end\n\nfunction WatchForRockTrollKilled(quest, troll)\n'+killed+'end\n\nfunction WatchForRockTrollHit(quest, troll)\n'+hit+'end\n',
        'RockTrollFirstEncounter/Entities/M_RTFERockTrollTrigger.lua':'function Main(quest, me)\n'+trigger+'end\n',
        'RockTrollFirstEncounter/Entities/RTFE_Sparrow.lua':'function Main(quest, me)\n'+sparrow+'end\n',
        'RockTrollFirstEncounter/Phases/RTFE_RockTroll.resources.lua':actor_phases+'\n'+rewards+'\n'+exhumation+'\n'+animation}
    for name,text in files.items():
        LuaRuntime().execute('return function()\n'+text+'\nend')
        target=output/name;target.parent.mkdir(parents=True,exist_ok=True)
        target.write_text('-- Disabled partial native-backed candidate; see COVERAGE.json.\n'+text)
    (output/'quests.lua').write_text('-- Registration disabled: missing RockTroll actor/helpers and pending host integration.\n')
    report={'enabled':False,'gameplayComplete':False,'functions':['Init','OnPersist','Main','WatchForRegionExit','WatchForRockTrollKilled','WatchForRockTrollHit','M_RTFERockTrollTrigger.Main','RTFE_Sparrow.Main'],
        'state':{name:{'retailOffset':hex(offset),'type':kind,'persisted':(offset,name,kind) in PERSISTED} for offset,name,kind in FIELDS},
        'native':{'state':evidence,'root':root_evidence,'trigger':trigger_evidence,'sparrow':sparrow_evidence,'regionExit':region_exit_evidence,'killed':killed_evidence,'hit':hit_evidence},
        'files':{name:hashlib.sha256((output/name).read_bytes()).hexdigest() for name in files},
        'missingFunctions':['RTFE_RockTroll.Main'],
        'partialPhases':{'RockTrollFirstEncounter/Phases/RTFE_RockTroll.resources.lua':{
            'native':actor_phases_evidence,'rewardsNative':rewards_evidence,'animationNative':animation_evidence,'exhumationNative':exhumation_evidence,'completeActorMain':False,
            'continuationContract':'Self continuation owns omitted remainder while self resource lives; Hero continuation owns omitted macro/map/movie body while both resources live. Hero phase only belongs inside native exhumation branch after item work.',
            'registration':'Not registered and does not define Main.'}},
        'readOnlyState':{'TrollAwake':{'retailOffset':'0x4c','type':'Bool','initializedHere':False,'persistedHere':False,'producer':'Unresolved: recovered helpers and audited Main have not identified a retail writer; do not fabricate one.'}},
        'requiredCapabilities':['RetailResources.PlayRockTrollAnimation(id,name)','AddRockTrollReward(actor,index)','CreateRetainedThingThread(name,actor,nativeEntryFlag,region)','WithCopiedThreadThing(troll,callback)','PersistTransferIntDefault(context,name,value,default)','WithRegionLoadedMessage',
            'GetRockTrollTriggerProximity','RetailResources.CreateCreatureAtThingPosition'],
        'integrationGates':['Bind parent-shared Bool/Int state to root and entity quest views.',
            'Native integer persistence passes default0; current PersistTransferInt defaults to current value and cannot substitute.',
            'Killed helper writes MissionOver and reads full Int RockTrollHealthBarID; missing actor must create/retain/schedule its troll argument and write the healthbar ID.',
            'WithCopiedThreadThing must own one by-value CScriptThing copy across callback, register parent-thread alive condition, and atomically poll Data MsgIsKilledBy with a CString even for empty Data. Existing raw argument and entity-condition wrappers are not substitutes.',
            'Copied Thing scope also needs short-circuit MsgIsHitOrAggressiveAbilityFrom with nested CString lifetime, GetHealth and AddHealthBarToState writing full Int before texture destruction.',
            'Init does not write TrollAlive/TrollAwake (+4B/+4C); OnPersist does not transfer MissionSucceeded/Failed/Over.',
            'Root still references absent RockTroll actor; activation must remain disabled.',
            'Host entry timing, cancellation, condition lifetime, scheduler teardown and save/restore ordering remain integration gates.']}
    (output/'COVERAGE.json').write_text(json.dumps(report,indent=2)+'\n')
    (output/'MAIN_GAPS.md').write_text(Path(__file__).with_name('rock_main_gaps.md').read_text())
    proposal=output/'proposals';proposal.mkdir(exist_ok=True)
    (proposal/'rock_reward_adapter.inc').write_text(Path(__file__).with_name('rock_reward_adapter.inc').read_text())
    (proposal/'rock_animation_method.inc').write_text(Path(__file__).with_name('rock_animation_method.inc').read_text())
    (proposal/'ANIMATION_STATUS.md').write_text('''# Animation consumer status

Unapplied, uncompiled LuaRetailResources method proposal. Add the method in its
public section and bind PlayRockTrollAnimation only after actual-FSE compilation.
It constructs the animation CString before reading byte01375748, then calls
native helper7E73D0 with seven exact32-bit slots0,0,0,1,rawbyte,0,0. This preserves
noncanonical raw byte values rather than converting through a Lua bool. Empty
resource Data is handled by the native helper, with the CString still scoped.
Original phase/helper instructions are tested separately with flags0,1,2,255.
The callback's resource object remains live after PrepareResource clears control.
No actor Main, runtime binding, scheduler or game behavior is certified here.
''')
    reward_source=Path(__file__).with_name('rock_reward_adapter.inc')
    reward_report=ROOT/'work/rock_trigger_converter/reward_proposal/proposal.json'
    compiled_reward=False
    if reward_report.exists():
        gate=json.loads(reward_report.read_text())
        compiled_copy=ROOT/'work/rock_trigger_converter/reward_proposal/rock_reward_adapter.inc'
        compiled_reward=(gate.get('peMachine')=='0x014c (x86)' and
            gate.get('inputs',{}).get(str(compiled_copy))==hashlib.sha256(reward_source.read_bytes()).hexdigest() and
            all(command['exitCode']==0 for command in gate.get('commands',[])))
    (proposal/'REWARD_STATUS.md').write_text('# Reward adapter status\n\n'+
        ('Current source passed the recorded actual-FSE x86/Lua gate; integration remains pending.\n'
         if compiled_reward else 'No matching compiled gate found for current source; compile before integration.\n')+'''
The isolated source proposal is not installed. It calls
the native CDefString hidden-output conversion at415D70, keeps the constructed
CString through AddItemToContainer, then destroys it even for token-1/empty.
It reloads the runtime definition pointer for each reward. Bind the proposed
quest method only after reviewing the isolated host proposal.
Original conversion/lookup instructions and actor phase are exercised by the
offline native-vs-Lua tests. The separate compiled gate and input hashes are in
work/rock_trigger_converter/reward_proposal/proposal.json; reproduce with
python -m tools.script_recovery.rock_reward_prepare. No game tests are implied.
''')
    (output/'REVIEW.md').write_text('''# Rock state candidate — disabled, partial

Reproduce: `python -m tools.script_recovery.rock_state_candidate`.
The output combines recovered root Init, OnPersist and Main with the trigger
Main and Sparrow Main under their actual binding paths. RockTroll Main and its
helpers remain absent, not success-returning placeholders. Binding references remain
visible so missing work cannot be mistaken for complete port coverage.

Init writes seven false booleans and a 32-bit zero health-bar ID, after disabling
Witchwood1 generators. Native +4B/+4C are untouched. Original PDB member roles
name +48/+49/+4A MissionSucceeded/MissionFailed/MissionOver; actual retail
operands establish the offsets. PDB debug offsets are not transplanted.

OnPersist transfers Activated, RockTrollTriggered, PlayedExhumeCutScene,
AddedItemsToRockTroll and the full signed 32-bit RockTrollHealthBarID, in that
order, all with zero/false defaults. MissionOver is not persisted. The native
integer callee is 410BE0, distinct from byte callee4045C0 despite the decompiler's
incorrect shared signed-char label. Native instruction execution checks save,
load and default policies against generated Lua with signed boundary values.

The current host PersistTransferInt defaults to its input value. Therefore this
candidate explicitly requires PersistTransferIntDefault(context,name,value,0).
Implement this as a separate compatible host method or an explicitly reviewed
optional-default extension before integration; silently dropping the final
argument would change missing-field/default behavior. Other required capabilities
are separately staged in runtime_proposal and region_message_proposal.

Root and trigger use the same named parent state. Loaded RockTrollTriggered
skips respawning; it does not imply MissionOver. Missing actor producers must
write MissionOver and Int RockTrollHealthBarID when recovered. Actual host
shared-state persistence, entry timing, cancellation, manager teardown and
save/restore ordering still require integration review. No runtime or game
files are modified, and quests.lua intentionally registers nothing.

Sparrow Main registers its bound-alive condition before its first frame, then
acquires its own Thing at priority4, keeping a single resource across retries.
It waits for parent TrollAwake and releases the resource; native Main has no
subsequent speech, removal or mission-state effect. TrollAwake is not initialized
or persisted here. Its missing native producer and native entry timing remain
explicit integration requirements. Cancellation releases even populated failed
acquisition output; the final post-awake termination query remains observable.

WatchForRegionExit now resolves the root's named helper reference. It polls
Witchwood1 with a fresh scoped CString each time, yields while loaded, and
checks cancellation before every repoll and before re-enabling generators on
exit. It holds no actor/resource. Its Lua body does not certify native helper
entry scheduling: the false native flag and empty region still require the
reviewed host entry policy and manager teardown guarantees.

WatchForRockTrollKilled now supplies the verified MissionOver producer. It uses
the original PDB parameter name troll. Its by-value Thing is released after
the last side effect or cancellation, while the thread's stored argument and
registered condition clone have separate later lifetimes. Each empty-name
MsgIsKilledBy poll constructs/destroys a CString even if actor Data is empty.
The proposed WithCopiedThreadThing callback scope must enforce that behavior,
register on the active parent thread through CB7920, and invalidate escaped
scope handles. Existing MsgIsKilledBy returns before CString construction for
empty Data and logs an extra getter on success, so it is not substituted.
The adapter and retained thread-argument scheduling are pending implementation;
engine-double error cleanup tests do not certify native engine fault recovery.

WatchForRockTrollHit is also recovered. It polls ordinary Hero hit before
aggressive-ability hit, retaining the first filter CString across the optional
second query. It has no initial frame or condition registration in its body.
The x87 comparison accepts health strictly greater than native float bits
0x38D1B717; NaN and equality do not create a bar. The red BGRA colour is
0000FFFF, scale1, texture HUD_QUEST_ICON_ROCK_TROLL. The full ID is written
before texture destruction, then quest info is displayed. These atomic scope
consumers remain host requirements. See MAIN_GAPS.md for the remaining actor.

The Phases directory contains separately named self/Hero resource continuation
phases. It is not an actor Main or an automatically executable approximation.
Self phase registers the retained Killed helper before acquisition. Hero phase
belongs after omitted item work inside the native exhumation branch. Native
instruction tests stop at the success continuation with resources still held;
Lua tests verify those resources survive the callback and unwind afterward.
CreateRetainedThingThread is a pending host capability. Native dispatch and
destruction tests distinguish the by-value call copy from the stored thread
argument, including empty Data and missing Info. Scheduler/manager quiescence
is not established by those tests.

WithRockTrollRewardsPhase is now available between self acquisition and the
exhumation branch. It gates both dynamic rewards on AddedItemsToRockTroll,
checks cancellation once, then consumes rewards1/2 without an invented check
between them. Native CDefString fields+730/+734 are named RockTrollReward1/2.
The definition pointer is reread per item; token-1 still sends an empty CString
to AddItemToContainer. Each temporary dies before the next read/final state
write. The outer self resource remains live through the continuation. A typed
adapter proposal is under proposals; its status checks the recorded compiled
source hash, and host integration remains pending.

WithRockTrollAnimationPhase now expresses the post-exhumation SPECIAL_BOAST /
SPECIAL_IDLE sequence, busy-task wait, cancellation and conditional control
release. The seven flags are 0,0,0,1,raw byte01375748,0,0, reread per animation
after CString construction. Original helper execution covers empty Data and
raw values2/255. PrepareResource clears control while retaining the live object
through the final-targeting continuation. The scoped consumer is staged under
proposals with an explicit uncompiled status. This phase neither reads nor
writes TrollAwake, so that unresolved producer does not block its recovery.

WithRockTrollExhumationPhase now connects the Hero acquisition continuation to
the native HERO/TROLL map, empty-name movie, pause and CS_ROCKTROLL_EXHUME macro.
It reuses existing checked resource APIs with null macro flag/input maps and
setup=false/skippable=true. Hit registration and PlayedExhumeCutScene=true
precede unpause, movie destruction, map destruction and Hero release. There is
no extra cancellation check after macro return. The actual-FSE x86/Lua gate at
work/rock_trigger_converter/exhumation_proposal exercises those APIs and error
cleanup; retained-thread registration remains an explicit engine double there.
Played-exhumation skips the complete Hero/movie phase. No TrollAwake writer is
inferred, and the final targeting phase/whole-actor composition remain absent.
''')
    return report


if __name__=='__main__':print(json.dumps(generate()['state'],indent=2))
