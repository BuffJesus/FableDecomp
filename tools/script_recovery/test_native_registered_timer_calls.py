import copy,json,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter,_text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_registered_timer_calls import recover
FN=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json').native('0x00d3e2e0')
assert FN is not None
SOURCE=FN['decompile']
MEMORY=RData()
class ChangedMemory:
 def __init__(self,patches):self.patches=patches
 def bytes_at(self,address,length):
  raw=MEMORY.bytes_at(address,length)
  if raw is None:return None
  out=bytearray(raw)
  for at,data in self.patches.items():
   for k,value in enumerate(data):
    if address<=at+k<address+length:out[at+k-address]=value
  return bytes(out)
def run(source=SOURCE,fn=FN,memory=MEMORY):return recover(source,fn,_text_order_sites(source,fn),memory)
def test_native_initial_and_retry_timer_ids():
 source,evidence=run();assert len(evidence)==2
 assert [e['site'] for e in evidence]==['0x00D3EC57','0x00D3FC05']
 assert all(e['new']=='iStack_250' and e['slot']==-0x250 for e in evidence)
 assert source.count('iStack_250,iVar7')==2
@pytest.mark.parametrize('addition',[
 '\n iStack_250 = 0;\n',
 '\n iStack_250 = iStack_254;\n',
])
def test_mutable_result_name_does_not_supply_identity(addition):
 source=SOURCE+addition;out,evidence=run(source);assert out==source and evidence==[]
def test_unknown_ftol_preservation_refuses_both_calls():
 out,evidence=run(memory=ChangedMemory({0xbfea70:b'\x90'}));assert out==SOURCE and evidence==[]
def test_wrong_global_receiver_refuses_only_that_window():
 # MOV EBP,[GSI] in initial-race setup now loads a different global.
 out,evidence=run(memory=ChangedMemory({0xd3ec3d:(0x143e8fc).to_bytes(4,'little')}))
 assert [e['site'] for e in evidence]==['0x00D3FC05']
def test_wrong_call_depth_refuses_only_that_window():
 fn=copy.deepcopy(FN)
 for site in fn['indirectCalls']:
  if site['site']=='0x00D3EC57':site['depth']+=4
 out,evidence=run(fn=fn);assert [e['site'] for e in evidence]==['0x00D3FC05']
def test_indexed_result_store_does_not_supply_stack_identity():
 # RegisterTimer's plain [ESP+20] result store gains an unknown index.
 out,evidence=run(memory=ChangedMemory({0xd3e50a:b'\x54'}))
 assert out==SOURCE and evidence==[]
def test_ambiguous_call_order_leaves_source_unchanged():
 fn=copy.deepcopy(FN);fn['callOrder']=fn['callOrder'][:-1]
 out,evidence=run(fn=fn);assert out==SOURCE and evidence==[]
def test_complex_printed_operand_remains_unrecovered():
 source=SOURCE.replace('iStack_258,iVar7','(iStack_258 + 1),iVar7')
 out,evidence=run(source);assert out==source and evidence==[]
