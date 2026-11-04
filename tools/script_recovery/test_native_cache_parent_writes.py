from copy import deepcopy
import hashlib,struct
import pytest
from tools.script_recovery import native_cache_epoch_proof as P
from tools.script_recovery.native_register_snapshot_provenance import analyze

class Image:
 def __init__(self,raw,table=b''):self.regions={0x1000:raw,0x2000:table}
 def bytes_at(self,a,n):
  for base,data in self.regions.items():
   if base<=a and a+n<=base+len(data):return data[a-base:a-base+n]
  return None

def fixture(switch=False,*,frame_write=False,cache_escape=False,implicit_overwrite=False,copied_esp=False):
 raw=bytearray();labels={};fixups=[]
 def mark(name):labels[name]=0x1000+len(raw)
 def emit(code):raw.extend(bytes.fromhex(code))
 def jump(opcode,label):
  raw.append(opcode);fixups.append((len(raw),label));raw.append(0)
 emit('53 56 83 ec 10 89 ce');mark('pointerLoad');emit('8b 5e 04');mark('cacheStore');emit('89 5c 24 0c')
 if switch:
  emit('83 f8 01');jump(0x77,'default');mark('switch');emit('ff 24 85 00 20 00 00');mark('case0')
 mark('parentLoad0');emit('8b 46 14');mark('parentWrite0');emit('c6 40 54 01')
 if frame_write:mark('frameWrite');emit('c6 44 24 0c 00')
 if cache_escape:mark('escape');emit('8d 44 24 0c')
 if copied_esp:mark('escape');emit('89 e0')
 if implicit_overwrite:
  emit('83 c4 10');mark('call');raw.append(0xe8);raw.extend(struct.pack('<i',0x3000-(0x1000+len(raw)+4)));emit('83 ec 10')
 if switch:
  jump(0xeb,'reload');mark('case1');emit('8b 46 14');mark('parentWrite1');emit('c6 40 55 00');jump(0xeb,'reload');mark('default');emit('90')
 mark('reload');emit('8b 4c 24 0c');mark('tableRead');emit('8b 11 83 c4 10 5e 5b c3')
 for offset,label in fixups:
  delta=labels[label]-(0x1000+offset+1);assert -128<=delta<=127;raw[offset]=delta&255
 raw=bytes(raw);data=struct.pack('<II',labels['case0'],labels['case1']) if switch else b''
 row=dict(nativeSha256=hashlib.sha256(raw).hexdigest(),abiContract=dict(ordinaryMsvcCalleeSaved=True,normalChildStackBalanceAssumed=True,callerFramePreservedExceptDeclaredBuffers=True,declaredBuffersStayInBounds=True,engineReentryVerified=False,exceptionsVerified=False,heapVerified=False,declaredParentBoolFieldsDoNotAliasCallerFrame=True),pointerLoad=hex(labels['pointerLoad']),cacheStore=hex(labels['cacheStore']),pointerRead=hex(labels['reload']),tableRead=hex(labels['tableRead']),pointerRegister='ebx',ownerRegister='esi',tableRegister='edx',interfaceOffset=4,declaredBuffers={},declaredParentWrites={hex(labels['parentWrite0']):dict(memberOffset=0x14,fieldOffset=0x54,value=True)})
 if switch:
  row['branchTables']=[dict(site=hex(labels['switch']),tableBytes=data.hex())]
  row['declaredParentWrites'][hex(labels['parentWrite1'])]=dict(memberOffset=0x14,fieldOffset=0x55,value=False)
 return raw,row,Image(raw,data),labels

@pytest.mark.parametrize('switch',[False,True])
def test_only_explicit_parent_bool_spans_are_excluded_from_caller_frame(switch):
 raw,row,image,labels=fixture(switch);result=P.prove(raw,0x1000,row,{},analyze,image=image)
 assert result['cacheCoordinate']==result['readCoordinate']==12
 assert len(result['parentFieldWrites'])==(2 if switch else 1)
 assert result['abiContract']['heapVerified'] is False
 assert all(r['outsideCallerFrameAssumed'] and r['heapLifetimeVerified'] is False for r in result['parentFieldWrites'])
 assert 'assumed' in result['limits'] and 'not physical' in result['limits']

def test_false_store_requires_its_matching_explicit_bool_contract():
 raw,row,image,labels=fixture();raw=bytearray(raw);raw[labels['parentWrite0']-0x1000+3]=0;raw=bytes(raw)
 row['nativeSha256']=hashlib.sha256(raw).hexdigest();row['declaredParentWrites'][hex(labels['parentWrite0'])]['value']=False
 result=P.prove(raw,0x1000,row,{},analyze,image=Image(raw))
 assert result['parentFieldWrites'][0]['value'] is False

