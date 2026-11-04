import copy
import hashlib
import itertools
import json
import re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_stack_call_arguments as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    result={};original=adapter.restore
    class Done(Exception):pass
    def capture(source,todo,fn,unit,rd):
        generated,remaining=original(source,todo,fn,unit,rd)
        if fn['address'].lower()=='0x00e64fb0':
            result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,readable=readable_file(generated)[0],remaining=remaining)
            raise Done()
        return generated,remaining
    with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('stack_call_args'))
    return result

@pytest.mark.parametrize('fault',['native','source','lua','frame','window','window_inventory','timer_inventory','slot','local','registration','return_copy','register','store','setter','initialization','cache','load','push1','push2','call1','call2','branch','register_slot','set_slot','cleanup_slot','api_target','api_body','api_return','block'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_stack_call_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];row=unit['reviewedStackCallArguments']['0x00e64fb0'];timer=row['timers'][1]
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='window':row['windows']['talk']['sha256']='0'*64
    elif fault=='window_inventory':row['windows'].pop('cache')
    elif fault=='timer_inventory':row['timers'].pop()
    elif fault=='slot':timer['entrySlot']-=4
    elif fault=='local':timer['local']='other'
    elif fault=='registration':timer['registerCall']=timer['setCall']
    elif fault=='return_copy':timer['returnCopy']=timer['store']
    elif fault=='register':timer['register']='edi'
    elif fault=='store':timer['store']=row['timers'][0]['store']
    elif fault=='setter':timer['setCall']=timer['registerCall']
    elif fault=='initialization':timer['initialization']='missing'
    elif fault in ['cache','load','push1','push2','call1','call2','branch']:
        key=dict(cache='cacheLoad',load='cleanupLoad',push1='firstPush',push2='secondPush',call1='firstCall',call2='secondCall',branch='terminationBranch')[fault];row[key]=timer['store']
    elif fault in ['register_slot','set_slot','cleanup_slot']:row[dict(register_slot='registerSlot',set_slot='setSlot',cleanup_slot='cleanupSlot')[fault]]+=4
    elif fault=='api_target':row['api']['table']+=4
    elif fault=='api_body':row['api']['sha256']='0'*64
    elif fault=='api_return':row['api']['return']=row['api']['start']
    elif fault=='block':row['block']=row['block'].replace('IsTalkedToByHero','other')
    with pytest.raises(ValueError):restore(source,c['todo'],fn,unit,c['rd'])

def test_unreviewed_function_unchanged(captured):
    from tools.script_recovery.native_reviewed_stack_call_arguments import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedStackCallArguments')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])

def test_only_first_cleanup_argument_changes(captured):
    c=captured;before=c['source'].splitlines();after=c['draft'].splitlines()
    assert len(before)==len(after)
    changes=[(a.strip(),b.strip()) for a,b in zip(before,after) if a!=b]
    assert changes==[('quest:DeregisterTimer(i_stk_3a4)','quest:DeregisterTimer(i_stk_3a0)')]
    assert c['remaining']==c['todo']

def executable(text):
    lines=text.splitlines();start=next(i for i,s in enumerate(lines) if 'me:IsTalkedToByHero()' in s)
    end=next(i for i in range(start,len(lines)) if 'quest:SetStateBool("TalkedTo", true)' in lines[i])+2
    block='\n'.join(lines[start:end])
    registrations=re.findall(r'(?:local )?(\w+) = quest:RegisterTimer\(\)',text)
    # Readable may retain the return alias or assign it to the stack local.
    first,second=registrations[:2]
    aliases={}
    for name in [first,second]:
        match=re.search(r'\b(\w+) = '+re.escape(name)+r'\n\s+quest:SetTimer\(\1, 0\)',text)
        aliases[name]=match[1] if match else name
    lhs=set(re.findall(r'(?m)^\s*(?:local )?(\w+) = ',block))|set(aliases.values())
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.eval('''function(first,second,talked,terminating)
     local '''+','.join(sorted(lhs))+'''
     '''+aliases[first]+'''=first;'''+aliases[second]+'''=second
     local live={[first]=true,[second]=true};local trace={};local state=false
     local quest={IsActiveThreadTerminating=function() trace[#trace+1]='terminating';return terminating end,
       DeregisterTimer=function(self,id) trace[#trace+1]='cancel:'..tostring(id);live[id]=nil end,
       SetStateBool=function(self,k,v) assert(k=='TalkedTo' and v);state=true end}
     local resources={ReleaseResource=function() trace[#trace+1]='release' end}
     local me={IsTalkedToByHero=function() trace[#trace+1]='talk';return talked end}
     local function body()
     '''+block+'''
     end
     body()
     local count=0;for _ in pairs(live) do count=count+1 end
     return table.concat(trace,','),state,count
    end''')

def test_duplicate_argument_leaves_second_timer_registered(captured):
    assert executable(captured['source'])(1,2,True,True)==('talk,terminating,cancel:1,cancel:1,release',False,1)

@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_cleanup_order_and_branches(captured,style):
    run=executable(captured[style])
    pairs=[(1,2),(0,0),(-2147483648,2147483647),(-1,7),(4294967295,2147483648),(500,1234567)]
    for first,second in pairs:
        for talked,terminating in itertools.product([False,True],repeat=2):
            cleanup=talked and terminating
            trace='talk'+(',terminating' if talked else '')+(f',cancel:{second},cancel:{first},release' if cleanup else '')
            assert run(first,second,talked,terminating)==(trace,talked and not terminating,0 if cleanup else len({first,second}))
