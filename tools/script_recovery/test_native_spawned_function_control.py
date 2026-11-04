import copy,json,importlib.util
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent
FIXTURE=W if (W/'pre_lift.c').exists() else W/'fixtures/spawned_function_control'
if FIXTURE==W:
    spec=importlib.util.spec_from_file_location('candidate_control',W/'native_spawned_function_control.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
else:
    from tools.script_recovery import native_spawned_function_control as module
SOURCE=(FIXTURE/'pre_lift.c').read_text(encoding='utf-8')
FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))

def test_original_recovery():
    result=module.restore(SOURCE,FN,RData())
    assert 'ENGINE_KillSpawnedFunction("FollowThread", 0);' in result
    assert 'CScriptBase::KillSpawnedFunction(' not in result
    assert result.index('ENGINE_KillSpawnedFunction')<result.index('GSI->DeactivateQuest(')

@pytest.mark.parametrize('fault',['name','identifier','temporary','duplicate','depth','stack','missing','no_function','no_bytes','helper','comparison','constructor','receiver','destructor','zero','bypass','receiver_init','branch_into'])
def test_changed_evidence_remains_unresolved(fault):
    source=SOURCE;fn=copy.deepcopy(FN);rd=RData()
    site=next(c for c in fn['calls'] if c['target']=='0x00CB7AA0')
    if fault=='name':source=source.replace('"FollowThread"','"OtherThread"')
    elif fault=='identifier':source=source.replace('(int)&xStack_34,0);','(int)&xStack_34,1);')
    elif fault=='temporary':source=source.replace('(int)&xStack_34,0);','(int)&xStack_30,0);')
    elif fault=='duplicate':source+='\n'+next(module.PATTERN.finditer(source))[0]
    elif fault=='depth':site['depth']+=4
    elif fault=='stack':site['pushedStack']=[None,-48]
    elif fault=='missing':fn['calls'].remove(site)
    elif fault=='no_function':fn=None
    elif fault=='no_bytes':rd=None
    else:
        address={'helper':0xcb7aa0,'comparison':0x411570,'constructor':0xeea5bb,'receiver':0xeea5c6,'destructor':0xeea5d1,'zero':0xeea4c2,'bypass':0xeea4b0,'receiver_init':0xeea465,'branch_into':0xeea52f}[fault]
        original=rd
        class Changed:
            def string_at(self,a):return original.string_at(a)
            def bytes_at(self,a,n):
                value=original.bytes_at(a,n)
                if value is not None and a<=address<a+n:
                    value=bytearray(value)
                    if fault in ('bypass','branch_into'):value[address-a+2:address-a+6]=((0xeea5b0 if fault=='bypass' else 0xeea5c0)-(address+6)).to_bytes(4,'little',signed=True)
                    else:value[address-a]^=1
                    return bytes(value)
                return value
        rd=Changed()
    assert module.restore(source,fn,rd)==source

@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_main_preserves_other_behavior(style):
    from lupa.lua54 import LuaRuntime
    baseline=(FIXTURE/f'baseline_{style}.lua').read_text(encoding='utf-8')
    candidate=(FIXTURE/f'{style}.lua').read_text(encoding='utf-8')
    harness=(FIXTURE/'harness.lua').read_text(encoding='utf-8')
    def run(text,c):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text)
        config=lua.table_from(c);config.workers=lua.table_from([lua.table_from(w) for w in [dict(name='FollowThread',id=0,flag=0),dict(name='FollowThread',id=1,flag=0),dict(name='OtherThread',id=0,flag=0)]])
        return lua.execute(harness,config)
    errors=['','hero','frame','terminating','acquire','movie','fadeOut','pause','deactivate','camera','fadeIn','activeName','deactivateLater','destroyMovie','release','heroAcquire','heroRelease']
    count=0
    for delay in [0,1,3]:
      for term in [1,2,3,99]:
       for frame in [1,99]:
        for pause in [0,1,2]:
         for error in errors:
            c=dict(delay=delay,termAt=term,frameStop=frame,yieldPause=pause!=0,abortPause=pause==2,error=error)
            before=run(baseline,c);after=run(candidate,c)
            b=list(before.events.values());a=list(after.events.values());filtered=[e for e in a if not e.startswith('cancel:')]
            assert filtered==b
            assert after.ok==before.ok and after.suspensions==before.suspensions and after.status==before.status
            cancel=[i for i,e in enumerate(a) if e.startswith('cancel:')]
            assert len(cancel)<=1
            if cancel:
                assert a[cancel[0]-1]=='pause:' and a[cancel[0]+1]=='deactivate:Expression_Follow,0'
                assert after.workers[1].flag==1
            else:assert after.workers[1].flag==0
            assert after.workers[2].flag==0 and after.workers[3].flag==0
            count+=1
    assert count==1224
