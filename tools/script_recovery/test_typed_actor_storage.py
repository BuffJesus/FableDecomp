"""Actor temporaries must survive typed argument placement and conditional stores."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter
from tools.script_recovery.test_lift_native_lua import NoRData

@pytest.mark.parametrize('same',[False,True])
def test_staged_actor_is_equal_to_operand(same):
    manifest={'IsEqualTo':{'scope':'Thing','returnType':'bool','parameters':[{'name':'other','type':'CScriptThing'}]}}
    lifter=Lifter(manifest,{},'quest',False,'Example',NoRData(),thing_sigs={'IsEqualTo':'?IsEqualTo@CScriptThing@@UBE_NABV1@@Z'})
    lifter.kinds.update(actor='thing',other='thing')
    lifter.locals.update(('actor','other'));lifter.push_temp('other','other')
    lifter.thing_call('equal','IsEqualTo','actor','other')
    assert not lifter.todo
    lua=LuaRuntime()
    code='''
local actor,other={},{}
if SAME then other=actor end
function actor:IsEqualTo(value) assert(value==other);return self==value end
BODY
return equal
'''.replace('SAME','true' if same else 'false').replace('BODY','\n'.join(lifter.out))
    assert lua.execute(code)==same

@pytest.mark.parametrize('kind',['string',None])
def test_ambiguous_staged_operand_remains_unresolved(kind):
    manifest={'IsEqualTo':{'scope':'Thing','returnType':'bool','parameters':[{'name':'other','type':'CScriptThing'}]}}
    lifter=Lifter(manifest,{},'quest',False,'Example',NoRData(),thing_sigs={'IsEqualTo':'?IsEqualTo@CScriptThing@@UBE_NABV1@@Z'})
    lifter.kinds['actor']='thing'
    if kind:lifter.kinds['other']=kind
    lifter.locals.update(('actor','other'));lifter.push_temp('other','other')
    lifter.thing_call('equal','IsEqualTo','actor','other')
    assert lifter.todo and 'nil --[[missing]]' in '\n'.join(lifter.out)

@pytest.mark.parametrize('shared',[False,True])
@pytest.mark.parametrize('swap',[False,True])
def test_conditional_swap_preserves_both_lookup_results(shared,swap):
    manifest={'GetThingWithScriptName':{'scope':'Quest','returnType':'CScriptThing','parameters':[{'name':'scriptName','type':'std::string'}]}}
    lifter=Lifter(manifest,{},'quest',False,'Example',NoRData());lifter.accessor_kinds=True
    lifter.interface_call('local_left','GetThingWithScriptName','&local_left,"LEFT"')
    lifter.interface_call('local_right','GetThingWithScriptName','&local_right,"RIGHT"')
    lifter.emit('if SWAP then');lifter.indent+=1
    for statement in ('native_swap_handle = (CScriptThing *)local_left;',
                      'local_left = (CScriptThing *)local_right;',
                      'local_right = (CScriptThing *)native_swap_handle;'):
        lifter.statement('Main',statement)
    lifter.indent-=1;lifter.emit('end');lifter.emit('return local_left,local_right')
    assert not lifter.todo
    lua=LuaRuntime(unpack_returned_tuples=True)
    code='''
local left,right={},{}
if SHARED then right=left end
local quest={GetThingWithScriptName=function(_,name) return name=='LEFT' and left or right end}
local a,b=(function()
BODY
end)()
assert(a==(SWAP and right or left) and b==(SWAP and left or right))
return true
'''.replace('SHARED','true' if shared else 'false').replace('SWAP','true' if swap else 'false').replace('BODY','\n'.join(lifter.out).replace('SWAP','true' if swap else 'false'))
    assert lua.execute(code) is True

def test_unknown_actor_write_does_not_invent_typed_store():
    lifter=Lifter({}, {},'quest',False,'Example',NoRData());lifter.accessor_kinds=True
    lifter.locals.add('local_left');lifter.kinds['local_left']='thing'
    lifter.statement('Main','local_left = (CScriptThing *)unknown_handle;')
    assert lifter.todo and any('-- TODO(native):' in line for line in lifter.out)
    assert not any(line.lstrip().startswith('local_left =') for line in lifter.out)
    assert 'local_left' not in lifter.kinds
