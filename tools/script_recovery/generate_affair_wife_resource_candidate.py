"""Build a disabled ownership-aware wife candidate; unresolved calls stay explicit."""
import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.generate_affair_man_resource_candidate import Rewrite
from tools.script_recovery.native_affair_wife_resource_scope import verify as resource_map
from tools.script_recovery.native_affair_wife_movie_scopes import verify as movie_map
from tools.script_recovery.native_affair_wife_actor_scope import verify as actor_map
from tools.script_recovery.native_affair_wife_hit_scopes import verify as hit_map
from tools.script_recovery.native_affair_wife_argument_key import verify as argument_key_map
from tools.script_recovery.native_affair_wife_talk import verify as talk_map
from tools.script_recovery.native_new_oakvale_conditions import recover as recover_entry

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua'


def generate(out=None, *, draft=DRAFT, data=None):
    data=data or RData()
    unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB2B10)
    resources=resource_map(function,data);movies=movie_map(function,data);actors=actor_map(function,data)
    hits=hit_map(data)
    argument_key=argument_key_map(data)
    talk=talk_map(data)
    source=Path(draft).read_text()
    expected=Path(__file__).with_name('affair_wife_candidate_draft.sha256').read_text().strip()
    if hashlib.sha256(source.encode()).hexdigest()!=expected:
        raise ValueError('AffairWife candidate draft changed')
    edits={};counts=Counter(e['name'] for e in resources['events'])
    def replace(name,pattern,replacement,count):
        nonlocal source
        source,edits[name]=Rewrite(name,pattern,replacement,count).apply(source)
    for index,block in enumerate(hits['blocks']):
        indent=block['oldLua'][:len(block['oldLua'])-len(block['oldLua'].lstrip())]
        replace('scoped hit '+str(index),re.escape(block['oldLua']),
                indent+block['result']+' = resources:IsHitByHeroExceptAbility(me, 14)\n',1)
    replace('preparation',r'(?m)^(?P<i> *)-- TODO\(native\): bVar4 = C3DMeshInfo::HasPhysicsMesh[^\n]+\n(?P=i)if bVar4 then\n(?P=i)end',
            r'\g<i>resources:PrepareResource(wife_resource)',counts['has'])
    replace('acquire',r'me:AcquireControl\(','resources:TryAcquire(wife_resource, me, ',counts['acquire'])
    for native,method in [('task','IsPerformingScriptTask'),('clear_actions','ClearAllActions'),('clear_commands','ClearCommands')]:
        replace(native,r'me:'+method+r'\(\)','resources:'+method+'(wife_resource)',counts[native])
    for native,method in [('speak','Speak'),('move','MoveToPosition'),('animate','PlayAnimation')]:
        replace(native,r'me:'+method+r'\(','resources:'+method+'(wife_resource, ',counts[native])
    replace('animation byte',r'\bDAT_01375748\b','resources:ReadAnimationArgument5()',2)
    replace('health',r'(?m)^(?P<i> *)fVar22 = quest:GetHealth\(me\)',
            r'\g<i>wife_thing = resources:NewThingFromResource(wife_resource)\n\g<i>fVar22 = resources:ThingHealth(wife_thing)\n\g<i>resources:DestroyThing(wife_thing); wife_thing = nil',6)
    # FCOMP/FNSTSW/test AH,41 rejects unordered health as well as <= zero.
    replace('unordered health',r'if fVar22 <= fVar3 then','if not (fVar22 > fVar3) then',1)
    replace('husband lookup',r'quest:GetThingWithScriptName\("NOVI_AffairMan"\)',
            'resources:NewThingFromScriptName("NOVI_AffairMan")',1)
    replace('husband position',r'RetailThingPosition\(r1\)','resources:ThingPosition(r1)',1)
    replace('husband distance',r'quest:IsDistanceBetweenThingsUnder\(me, r1, 3.0\)',
            'resources:ThingsAreWithinDistance(me, r1, 3.0)',2)
    replace('husband facing',r'quest:EntitySetFacingAngleTowardsThing\(me, r1, ',
            'resources:FaceThing(me, r1, ',3)
    # Use the staged resource bridge, which resolves owned Thing IDs explicitly.
    replace('husband participant',r'quest:AddPersonToConversation\(ppVar15, r1\)',
            'resources:AddConversationPerson(ppVar15, r1)',1)
    replace('husband lines',r'quest:AddLineToConversation\(ppVar15, ',
            'resources:AddConversationLine(ppVar15, ',2)
    replace('unused receiver byte',r'(?m)^ *CVar29 = SUB41\(me,0\)\n','',2)
    replace('movie start',r'quest:StartMovieSequence\(\)',
            'wife_movie = resources:StartMovie("")',4)
    replace('pause',r'quest:PauseAllNonScriptedEntities\(', 'resources:Pause(',30)
    replace('movie end',r'quest:EndMovieSequence\(\)',
            'resources:DestroyMovie(wife_movie); wife_movie = nil',26)
    replace('omitted partner cleanup exits',r'(?m)^( *)-- TODO\(native\): goto LAB_00db3d(?:6a|90)\n',r'\1return\n',3)
    def argument_scope(match):
        indent=match['i']
        body='''local continueArgument = resources:WithArgumentKey(native_arg_wife_line_counter, function(argumentKey)
    if not argumentKey:Exists() then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return false end
        native_arg_wife_line_counter = 10
        argumentKey:ResetToFirst()
    end
    resources:AddArgumentKeyLine(argumentKey, ppVar15, me, r1)
    native_arg_wife_reply_remainder = quest:RetailRandModulo(2)
    if native_arg_wife_reply_remainder == 0 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return false end
        resources:AddConversationLine(ppVar15, "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE", r1, me, false)
    end
    return true
end)
if not continueArgument then return end
'''
        return ''.join(indent+line+'\n' for line in body.splitlines())
    replace('persistent argument key',
        r'(?m)^(?P<i> *)native_arg_wife_line_key = "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_"[^\n]+\n'
        r'[\s\S]*?resources:AddConversationLine\(ppVar15, "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE", r1, me, false\)\n(?P=i)end\n',
        argument_scope,1)
    replace('entry',r'function Main\(quest, me\)\n',
            'local function __resource_main(quest, me, resources)\n    local wife_resource, wife_thing, wife_movie\n',1)
    replace('body scope',r'    native_arg_wife_argument_id = 0\n',
            '    wife_resource = resources:NewResource()\n    local function runBody()\n    native_arg_wife_argument_id = 0\n',1)
    replace('body end',r'    ::LAB_00db3e16::\nend',
            '''    ::LAB_00db3e16::
    end
    runBody()
    assert(wife_movie == nil, "wife movie cleanup was bypassed")
    if r1 ~= nil then resources:DestroyThing(r1); r1 = nil end
    resources:ReleaseResource(wife_resource); wife_resource = nil
end''',1)
    source+='''
function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
'''
    source='-- DISABLED incomplete wife candidate; conversation string lifetimes and behavior review pending.\n'+source
    source,entry_condition=recover_entry('NOVI_AffairWife',source,data)
    syntax=LuaSyntaxChecker().check({'wife.lua':source})
    if not syntax['ok']:raise ValueError('Wife candidate syntax failed: '+repr(syntax))
    report=dict(status='disabled-incomplete',sha256=hashlib.sha256(source.encode()).hexdigest(),rewrites=edits,
                resourceMap=resources,movies=movies,actors=actors,syntax=syntax,
                argumentKey=argument_key,
                talkScopes=talk,
                entryCondition=entry_condition,
                hitScopes=[{k:v for k,v in block.items() if k!='oldLua'} for block in hits['blocks']],
                remaining=['Argument-key resource methods pass isolated x86 real-Lua checks; complete DLL/gameplay integration pending.',
                    'Talk query scopes verified with existing FSE method; other conversation-string lifetimes pending.',
                    'Complete operands, native parity and readable structure pending.'])
    if out is not None:
        out=Path(out);out.mkdir(parents=True,exist_ok=True)
        (out/'NOVI_AffairWife.resource_candidate.lua').write_text(source)
        (out/'NOVI_AffairWife.resource_candidate.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',type=Path,default=ROOT/'work/affair_wife_candidate')
    args=parser.parse_args();_,report=generate(args.out)
    print(json.dumps({'rewrites':report['rewrites'],'syntax':report['syntax']},indent=2))
