"""Isolated Wife candidate with native Init and corrected conversation key scope."""
import hashlib,json,re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.generate_affair_wife_resource_candidate import generate as resource_candidate
from tools.script_recovery.structure_affair_wife_lua import structure_wife
from tools.script_recovery.readable_affair_wife import readable_wife_source
from tools.script_recovery.readable_affair_wife_final import readable_wife_final
from tools.script_recovery.wife_init_recovery import SOURCE as INIT,prove as prove_init
from tools.script_recovery.wife_conversation_native import prove as prove_conversation
from tools.script_recovery.wife_dispatcher_native import prove as prove_dispatcher
from tools.script_recovery.wife_animation_recovery import lower as lower_animation

def correct_approach_entry(source):
    """DB34B0 yields/queries before each running-line and distance iteration."""
    prove_dispatcher()
    start=source.index('    local function waitUntilNearHusband()')
    end=source.index('    local function processHeroInteraction()',start)
    body=source[start:end]
    frame='''            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return false end
'''
    # Historical comments are stripped by lower before this correction.
    if body.count(frame)!=1 or body.count('        while true do\n')!=1:
        raise ValueError('Wife approach frame boundary changed')
    body=body.replace(frame,'').replace('        while true do\n','        while true do\n'+frame,1)
    return source[:start]+body+source[end:]

def baseline(**kwargs):
    source,report=resource_candidate(**kwargs);source,_=structure_wife(source);source,_=readable_wife_source(source);source,_=readable_wife_final(source)
    return source,report

def lower(source):
    witness=json.loads(Path(__file__).with_name('wife_complete_candidate_witness.json').read_text())
    if hashlib.sha256(source.encode()).hexdigest()!=witness['sourceSha256']:raise ValueError('Wife complete candidate correspondence changed')
    init=prove_init();conversation=prove_conversation()
    start=source.index('function Init(');end=source.index('\nlocal function __resource_main(',start)
    new=INIT.replace('function WifeRecoveredInit(quest, me, state)','function Init(quest, me)').replace('state:SetStateBool','__native_entity_state:SetStateBool')
    source=source[:start]+new+source[end:]
    old='''                conversationId = quest:AddNewConversation(me, false, false)
                wifeParticipant = quest:GetHero()
                quest:AddPersonToConversation(conversationId, wifeParticipant)
                hero6 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND", me, hero6, false)
'''
    if source.count(old)!=1:raise ValueError('Wife intermittent conversation mapping changed')
    source=source.replace(old,'                resources:AddWifeWhereHusbandConversation(me)\n')
    todos=re.findall(r'-- TODO\(native\):[^\n]*',source)
    source=re.sub(r'(?m)^ *-- TODO\(native\):[^\n]*\n','',source)
    source=re.sub(r'  -- TODO\(native\):[^\n]*','',source)
    source=source.replace('if true then return end','return')
    source=correct_approach_entry(source)
    source,animation=lower_animation(source)
    source=source.replace('-- DISABLED incomplete wife candidate; conversation string lifetimes and behavior review pending.',
        '-- DISABLED Wife candidate; native phase/adapter proofs do not establish gameplay integration.')
    return source,{'init':init,'intermittentConversation':conversation,'historicalCommentsRemoved':todos,
        'argumentAnimation':animation,
        'approachEntryCorrection':{'nativeFrame':'DB34B5','nativeQuery':'DB34BA','behavior':'Yield and query cancellation before first and subsequent approach iterations.'},
        'limits':['Main dispatcher still relies on separately verified phases and source-to-source tests.',
                  'Other live dynamic key scopes use existing argument-key proposal; full merged owner remains pending.',
                  'Engine scheduler/state persistence/save-load and arbitrary native exceptions remain unverified.']}

def generate(**kwargs):
    source,report=baseline(**kwargs);source,evidence=lower(source)
    out=ROOT/'work/wife_complete_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'CANDIDATE.lua').write_text(source);(out/'quests.lua').write_text('Quests = {} -- Disabled offline candidate.\n')
    report['completionPass']=evidence;report['sha256']=hashlib.sha256(source.encode()).hexdigest()
    (out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n');return source,report

if __name__=='__main__':generate()
