import copy,sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.convert_quest_unit import UnitConverter,_text_order_sites
from tools.script_recovery.native_animation_byte_calls import recover
@pytest.fixture(scope='module')
def native():
 c=UnitConverter(R/'refs/script_recovery/guild_training/translation_unit_typed.json')
 return c.native('0x00D68810'),c.rdata
def test_actual_native_caller_and_source_match(native):
 fn,data=native;source=fn['decompile']
 result,evidence=recover(source,fn,_text_order_sites(source,fn,include_unslotted=True),data)
 assert len(evidence)==1 and evidence[0]['site']=='0x00D68AAC'
 assert result.count('PlayAnimationWithNativeArgument5')==1
 assert 'DAT_01375748' not in result
 assert 'appuStack_20' in result and '(int)&local_24' in result
 assert recover(result,fn,_text_order_sites(result,fn,include_unslotted=True),data)==(result,[])
@pytest.mark.parametrize('change',[
 ('DAT_01375748','DAT_01375749'),
 ('(uint)DAT_01375748','0'),
 ('(uint)DAT_01375748','rawUnknown'),
 (',0,0,0\n                     ,1,',',0,0,0\n                     ,unknown,'),
 (',0,0);',',0,0,0);'),
 ('CScriptGameResourceObjectScriptedThingBase::\n          _PlayAnimation','OtherClass::\n          _PlayAnimation'),
])
def test_unknown_operand_or_class_not_rewritten(native,change):
 fn,data=native;source=fn['decompile'].replace('\r','')
 assert change[0] in source,change
 source=source.replace(*change)
 assert recover(source,fn,_text_order_sites(source,fn,include_unslotted=True),data)==(source,[])
@pytest.mark.parametrize('mode',('missing_order','wrong_target','wrong_call_order'))
def test_unproven_site_mapping_unchanged(native,mode):
 original,data=native;fn=copy.deepcopy(original);source=fn['decompile']
 if mode=='missing_order':fn['callOrder']=[]
 elif mode=='wrong_target':
  for call in fn['calls']:
   if call['site']=='0x00D68AAC':call['target']='0x007E73D1'
 else:fn['callOrder']=fn['callOrder'][:-1]
 assert recover(source,fn,_text_order_sites(source,fn,include_unslotted=True),data)==(source,[])
@pytest.mark.parametrize('mode',('helper','load_address','flag4','caller_truncated'))
def test_native_bytes_must_match_source_operands(native,mode):
 fn,data=native;source=fn['decompile']
 class Changed:
  def bytes_at(self,address,size):
   raw=data.bytes_at(address,size)
   if mode=='helper' and address==0x7e73d0:return b'\x90'*size
   if address==int(fn['address'],16):
    if mode=='caller_truncated':return raw[:-1]
    raw=bytearray(raw)
    if mode=='load_address':raw[0xd68a94-address]=0x49
    if mode=='flag4':raw[0xd68aa4-address]=0
    return bytes(raw)
   return raw
 assert recover(source,fn,_text_order_sites(source,fn,include_unslotted=True),Changed())==(source,[])
