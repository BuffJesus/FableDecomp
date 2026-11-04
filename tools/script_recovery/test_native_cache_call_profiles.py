from copy import deepcopy
import hashlib,struct
import pytest
from tools.script_recovery import native_cache_call_contracts as P

class Image:
 def __init__(self,regions):self.regions=regions
 def bytes_at(self,a,n):
  for base,data in self.regions.items():
   if base<=a and a+n<=base+len(data):return data[a-base:a-base+n]
  return None
def fixture(*,budget=False,alias='eax',offset=8):
 factory=b'\x8d\x4e'+bytes([offset])+bytes.fromhex('c70600700000')
 factory+=b'\xe8'+struct.pack('<i',0x4000-(0x3000+len(factory)+5))
 ctor=bytes.fromhex('89c88b4c2404c70000500000c20400') if alias=='eax' else bytes.fromhex('568bf1c706005000005ec20400')
 if budget:
  raw=b'\x6a\x01\xe8'+struct.pack('<i',0x6000-0x1007)+b'\xc3';call=0x1002
  body=b'\x90'*5000+bytes.fromhex('c20400')
  record=dict(site=hex(call),instructionBytes=raw[2:7].hex(),family='direct',receiverValueEquivalenceProved=False)
 else:
  raw=bytes.fromhex('565589ce8d6e088b45006a018bcdff900c0100005d5ec3');call=0x100e
  body=bytes.fromhex('c20400')+b'\xcc'*13
  record=dict(site=hex(call),instructionBytes=raw[14:20].hex(),family='constructor_member_profile',receiverValueEquivalenceProved=True,vtable='0x5000',slot='0x10c')
 record.update(target='0x6000',calleePrefix=body[:16].hex(),purgeBytes=4)
 row=dict(memberProfile=dict(offset=offset,vtable='0x5000',witnesses={'factory':dict(address='0x3000',bytes=factory.hex()),'constructor':dict(address='0x4000',bytes=ctor.hex())}),cacheProof=dict(interfaceOffset=4),callContracts=[record],boundedReturnRegions={})
 if alias=='eax':row['memberProfile']['constructorReceiverAlias']='eax'
 if budget:
  row['boundedReturnRegions']={'0x6000':[dict(start='0x6000',endExclusive=hex(0x6000+len(body)),sha256=hashlib.sha256(body).hexdigest())]}
  row['boundedReturnInstructionBudgets']={'0x6000':6000}
 image=Image({0x1000:raw,0x3000:factory,0x4000:ctor,0x510c:struct.pack('<I',0x6000),0x6000:body})
 return raw,row,image,call

def resolve(raw,row,image):return P.resolve(image,raw,0x1000,row,lambda target:1)

@pytest.mark.parametrize('alias',['eax','esi'])
def test_small_member_offset_and_constructor_receiver_profiles(alias):
 raw,row,image,call=fixture(alias=alias);contracts,reports=resolve(raw,row,image)
 assert contracts[call]['purgeBytes']==4 and reports[0]['receiverValueEquivalenceProved'] is True
 assert reports[0]['currentVtableLifetimeVerified'] is False
 assert reports[0].get('constructorReceiverAlias')==('eax' if alias=='eax' else None)

def test_explicit_large_budget_is_recorded_and_keeps_stack_purge_evidence():
 raw,row,image,call=fixture(budget=True);contracts,reports=resolve(raw,row,image)
 assert contracts[call]['purgeBytes']==4 and reports[0]['boundedReturnInstructionBudget']==6000

@pytest.mark.parametrize('claim',[None,False])
def test_direct_profiles_preserve_not_applicable_or_unproved_receiver_claim(claim):
 raw,row,image,call=fixture(budget=True);row['callContracts'][0]['receiverValueEquivalenceProved']=claim
 assert resolve(raw,row,image)[1][0]['receiverValueEquivalenceProved'] is claim

def test_direct_call_cannot_claim_an_unproved_table_receiver_equality():
 raw,row,image,call=fixture(budget=True);row['callContracts'][0]['receiverValueEquivalenceProved']=True
 with pytest.raises(ValueError,match='unproved direct-call receiver equality'):resolve(raw,row,image)

@pytest.mark.parametrize('fault',[
 'implicit_new_alias','wrong_alias','wrong_entry_alias','wrong_vtable','wrong_factory_target','factory_receiver_clobber',
 'member_interface_overlap','unaligned_offset','false_receiver_equivalence','stale_call_bytes','wrong_purge',
 'missing_budget','budget_bool','budget_float','budget_too_large','budget_noncanonical','budget_unknown','budget_unused',
])
def test_unproved_profiles_or_undeclared_budget_refused(fault):
 use_budget=fault.startswith('budget_') or fault=='missing_budget'
 raw,row,image,call=fixture(budget=use_budget)
 profile=row['memberProfile']
 if fault=='implicit_new_alias':profile.pop('constructorReceiverAlias')
 elif fault=='wrong_alias':profile['constructorReceiverAlias']='edx'
 elif fault in ['wrong_entry_alias','wrong_vtable']:
  data=bytearray(image.regions[0x4000])
  if fault=='wrong_entry_alias':data[1]=0xd0
  else:data[8]=4
  image.regions[0x4000]=bytes(data);profile['witnesses']['constructor']['bytes']=bytes(data).hex()
 elif fault=='wrong_factory_target':
  data=bytearray(image.regions[0x3000]);data[-4:]=struct.pack('<i',0x4001-(0x3000+len(data)))
  image.regions[0x3000]=bytes(data);profile['witnesses']['factory']['bytes']=bytes(data).hex()
 elif fault=='factory_receiver_clobber':
  data=image.regions[0x3000][:-5]+bytes.fromhex('31c9');data+=b'\xe8'+struct.pack('<i',0x4000-(0x3000+len(data)+5))
  image.regions[0x3000]=data;profile['witnesses']['factory']['bytes']=data.hex()
 elif fault=='member_interface_overlap':profile['offset']=4
 elif fault=='unaligned_offset':profile['offset']=9
 elif fault=='false_receiver_equivalence':row['callContracts'][0]['receiverValueEquivalenceProved']=False
 elif fault=='stale_call_bytes':row['callContracts'][0]['instructionBytes']='90'
 elif fault=='wrong_purge':row['callContracts'][0]['purgeBytes']=0
 elif fault=='missing_budget':row.pop('boundedReturnInstructionBudgets')
 elif fault=='budget_bool':row['boundedReturnInstructionBudgets']['0x6000']=True
 elif fault=='budget_float':row['boundedReturnInstructionBudgets']['0x6000']=6000.0
 elif fault=='budget_too_large':row['boundedReturnInstructionBudgets']['0x6000']=65537
 elif fault=='budget_noncanonical':row['boundedReturnInstructionBudgets']={'0x06000':6000}
 elif fault=='budget_unknown':row['boundedReturnInstructionBudgets']={'0x7000':6000}
 elif fault=='budget_unused':
  row['boundedReturnRegions']['0x6100']=deepcopy(row['boundedReturnRegions']['0x6000']);row['boundedReturnInstructionBudgets']['0x6100']=6000
 with pytest.raises(ValueError):resolve(raw,row,image)
