import copy,itertools,json,re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_saved_thing_arguments as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text());result={};original=adapter.restore
    class Done(Exception):pass
    def capture(source,todo,fn,unit,rd):
        generated,remaining=original(source,todo,fn,unit,rd)
        if fn['address'].lower()=='0x00e64fb0':
            result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,readable=readable_file(generated)[0],remaining=remaining)
            raise Done()
        return generated,remaining
    with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('saved_thing'))
    return result

@pytest.mark.parametrize('fault',['native','source','lua','frame','dependency_inventory','dependency_body','window','vtable','lookup','hero','teleport','saved_push','moving_push','flag_push','result_slot','result_lea','destructor_lea','destructor_call','literal_push','literal_address','literal','old_local','saved_alias','token_alias','duplicate_alias','flag_local'])
def test_stale_or_ambiguous_call_evidence_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_saved_thing_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];row=unit['reviewedSavedThingArguments']['0x00e64fb0']
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='dependency_inventory':row['dependencies'].pop('hero')
    elif fault=='dependency_body':row['dependencies']['lookup']['sha256']='0'*64
    elif fault=='window':row['window']['sha256']='0'*64
    elif fault=='vtable':row['vtable']+=4
    elif fault in ['lookup','hero','teleport']:row[fault+'Call']=row['destructorCall']
    elif fault in ['saved_push','moving_push','flag_push','result_lea','destructor_lea','destructor_call','literal_push']:
        key=dict(saved_push='savedPush',moving_push='movingPush',flag_push='flagPush',result_lea='resultLea',destructor_lea='destructorLea',destructor_call='destructorCall',literal_push='literalPush')[fault];row[key]=row['lookupCall']
    elif fault=='result_slot':row['resultEntrySlot']-=4
    elif fault=='literal_address':row['literalAddress']+=1
    elif fault=='literal':row['literal']='CK_ChickenStart'
    elif fault=='old_local':row['overwrittenLocal']='other'
    elif fault=='saved_alias':row['savedLocal']='CVar27'
    elif fault=='token_alias':row['ownerToken']='CVar27'
    elif fault=='duplicate_alias':row['ownerToken']=row['savedLocal']
    elif fault=='flag_local':row['flagLocal']='other'
    with pytest.raises(ValueError):restore(source,c['todo'],fn,unit,c['rd'])


def test_unreviewed_body_unchanged(captured):
    from tools.script_recovery.native_reviewed_saved_thing_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedSavedThingArguments')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])


def test_only_reviewed_argument_lifetime_changes(captured):
    c=captured;before=c['source'];after=c['draft']
    after=after.replace('    local native_call_target, native_call_target_token\n','')
    after=after.replace('native_call_target = quest:GetThingWithScriptName("CK_HeroStart")','pCVar15 = quest:GetThingWithScriptName("CK_HeroStart")')
    after=re.sub(r'(?m)^\s*native_call_target_token = resources:TrackThingOutput\(native_call_target\)\n','',after)
    after=after.replace('quest:EntityTeleportToThing(pCVar15, native_call_target, (CVar27 ~= 0))','quest:EntityTeleportToThing(pCVar15, pCVar15, (CVar27 ~= 0))')
    after=re.sub(r'(?m)^\s*resources:RetireThingOutput\(native_call_target_token\)\n','',after)
    after=re.sub(r'(?m)^\s*native_call_target = nil\n','',after)
    assert after==before and c['todo']==c['remaining']


def executable(text,old=False):
    lines=text.splitlines();a=next(i for i,s in enumerate(lines) if 'quest:GetThingWithScriptName("CK_HeroStart")' in s)
    b=next(i for i in range(a,len(lines)) if 'quest:EntityTeleportToThing(' in lines[i])+1
    if not old:
        b=next(i for i in range(b,len(lines)) if 'resources:RetireThingOutput(' in lines[i])+1
        if lines[b].strip().endswith(' = nil'):b+=1
    block='\n'.join(lines[a:b]);names=set(re.findall(r'(?m)^\s*(?:local )?(\w+) = ',block))|{'CVar27'}
    return LuaRuntime(unpack_returned_tuples=True).eval('''function(heroAlive,markerAlive,x,y,z)
     local '''+','.join(sorted(names))+'''
     CVar27=0
     local hero={alive=heroAlive,x=1,y=2,z=3};local marker={alive=markerAlive,x=x,y=y,z=z};local trace={};local retired=false;local tracked=false
     local resources={TrackThingOutput=function(self,p) assert(p==marker and not tracked);tracked=true;trace[#trace+1]='track';return 77 end,
       RetireThingOutput=function(self,id) assert(id==77 and tracked and not retired);retired=true;trace[#trace+1]='retire' end}
     local quest={GetThingWithScriptName=function(self,key) assert(key=='CK_HeroStart');trace[#trace+1]='lookup';return marker end,
       GetHero=function() collectgarbage('collect');trace[#trace+1]='hero';return hero end,
       EntityTeleportToThing=function(self,moving,target,flag) assert(moving==hero and not flag and not retired);trace[#trace+1]=(target==marker and 'marker' or 'hero');if moving.alive and target.alive then moving.x,moving.y,moving.z=target.x,target.y,target.z end end}
     '''+block+'''
     return hero.x,hero.y,hero.z,table.concat(trace,','),retired
    end''')


def test_original_overwrite_teleports_hero_to_itself(captured):
    assert executable(captured['source'],old=True)(True,True,100,-25,3.5)==(1,2,3,'lookup,hero,hero',False)

@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_saved_marker_and_retirement(captured,style):
    run=executable(captured[style])
    for hero,marker in itertools.product([False,True],repeat=2):
        for position in [(100,-25,3.5),(-10000,20000,-3),(0,0,0),(1e-20,-1e20,0.5)]:
            result=position if hero and marker else (1,2,3)
            trace='lookup,track,hero,marker,retire' if style=='draft' else 'lookup,track,marker,retire'
            assert run(hero,marker,*position)==(*result,trace,True)
