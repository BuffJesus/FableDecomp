from types import SimpleNamespace
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import native_signed_field_steps as MODULE
C='void Main(void *this)\n{\n *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + 1;\n}\n'
BODY=bytes.fromhex('8bf1 ff461c c3')
LUA='function Main(quest)\n quest:SetStateInt("Counter", quest:GetStateInt("Counter") + 1)\nend\n'
class Data:
    def __init__(self,body):self.body=body
    def bytes_at(self,address,length):assert address==0x1000 and length==len(self.body);return self.body
def apply(c=C,lua=LUA,body=BODY,fields=None,entity=False):
    fn=dict(address='0x1000',bodyEndExclusive=hex(0x1000+len(body)),decompile=c)
    spec=SimpleNamespace(entity=entity,self_fields={0x1c:('Counter','Int')} if fields is None else fields)
    return MODULE.recover_direct(lua,fn,spec,Data(body))
@pytest.mark.parametrize('counter',[-2147483648,-2147483647,-1,0,1,2,17,2147483646,2147483647,2147483648,4294967294,4294967295])
@pytest.mark.parametrize('entity',[False,True])
@pytest.mark.parametrize('cast',['(int)this','this'])
def test_signed_word_store_and_single_evaluation(counter,entity,cast):
    source=LUA.replace('quest:', '__native_entity_state:') if entity else LUA
    out,proof=apply(c=C.replace('(int)this',cast),lua=source,entity=entity);assert len(proof)==1 and proof[0]['kind']=='direct_memory_inc'
    vm=LuaRuntime();reads=[];writes=[];state=vm.table_from({'GetStateInt':lambda _,key:reads.append(key) or counter,'SetStateInt':lambda _,key,v:writes.append((key,v))})
    vm.globals().__native_entity_state=state;vm.execute(out);vm.globals().Main(state)
    assert reads==['Counter'] and writes==[('Counter',(counter+1+2147483648)%4294967296-2147483648)]
@pytest.mark.parametrize('old,new',[
    ('*(int *)','*(uint *)'),('*(int *)','*(undefined4 *)'),('*(int *)','*(long *)'),
    (' + 1;',' + 2;'),(' + 1;',' - 1;'),(' + 1;',' + sideEffect();'),
    (' = *(int *)((int)this + 0x1c)',' = *(int *)((int)this + 0x20)'),
    (' = *(int *)((int)this + 0x1c)',' = *(int *)((int)other + 0x1c)'),
])
def test_unsupported_or_ambiguous_c_declines(old,new):assert apply(c=C.replace(old,new))==(LUA,[])
@pytest.mark.parametrize('fields',[{}, {0x1c:('Counter','Bool')},{0x1c:('Counter','Float')},{0x20:('Counter','Int')},{0x1c:('Counter','Int'),0x20:('Counter','Int')}])
def test_missing_wrong_or_aliased_field_metadata_declines(fields):assert apply(fields=fields)==(LUA,[])
@pytest.mark.parametrize('body',[bytes.fromhex(s) for s in ['8bf2ff461cc3','8bf166ff461cc3','8bf1ff4620c3','8bf1ff4e1cc3','8bf1fe461cc3','8bf140c3','8bf1ff461cff461cc3','e800000000ff411cc3','8bf1ff461cc30f']])
def test_unestablished_native_increment_or_receiver_declines(body):assert apply(body=body)==(LUA,[])
@pytest.mark.parametrize('body',[bytes.fromhex(s) for s in ['ff411cc3','8bf1e800000000ff461cc3']])
def test_entry_ecx_and_saved_this_across_normal_child_call_supported(body):assert len(apply(body=body)[1])==1
@pytest.mark.parametrize('old,new',[
    ('"Counter", quest:GetStateInt','"Other", quest:GetStateInt'),('GetStateInt("Counter")','GetStateInt("Other")'),
    (' + 1)',' + 2)'),(' + 1)',' - 1)'),('quest:SetStateInt','other:SetStateInt'),
    ('quest:GetStateInt','other:GetStateInt'),('"Counter"','key'),
])
def test_noncorresponding_lua_declines(old,new):
    source=LUA.replace(old,new);assert apply(lua=source)==(source,[])
def test_duplicate_c_or_lua_statements_decline():
    line=C.splitlines()[2];assert apply(c=C.replace(line,line+'\n'+line))==(LUA,[])
    line=LUA.splitlines()[1];source=LUA.replace(line,line+'\n'+line);assert apply(lua=source)==(source,[])
@pytest.mark.parametrize('opaque',['--[[\n quest:SetStateInt("Counter", quest:GetStateInt("Counter") + 1)\n]]\n','local example = [[\n quest:SetStateInt("Counter", quest:GetStateInt("Counter") + 1)\n]]\n'])
def test_opaque_examples_are_preserved_and_not_counted(opaque):
    source=opaque+LUA;out,proof=apply(lua=source);assert out.startswith(opaque) and len(proof)==1
