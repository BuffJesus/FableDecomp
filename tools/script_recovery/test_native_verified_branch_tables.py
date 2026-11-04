from pathlib import Path
import importlib.util,struct
import pytest
from tools.script_recovery import native_verified_branch_tables as B
from tools.script_recovery import native_register_snapshot_provenance as P
START=0x1000;TABLE=0x2000
RAW=bytes.fromhex('83f8027719ff248500200000b801000000c3b802000000c3b803000000c3b8ffffffffc3')
TARGETS=(0x100c,0x1012,0x1018)
DATA=struct.pack('<III',*TARGETS)
class Image:
 def __init__(self,raw=RAW,data=DATA):self.regions={START:raw,TABLE:data}
 def bytes_at(self,a,n):
  for base,data in self.regions.items():
   if base<=a and a+n<=base+len(data):return data[a-base:a-base+n]
  return None
def row(data=DATA):return dict(site='0x1005',tableBytes=data.hex())
def test_checked_unsigned_switch_case_targets():
 proof=B.verify(RAW,START,Image(),[row()])
 assert proof['targets']=={0x1005:TARGETS}
 assert proof['tables'][0]['maximum']==2 and proof['tables'][0]['default']=='0x101e'

@pytest.mark.parametrize('fault',[
 'code_image','table_image','missing_case','extra_case','misaligned_case','outside_case','case_bypasses_compare',
 'signed_guard','inclusive_guard','wrong_compare_register','negative_bound','wrong_bound','wrong_scale','base_register',
 'not_dispatch','bad_site','bad_hex','extra_descriptor_key','duplicate_dispatch','empty_inventory','not_list',
 'direct_bypass','unreviewed_indirect','external_direct_jump',
])
def test_stale_or_unbounded_switch_refused(fault):
 raw=bytearray(RAW);data=DATA;rows=[row()]
 if fault=='code_image':raw[0]^=1
 elif fault=='table_image':data=b'\0'*12
 elif fault=='missing_case':data=DATA[:-4];rows=[row(data)]
 elif fault=='extra_case':data=DATA+DATA[:4];rows=[row(data)]
 elif fault=='misaligned_case':data=struct.pack('<III',0x100d,*TARGETS[1:]);rows=[row(data)]
 elif fault=='outside_case':data=struct.pack('<III',0x3000,*TARGETS[1:]);rows=[row(data)]
 elif fault=='case_bypasses_compare':data=struct.pack('<III',0x1003,*TARGETS[1:]);rows=[row(data)]
 elif fault=='signed_guard':raw[3]=0x7f
 elif fault=='inclusive_guard':raw[3]=0x73
 elif fault=='wrong_compare_register':raw[1]=0xfb
 elif fault=='negative_bound':raw[2]=0xff
 elif fault=='wrong_bound':raw[2]=3
 elif fault=='wrong_scale':raw[7]=0xc5
 elif fault=='base_register':raw[7]=0x80
 elif fault=='not_dispatch':rows[0]['site']='0x1000'
 elif fault=='bad_site':rows[0]['site']=True
 elif fault=='bad_hex':rows[0]['tableBytes']='bad hex'
 elif fault=='extra_descriptor_key':rows[0]['trusted']=True
 elif fault=='duplicate_dispatch':rows*=2
 elif fault=='empty_inventory':rows=[]
 elif fault=='not_list':rows=tuple(rows)
 elif fault=='direct_bypass':raw[12:17]=b'\xe9'+struct.pack('<i',0x1003-(0x100c+5))
 elif fault=='unreviewed_indirect':raw[12:17]=b'\xff\xe0\x90\x90\x90'
 elif fault=='external_direct_jump':raw[12:17]=b'\xe9'+struct.pack('<i',0x3000-(0x100c+5))
 image=Image(RAW if fault=='code_image' else bytes(raw),data)
 with pytest.raises(ValueError):B.verify(bytes(raw),START,image,rows)

def test_generated_table_bytes_do_not_accept_code_as_data():
 with pytest.raises(ValueError):B.verify(RAW,START,Image(),[dict(site='0x1005',tableBytes=RAW[:12].hex())])

def test_no_table_option_keeps_original_refusal():
 with pytest.raises(ValueError,match='indirect jump'):P.analyze(RAW,START)

def test_checked_switch_keeps_original_register_bits_on_every_case_edge():
 result=P.analyze(RAW,START,branch_tables=[row()],image=Image())
 assert result['verifiedBranchTables']['targets']=={0x1005:TARGETS}
 assert all(result['states'][a]['ecx']==('entry_this',) for a in [*TARGETS,0x101e])


@pytest.mark.parametrize('fault',['image_missing','bad_table','branch_into_compare_guard'])
def test_optional_switch_contract_is_not_a_bypass(fault):
 image=Image();raw=RAW
 if fault=='image_missing':image=None
 elif fault=='bad_table':image.regions[0x2000]=b'\0'*12
 else:
  image.regions[0x2000]=b'\x03\x10\0\0'+image.regions[0x2000][4:]
 with pytest.raises(ValueError):P.analyze(raw,START,branch_tables=[row()],image=image)



import struct
import pytest
from tools.script_recovery.native_verified_branch_tables import verify
CALL_ENTRY_RAW=bytes.fromhex('83f8027719ff248500200000b801000000c3b802000000c3b803000000c3b8ffffffffc3')
class CallEntryImage:
 def __init__(self,code,data):self.regions={0x1000:code,0x2000:data}
 def bytes_at(self,a,n):
  for base,data in self.regions.items():
   if base<=a and a+n<=base+len(data):return data[a-base:a-base+n]
  return None
@pytest.mark.parametrize('target,accepted',[
 (0x1008,False),(0x100a,False),(0x1006,False),(0x1012,False),
 (0x1005,True),(0x1011,True),(0x3000,True),
])
def test_explicit_call_entry_cannot_bypass_switch_guard_or_instruction_alignment(target,accepted):
 code=b'\xe8'+struct.pack('<i',target-0x1005)+CALL_ENTRY_RAW
 data=struct.pack('<III',0x1011,0x1017,0x101d)
 image=CallEntryImage(code,data);rows=[dict(site='0x100a',tableBytes=data.hex())]
 if accepted:assert verify(code,0x1000,image,rows)['targets']=={0x100a:(0x1011,0x1017,0x101d)}
 else:
  with pytest.raises(ValueError):verify(code,0x1000,image,rows)