@pytest.mark.parametrize('fault',[
 'native_hash','write_inventory_type','write_descriptor_type','extra_descriptor_key','site_encoding','member_bool','member_unaligned',
 'field_bool','field_outside','value_not_bool','field_mismatch','parent_mismatch','missing_disjoint_premise','false_disjoint_premise',
 'unreviewed_parent_write','unused_parent_write','wrong_native_value','wrong_native_opcode','wrong_native_parent_load','extra_call_contract',
])
def test_malformed_or_unproved_parent_write_refused(fault):
 raw,row,image,labels=fixture();site=hex(labels['parentWrite0']);write=row['declaredParentWrites'][site]
 if fault=='native_hash':row['nativeSha256']='0'*64
 elif fault=='write_inventory_type':row['declaredParentWrites']=[]
 elif fault=='write_descriptor_type':row['declaredParentWrites'][site]=None
 elif fault=='extra_descriptor_key':write['trusted']=True
 elif fault=='site_encoding':row['declaredParentWrites']={'bad site':write}
 elif fault=='member_bool':write['memberOffset']=True
 elif fault=='member_unaligned':write['memberOffset']=0x15
 elif fault=='field_bool':write['fieldOffset']=True
 elif fault=='field_outside':write['fieldOffset']=4096
 elif fault=='value_not_bool':write['value']=1
 elif fault=='field_mismatch':write['fieldOffset']=0x55
 elif fault=='parent_mismatch':write['memberOffset']=0x18
 elif fault=='missing_disjoint_premise':row['abiContract'].pop('declaredParentBoolFieldsDoNotAliasCallerFrame')
 elif fault=='false_disjoint_premise':row['abiContract']['declaredParentBoolFieldsDoNotAliasCallerFrame']=False
 elif fault=='unreviewed_parent_write':row['declaredParentWrites']={hex(labels['parentWrite0']+1):write}
 elif fault=='unused_parent_write':row['declaredParentWrites'][hex(labels['cacheStore'])]=deepcopy(write)
 elif fault in {'wrong_native_value','wrong_native_opcode','wrong_native_parent_load'}:
  changed=bytearray(raw)
  if fault=='wrong_native_value':changed[labels['parentWrite0']-0x1000+3]=0
  elif fault=='wrong_native_opcode':changed[labels['parentWrite0']-0x1000]=0x80
  else:changed[labels['parentLoad0']-0x1000+2]=4
  raw=bytes(changed);row['nativeSha256']=hashlib.sha256(raw).hexdigest();image=Image(raw)
 contracts={labels['parentLoad0']:dict(purgeBytes=0)} if fault=='extra_call_contract' else {}
 with pytest.raises(ValueError):P.prove(raw,0x1000,row,contracts,analyze,image=image)

def test_default_row_still_refuses_non_frame_write_with_original_error():
 raw,row,image,_=fixture();row.pop('declaredParentWrites');row['abiContract'].pop('declaredParentBoolFieldsDoNotAliasCallerFrame')
 with pytest.raises(ValueError,match='^native cache epoch: non-ESP memory write in epoch$'):P.prove(raw,0x1000,row,{},analyze)

def test_partial_parent_inventory_cannot_hide_another_case_write():
 raw,row,image,labels=fixture(True);row['declaredParentWrites'].pop(hex(labels['parentWrite1']))
 with pytest.raises(ValueError,match='unreviewed non-ESP'):P.prove(raw,0x1000,row,{},analyze,image=image)

@pytest.mark.parametrize('launder',[False,True])
def test_parent_contract_does_not_allow_a_direct_frame_cache_overwrite(launder):
 raw,row,image,labels=fixture(frame_write=True)
 if launder:row['declaredParentWrites'][hex(labels['frameWrite'])]=dict(memberOffset=0x14,fieldOffset=12,value=False)
 with pytest.raises(ValueError,match='direct write overlaps cache'):P.prove(raw,0x1000,row,{},analyze,image=image)

@pytest.mark.parametrize('declared',[False,True])
def test_parent_contract_does_not_allow_an_escape_overlapping_the_cache(declared):
 raw,row,image,labels=fixture(cache_escape=True)
 if declared:row['declaredBuffers'][hex(labels['escape'])]=dict(coordinate=12,bytes=1)
 with pytest.raises(ValueError):P.prove(raw,0x1000,row,{},analyze,image=image)

def test_parent_contract_does_not_allow_a_call_return_address_to_overwrite_cache():
 raw,row,image,labels=fixture(implicit_overwrite=True)
 with pytest.raises(ValueError,match='implicit stack write overlaps cache'):P.prove(raw,0x1000,row,{labels['call']:dict(purgeBytes=0)},analyze,image=image)

@pytest.mark.parametrize('declared',[False,True])
def test_parent_contract_does_not_allow_unbounded_or_overlapping_copied_esp(declared):
 raw,row,image,labels=fixture(copied_esp=True)
 if declared:row['declaredBuffers'][hex(labels['escape'])]=dict(coordinate=0,bytes=16)
 with pytest.raises(ValueError):P.prove(raw,0x1000,row,{},analyze,image=image)

@pytest.mark.parametrize('fault',['missing_table','wrong_table','missing_image'])
def test_switch_edges_require_original_checked_table(fault):
 raw,row,image,_=fixture(True)
 if fault=='missing_table':row.pop('branchTables')
 elif fault=='wrong_table':row['branchTables'][0]['tableBytes']='00'*8
 else:image=None
 with pytest.raises(ValueError):P.prove(raw,0x1000,row,{},analyze,image=image)
