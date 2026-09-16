"""Generate an isolated woman candidate from checked native ownership/operand maps."""
import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.generate_affair_man_resource_candidate import Rewrite
from tools.script_recovery.native_affair_woman_control import recover
from tools.script_recovery.native_affair_woman_resources import verify as resource_map
from tools.script_recovery.native_affair_woman_movies import verify as movie_map
from tools.script_recovery.native_affair_woman_actors import verify as actor_map

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWoman.lua'


def generate(out=None, *, draft=DRAFT, data=None):
    data=data or RData()
    unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    function=next(f for f in unit['functions'] if int(f['address'],16)==0xDB1F00)
    resources=resource_map(function,data); movies=movie_map(function,data); actors=actor_map(function,data)
    source,control=recover(Path(draft).read_text(),data)
    if data.bytes_at(0x122DEDC,4)!=b'\0'*4:
        raise ValueError('AffairWoman health threshold changed')
    counts=Counter(e['name'] for e in resources['events']); edits={}
    def replace(name,pattern,replacement,count):
        nonlocal source
        source,edits[name]=Rewrite(name,pattern,replacement,count).apply(source)
    replace('hit scope',re.escape(actors['hitScope']['oldLua']),
            '        bVar5 = resources:IsHitByHeroExceptAbility(me, 14)\n',1)
    replace('hit branch',r'if cStack_a5 ~= 0 then','if bVar5 then',1)
    replace('resource preparation',r'(?m)^(?P<i> *)-- TODO\(native\): bVar5 = C3DMeshInfo::HasPhysicsMesh[^\n]+\n(?P=i)if bVar5 then\n(?P=i)end',
            r'\g<i>resources:PrepareResource(woman_resource)',counts['has'])
    replace('receiver staging',r'(?m)^ *-- TODO\(native\): (?:pppuVar23|ppVar22) =[^\n]+\n','',2)
    replace('acquisition staging',r'(?m)^ *uVar19 = SUB41\(me,0\)\n','',2)
    replace('acquire',r'me:AcquireControl\(', 'resources:TryAcquire(woman_resource, me, ',counts['acquire'])
    replace('task',r'me:IsPerformingScriptTask\(\)','resources:IsPerformingScriptTask(woman_resource)',counts['task'])
    replace('speech',r'me:Speak\(','resources:Speak(woman_resource, ',counts['speak'])
    replace('animation',r'me:PlayAnimation\(','resources:PlayAnimation(woman_resource, ',counts['animate'])
    replace('animation byte',r'\bDAT_01375748\b','resources:ReadAnimationArgument5()',2)
    replace('retained lookup',r'quest:GetThingWithScriptName\(','resources:NewThingFromScriptName(',3)
    replace('home snapshot',r'(?m)^            me:GetHomePos\(\)$','            woman_home = me:GetHomePos()',1)
    replace('home movement',re.escape('me:MoveToPosition(pCVar7, aCStack_40, 0x0, false, false)'),
            'resources:MoveToPosition(woman_resource, woman_home, 0.0, 0, false, true)',1)
    replace('runoff movement',re.escape('me:MoveToPosition(native_arg_runoff_position, 0.0, 1, false, true)'),
            'resources:MoveToPosition(woman_resource, native_arg_runoff_position, 0.0, 1, false, true)',1)
    replace('owned marker position',r'RetailThingPosition\(r6\)','resources:ThingPosition(r6)',1)
    for label,position,count in [('home','aCStack_40',1),('runoff','native_arg_runoff_position',2)]:
        expression='woman_home' if label=='home' else position
        replace(label+' distance',r'(?m)^(?P<i> *)-- TODO\(native\): pCVar8 = [^\n]+\n'
            r'(?P=i)-- TODO\(native\): bVar5 = IsDistanceFromThingToPositionOver\(pCVar8,'+position+r',fVar21\);\n'
            r'(?P=i)bVar5 = nil --\[\[unresolved native result\]\]',
            r'\g<i>woman_thing = resources:NewThingFromResource(woman_resource)\n'
            r'\g<i>bVar5 = resources:ThingIsDistanceFromPositionOver(woman_thing, '+expression+r', 2.0)\n'
            r'\g<i>resources:DestroyThing(woman_thing); woman_thing = nil',count)
    replace('health temporaries',r'(?m)^(?P<i> *)-- TODO\(native\): (?:uVar9 = )?CScriptGameResourceObjectScriptedThingBase::_GetScriptThing[^\n]+\n'
            r'(?P=i)fVar14 = quest:GetHealth\([^\n]+\)',
            r'\g<i>woman_thing = resources:NewThingFromResource(woman_resource)\n'
            r'\g<i>fVar14 = resources:ThingHealth(woman_thing)\n'
            r'\g<i>resources:DestroyThing(woman_thing); woman_thing = nil',2)
    replace('health zero',r'\b_DAT_0122dedc\b','0.0',2)
    replace('face hero',re.escape('quest:EntitySetFacingAngleTowardsThing(r4, r1)'),
            'resources:FaceThing(r2, r4, false)',1)
    replace('face self',re.escape('quest:EntitySetFacingAngleTowardsThing(me, nil --[[missing]], false)'),
            'resources:FaceThing(r2, me, false)',1)
    replace('wife distance',re.escape('quest:IsDistanceBetweenThingsUnder(me, nil --[[missing]], apStack_98)'),
            'resources:ThingsAreWithinDistance(me, r1, 5.0)',1)
    replace('talk clear actions',r'(?m)^ *-- TODO\(native\): CCarriedReadableDef::CCarriedReadableDef\(aCStack_68\);\n',
            '            resources:ClearAllActions(woman_resource)\n            resources:ClearCommands(woman_resource)\n',1)
    replace('hit movie constructor',r'(?m)^ *-- TODO\(native\): CCarriedReadableDef::CCarriedReadableDef\(aCStack_34\);\n','',1)
    replace('movie starts',r'quest:StartMovieSequence\(\)\n(?P<i> *)quest:PauseAllNonScriptedEntities\(false\)',
            r'woman_movie = resources:StartMovie("")\n\g<i>resources:Pause(true)',2)
    replace('movie exits',r'quest:PauseAllNonScriptedEntities\((?:false|\(fVar21 ~= 0\))\)', 'finish_movie()',6)
    replace('movie cancellation returns',r'if not alive then return end  -- TODO\(native\): goto LAB_00db(?:2715|274c)',
            'if not alive then finish_movie(); goto LAB_00db2974 end',2)
    replace('retained marker end',r'(    ::LAB_00db296b::\n)',r'\g<1>    resources:DestroyThing(r6); r6 = nil\n',1)
    replace('retained partners end',r'(    ::LAB_00db2974::\n)',
            r'\g<1>    resources:DestroyThing(r2); r2 = nil\n    resources:DestroyThing(r1); r1 = nil\n',1)
    replace('resource end',r'(    ::LAB_00db2986::\n)',r'\g<1>    resources:ReleaseResource(woman_resource); woman_resource = nil\n',1)
    replace('resource construct',r'(    end\n)(    resources:PrepareResource\(woman_resource\))',
            r'\g<1>    woman_resource = resources:NewResource()\n\g<2>',1)
    replace('entry',r'function Main\(quest, me\)\n','''local function __resource_main(quest, me, resources)
    local woman_resource, woman_thing, woman_movie, woman_home
    local function finish_movie()
        resources:Pause(false)
        resources:DestroyMovie(woman_movie); woman_movie = nil
    end
''',1)
    source+='''
function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
'''
    source='-- DISABLED resource-aware woman candidate; native/runtime parity checks remain pending.\n'+source
    syntax=LuaSyntaxChecker().check({'woman.lua':source})
    if not syntax['ok']:raise ValueError('Woman candidate syntax failed: '+repr(syntax))
    report={'status':'disabled-incomplete','sha256':hashlib.sha256(source.encode()).hexdigest(),
        'rewrites':edits,'resourceMap':resources,'movies':movies,'actors':actors,'control':control,'syntax':syntax,
        'requiresResourceExtension':['ThingPosition','MoveToPosition','PlayAnimation','ReadAnimationArgument5',
            'IsHitByHeroExceptAbility','ThingIsDistanceFromPositionOver','FaceThing','ThingsAreWithinDistance','ClearAllActions','ClearCommands'],
        'runtimeProposal':'work/woman_resource_integration/proposal.json',
        'remaining':['Owned ThingPosition and other resource extensions are staged, not installed.',
            'Standalone candidate omits entry condition; readable builder restores it from native evidence.',
            'Complete native operand review and DLL/gameplay validation remain pending; mocked traces cover selected paths.']}
    if out is not None:
        out=Path(out);out.mkdir(parents=True,exist_ok=True)
        (out/'NOVI_AffairWoman.resource_candidate.lua').write_text(source)
        (out/'NOVI_AffairWoman.resource_candidate.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--out',type=Path,default=ROOT/'work/affair_woman_candidate')
    args=parser.parse_args();_,report=generate(args.out);print(json.dumps({'rewrites':report['rewrites'],'syntax':report['syntax']},indent=2))
