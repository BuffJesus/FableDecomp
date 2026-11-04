from types import SimpleNamespace
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_signed_field_steps import recover
BODY=bytes.fromhex('8bf1 8b9664010000 42 8bc2 83f812 899664010000 7c01 c3 c3')
C='''void Main(void *this)
{
 int next;
 next = *(int *)(this + 0x164) + 1;
 *(int *)(this + 0x164) = next;
 if (next < 0x12) {
   USE(next);
 }
}
'''
LUA='''function Main(quest)
 local next
 next = quest:GetStateInt("Counter") + 1
 quest:SetStateInt("Counter", next)
 if next < 0x12 then return next end
 return next
end
'''
class Data:
 def __init__(self,body):self.body=body
 def bytes_at(self,address,length):
  assert address==0x1000 and length==len(self.body)
  return self.body
def apply(c=C,lua=LUA,body=BODY,fields=None,entity=False):
 fn=dict(address='0x1000',bodyEndExclusive=hex(0x1000+len(body)),decompile=c)
 spec=SimpleNamespace(entity=entity,self_fields={0x164:('Counter','Int')} if fields is None else fields)
 return recover(lua,fn,spec,Data(body))

@pytest.mark.parametrize('counter',[-2147483648,-2147483647,-1,0,16,17,18,2147483646,2147483647,2147483648,4294967294,4294967295])
def test_signed_increment_value_and_store(counter):
 source,evidence=apply();assert len(evidence)==1
 lua=LuaRuntime();lua.execute(source);stored=[]
 q=lua.table_from({'GetStateInt':lambda _,key:counter,'SetStateInt':lambda _,key,value:stored.append((key,value))})
 expected=(counter+1+2147483648)%4294967296-2147483648
 assert lua.globals().Main(q)==expected and stored==[('Counter',expected)]

@pytest.mark.parametrize('before,after',[
 ('int next;','uint next;'),('int next;','long next;'),('int next;','int *next;'),
 ('int next;',''),('int next;','int next;\n int next;'),
 ('*(int *)(this + 0x164) + 1','*(uint *)(this + 0x164) + 1'),
 (' + 1;',' + 2;'),(' + 1;',' - 1;'),
 ('if (next < 0x12)','if (next <= 0x12)'),('if (next < 0x12)','if (next < 0x80000000)'),
 ('*(int *)(this + 0x164) = next;','*(int *)(this + 0x168) = next;'),
 ('*(int *)(this + 0x164) = next;','CALL();\n *(int *)(this + 0x164) = next;'),
 ('if (next < 0x12)','CALL();\n if (next < 0x12)'),
 ('this + 0x164','*(int *)(this + 0x14) + 0x164'),
])
def test_unestablished_c_semantics_decline(before,after):
 assert apply(c=C.replace(before,after))==(LUA,[])

@pytest.mark.parametrize('fields',[{}, {0x164:('Counter','Bool')},{0x168:('Counter','Int')}])
def test_field_metadata_is_required(fields):assert apply(fields=fields)==(LUA,[])

@pytest.mark.parametrize('before,after',[
 ('7c01','7201'),('42','4a'),('83f812','83f813'),
 ('899664010000','898664010000'),('899664010000','899668010000'),
 ('8bf1','8bf2'),('8b9664010000','668b9664010000'),
])
def test_native_signed_sequence_and_receiver_required(before,after):
 body=bytes.fromhex(BODY.hex().replace(before,after,1));assert apply(body=body)==(LUA,[])

@pytest.mark.parametrize('before,after',[
 ('"Counter"','"Other"'),('next = quest:GetStateInt','other = quest:GetStateInt'),
 (' + 1\n',' + 2\n'),('next < 0x12','next < 0x13'),
 ('quest:SetStateInt("Counter", next)','CALL()\n quest:SetStateInt("Counter", next)'),
])
def test_lifted_pair_must_match(before,after):
 source=LUA.replace(before,after);assert apply(lua=source)==(source,[])

def test_repeated_lua_pair_is_ambiguous():assert apply(lua=LUA+'\n'+LUA)==(LUA+'\n'+LUA,[])
def test_idempotence():
 first,evidence=apply();assert evidence;assert apply(lua=first)==(first,[])
def test_comment_only_c_is_not_evidence():assert apply(c='/*\n'+C+'\n*/')==(LUA,[])
def test_comment_mentions_do_not_change_local_type():
 source,evidence=apply(c=C+'\n// uint next;\n');assert source!=LUA and evidence
def test_wrong_entity_namespace_is_rejected():assert apply(entity=True)==(LUA,[])
def test_entity_self_namespace_supported():
 source=LUA.replace('quest:GetStateInt','__native_entity_state:GetStateInt').replace('quest:SetStateInt','__native_entity_state:SetStateInt')
 updated,evidence=apply(lua=source,entity=True);assert updated!=source and len(evidence)==1
def test_unreachable_clobber_does_not_erase_slow_receiver():
 # Fast path overwrites ESI and returns; the branch to slow retains entry ECX.
 prefix=bytes.fromhex('8bf1 85c0 7405 8b742404 c3')
 source,evidence=apply(body=prefix+BODY[2:]);assert source!=LUA and len(evidence)==1
def test_receiver_merge_with_clobbered_path_declines():
 # One path arrives at the shared load with ESI=EDX, so the meet is unknown.
 prefix=bytes.fromhex('8bf1 85c0 7402 8bf2')
 assert apply(body=prefix+BODY[2:])==(LUA,[])
def test_partial_receiver_write_declines():
 assert apply(body=BODY[:2]+bytes.fromhex('66befeca')+BODY[2:])==(LUA,[])
def test_segment_override_is_not_a_script_field():
 assert apply(body=BODY[:2]+b'\x64'+BODY[2:])==(LUA,[])
def test_incremented_receiver_register_is_not_the_same_member():
 body=bytes.fromhex('8bf1 8bb664010000 46 8bc6 83f812 89b664010000 7c01 c3 c3')
 assert apply(body=body)==(LUA,[])
def test_compare_copy_cannot_overwrite_the_receiver():
 body=bytes.fromhex('8bf1 8b9664010000 42 8bf2 83fe12 899664010000 7c01 c3 c3')
 assert apply(body=body)==(LUA,[])
def test_duplicate_reachable_native_sites_decline():
 block=BODY[2:];prefix=bytes.fromhex('8bf1 85c0 74')+bytes([len(block)])
 assert apply(body=prefix+block+block)==(LUA,[])
def test_incomplete_native_span_declines():assert apply(body=BODY[:-1])==(LUA,[])
def test_evidence_retains_original_signed_instruction_bytes():
 _,rows=apply();row=rows[0]
 assert row['native']['bytes']==BODY[2:-2].hex()
 assert row['field']=='Counter' and row['offset']=='0x164' and row['bound']==18
