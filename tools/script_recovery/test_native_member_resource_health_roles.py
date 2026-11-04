from pathlib import Path
import copy,json,struct
import pytest
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.native_member_resource_health_roles import prove_roles
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
P=Path(__file__).resolve().parent/'fixtures/member_resource_health_roles/packet.json'
PACKET=json.loads(P.read_text());FN=PACKET['function'];RAW=bytes.fromhex(PACKET['nativeBytes']);START=int(FN['address'],16)
PARTS=[(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in PACKET['imageParts']]
ENTRY=dict(FN,**{key:[dict(c,depth=PACKET['entryCallDepths'][c['site'].lower()]) for c in FN[key]] for key in ['calls','indirectCalls']})
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):
 from tools.script_recovery.lift_native_lua import RData
 monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))

def image(raw=RAW,parts=None):
 out=[]
 for a,b in PARTS if parts is None else parts:
  if a<=START and START+len(raw)<=a+len(b):b=b[:START-a]+raw+b[START-a+len(raw):]
  out.append((a,b))
 return FixtureImage(out)
def frame(raw=RAW):
 return prove_frame(raw,ENTRY,image(raw),entity_vtable=0x12f2404,scope_start=0xecf7ff,scope_end=int(FN['bodyEndExclusive'],16),frame_depth=228,mask_offset=0,this_definition=0xecf7f9,actor_definition=0xecf7fc,actor_profile='entry-edi',owner_register='ebp')
@pytest.fixture(scope='module')
def base():return frame()
def run(ctx,scope,img=None,**overrides):
 args=dict(this_definition=0xecf7f9,member_definition=0xecf849,member_offset=36,member_vtable=0x127094c)
 args.update(overrides)
 return prove_roles(ctx,int(scope['site'],16),scope['predicate'],img or image(),**args)

@pytest.mark.parametrize('scope',PACKET['scopes'])
def test_original_member_getter_query_and_stack_byte_roles(base,scope):
 result=run(base,scope)
 assert result['resourceKind']=='entity-member' and result['memberOffset']==36 and result['frameDepth']==228
 assert result['objectOffset']==scope['thingOffset'] and result['objectEntryOffset']==scope['thingEntryOffset']
 assert result['getter']==scope['getter'] and result['cleanup']==scope['cleanup']
 assert result['ownerRegister']=='ebp' and result['memberRegister']=='ebx'

@pytest.mark.parametrize('scope',PACKET['scopes'])
def test_exported_positive_ecxStack_is_not_used_as_a_stack_resource(base,scope):
 ctx=dict(base);ctx['calls']=copy.deepcopy(base['calls']);ctx['calls'][int(scope['getter'],16)]['ecxStack']=-3000
 assert run(ctx,scope)['memberOffset']==36

@pytest.mark.parametrize('fault',['owner-register','member-register','register-collision','member-offset','zero-offset','member-vtable','getter-slot','getter-target','health-slot','owner-definition','member-definition'])
def test_changed_profile_rejects(base,fault):
 kwargs={'owner-register':dict(owner_register='esi'),'member-register':dict(member_register='edi'),'register-collision':dict(member_register='ebp'),'member-offset':dict(member_offset=40),'zero-offset':dict(member_offset=0),'member-vtable':dict(member_vtable=0x1270950),'getter-slot':dict(getter_slot=0x34),'getter-target':dict(getter_target=0x7e7491),'health-slot':dict(health_slot=0x424),'owner-definition':dict(this_definition=0xecf7fc),'member-definition':dict(member_definition=0xecf84c)}[fault]
 with pytest.raises(ValueError):run(base,PACKET['scopes'][0],**kwargs)

@pytest.mark.parametrize('scope',PACKET['scopes'])
@pytest.mark.parametrize('fault',['mask','getter-slot','output-offset','external-predicate-entry'])
def test_actual_native_caller_or_predicate_mutations_reject(scope,fault):
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(RAW,START));by={i.address:n for n,i in enumerate(ins)};raw=bytearray(RAW)
 if fault=='mask':
  a=int(scope['predicate']['start'],16);code=list(decoder.disasm(RAW[a-START:int(scope['predicate']['endExclusive'],16)-START],a));raw[code[3].address-START+2]=0x40
 elif fault=='getter-slot':
  i=ins[by[int(scope['getter'],16)]];raw[i.address-START+i.size-1]=0x34
 elif fault=='output-offset':
  k=by[int(scope['getter'],16)];leas=[i for i in ins[max(0,k-8):k] if i.mnemonic=='lea' and i.operands[0].type==x.X86_OP_REG and i.operands[0].reg==x.X86_REG_EAX and i.operands[1].type==x.X86_OP_MEM and i.operands[1].mem.base==x.X86_REG_ESP];assert len(leas)==1
  i=leas[0];assert i.disp_size in [1,4];offset=i.address-START+i.disp_offset;raw[offset:offset+i.disp_size]=(scope['thingOffset']+4).to_bytes(i.disp_size,'little')
 else:
  target=int(scope['predicate']['start'],16)+6;raw[0xecf843-START+2:0xecf843-START+6]=struct.pack('<i',target-(0xecf843+6))
 with pytest.raises((AssertionError,ValueError)):run(frame(bytes(raw)),scope,image(bytes(raw)))

@pytest.mark.parametrize('fault',['predicate-flag','predicate-threshold','predicate-end','missing-purge','wrong-purge','predicate-depth','missing-reachability','external-edge','missing-threshold','missing-member-target'])
def test_corrupt_proof_or_missing_evidence_rejects(base,fault):
 scope=copy.deepcopy(PACKET['scopes'][0]);ctx=dict(base);ctx['depths']=dict(base['depths']);ctx['contracts']=copy.deepcopy(base['contracts']);ctx['preds']={k:list(v) for k,v in base['preds'].items()};ctx['reachable']=set(base['reachable']);img=image()
 if fault=='predicate-flag':scope['predicate']['flagOffset']+=1
 elif fault=='predicate-threshold':scope['predicate']['threshold']=1.0
 elif fault=='predicate-end':scope['predicate']['endExclusive']=hex(int(scope['predicate']['endExclusive'],16)-1)
 elif fault=='missing-purge':del ctx['contracts'][scope['getter']]
 elif fault=='wrong-purge':ctx['contracts'][scope['getter']]['derivedNormalPurge']=0
 elif fault=='predicate-depth':
  lo,hi=int(scope['predicate']['start'],16),int(scope['predicate']['endExclusive'],16)
  for k,i in enumerate(base['ins']):
   if lo<=i.address<hi:ctx['depths'][k]+=16
 elif fault=='missing-reachability':ctx['reachable'].remove(base['by'][int(scope['predicate']['start'],16)])
 elif fault=='external-edge':ctx['preds'][base['by'][int(scope['predicate']['start'],16)+6]].append(0)
 else:
  address=0x122dedc if fault=='missing-threshold' else 0x127097c
  class Missing:
   def bytes_at(self,a,n):return None if a<=address<a+n else img_base.bytes_at(a,n)
  img_base=img;img=Missing()
 with pytest.raises(ValueError):run(ctx,scope,img)

def test_old_export_depths_still_fail_registered_frame():
 with pytest.raises(AssertionError):prove_frame(RAW,FN,image(),entity_vtable=0x12f2404,scope_start=0xecf7ff,scope_end=int(FN['bodyEndExclusive'],16),frame_depth=228,mask_offset=0,this_definition=0xecf7f9,actor_definition=0xecf7fc,actor_profile='entry-edi',owner_register='ebp')
