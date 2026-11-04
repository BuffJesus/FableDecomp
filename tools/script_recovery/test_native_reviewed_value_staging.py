import copy
import hashlib
import json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_value_staging import classify
from tools.script_recovery.readable_lua import tokens

R=Path(__file__).resolve().parents[2]
F=R/'tools/script_recovery/fixtures/staging_liveness'
sha=lambda b:hashlib.sha256(b).hexdigest()

@pytest.fixture(scope='module')
def inputs():
 c=UnitConverter(R/'refs/script_recovery/sick_child/translation_unit_typed.json')
 source=(F/'woman_main.lua').read_text(encoding='utf8');row=json.loads((F/'woman_certificate.json').read_text(encoding='utf8'))
 return source,[p['diagnostic'] for p in row['reads']]+['label LAB_00ecfd04','label LAB_00ed0a5a'],c.native('0x00ecf7f0'),row,c.rdata

def test_nine_reads_are_classified_without_changing_executable_tokens(inputs):
 source,todo,fn,row,rdata=inputs
 out,remaining,proof=classify(source,todo,fn,row,rdata)
 sig=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 assert sig(out)==sig(source) and remaining==['label LAB_00ecfd04','label LAB_00ed0a5a']
 assert len(proof)==1 and len(proof[0]['reads'])==9
 assert out.count('--[[unresolved native value]]')==0 and out.count(' = nil')==source.count(' = nil')
 assert out.count('--[[reviewed native staging]]')==9
 for label in ['LAB_00ed0a5a','LAB_00ecfd04']:
  retained='-- '+label+': (native jump target)'
  assert retained in source and retained in out

def test_absent_certificate_leaves_all_diagnostics_visible(inputs):
 source,todo,fn,row,rdata=inputs
 assert classify(source,todo,fn,None,rdata)==(source,todo,[])

@pytest.mark.parametrize('key,value',[
 ('nativeSha256','0'*64),('sourceSha256','0'*64),('luaBodySha256','0'*64),
 ('frameBytes',224),('headerFrameOffset',28),('headerEntryOffset',-200),
 ('acceptedDispatchSealSha256','missing'),('acceptedSdkSealSha256','missing'),
 ('scopeLimit','No limitations'),('scopeLimit','unverified without calling convention'),
])
def test_changed_body_frame_or_evidence_rejects(inputs,key,value):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);row[key]=value
 with pytest.raises(ValueError):classify(source,todo,fn,row,rdata)

@pytest.mark.parametrize('name', ['compare','chars','resourceCtor','actorCtor','gsiCtor'])
@pytest.mark.parametrize('fault',['address','truncated','bytes'])
def test_helper_identity_extent_and_bytes_reject(inputs,name,fault):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);h=row['helpers'][name]
 if fault=='address':h['address']=hex(int(h['address'],16)+1)
 elif fault=='truncated':h['bytes']=h['bytes'][:-2]
 else:h['bytes']='90'+h['bytes'][2:]
 with pytest.raises(ValueError):classify(source,todo,fn,row,rdata)

@pytest.mark.parametrize('index',range(9))
def test_each_changed_native_witness_is_rejected(inputs,index):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);row['reads'][index]['sequence'][0]['bytes']='90'
 with pytest.raises(ValueError):classify(source,todo,fn,row,rdata)

@pytest.mark.parametrize('fault', ['site','source','region','alias','family','literal','multiplicity','duplicate','noncontiguous','method','cleanup','slot'])
def test_ambiguous_or_mismatched_binding_is_rejected(inputs,fault):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);todo=list(todo);fn=copy.deepcopy(fn)
 if fault=='site':row['reads'][0]['nativeSite']='0xed0011'
 elif fault=='source':row['reads'][0]['nativeSourceFragment']='invented field access'
 elif fault=='region':row['reads'][0]['luaRegion']='not present'
 elif fault=='alias':row['reads'][0]['alias']='another'
 elif fault=='family':row['reads'][0]['family']='unknown'
 elif fault=='literal':row['reads'][0]['literal']='EXPRESSION_WRONG'
 elif fault=='multiplicity':todo.append(row['reads'][0]['diagnostic'])
 elif fault=='duplicate':row['reads'][1]['nativeSite']=row['reads'][0]['nativeSite']
 elif fault=='noncontiguous':row['reads'][0]['sequence'].pop(1)
 elif fault=='method':row['reads'][1]['method']='AnotherGetter'
 elif fault=='cleanup':row['reads'][4]['luaCleanupRegion']='::LAB_00ed0a36::\nquest:PauseAllNonScriptedEntities(true)\n'
 else:
  call=next(c for c in fn['indirectCalls'] if int(c['site'],16)==0xed0190);call['slot']='0x11c'
 with pytest.raises(ValueError):classify(source,todo,fn,row,rdata)

@pytest.mark.parametrize('index',range(9))
def test_live_read_rejects_even_with_matching_updated_source_hash(inputs,index):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);part=row['reads'][index];alias=part['alias']
 old=alias+' = nil --[[unresolved native value]]\n';injection='                    quest:Observe('+alias+')\n'
 # Only the selected unique context is changed; update certificate phase/regions
 # so rejection must come from the actual alias-flow proof.
 region=part['luaRegion'];offset=source.index(region)+region.index(old)+len(old)
 for p in row['reads']:
  context=p['luaRegion'];begin=source.index(context)
  if begin<=offset<=begin+len(context):
   relative=offset-begin;p['luaRegion']=context[:relative]+injection+context[relative:]
 source=source[:offset]+injection+source[offset:]
 row['luaBodySha256']=sha(source.encode())
 with pytest.raises(ValueError,match='Lua staging liveness: live staging value read/escape'):classify(source,todo,fn,row,rdata)

class MutatedBytes:
 def __init__(self,base,address,data):self.base,self.address,self.data=base,address,data
 def bytes_at(self,address,size):
  raw=self.base.bytes_at(address,size)
  if raw is None:return raw
  out=bytearray(raw)
  for n,b in enumerate(self.data):
   at=self.address+n-address
   if 0<=at<size:out[at]=b
  return bytes(out)
 def string_at(self,address):return self.base.string_at(address)

@pytest.mark.parametrize('fault',['load_receiver','nonnull_guard','compare_signedness','text_accessor','resource_vptr','actor_vptr','gsi_vptr'])
def test_self_consistent_native_mutants_fail_semantic_guards(inputs,fault):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row)
 patches={'load_receiver':(0xed0011,b'\x31'),'nonnull_guard':(0xecffaa,b'\x84'),
 'compare_signedness':(0x411580,b'\x72'),'text_accessor':(0x99e4cd,b'\x01'),
 'resource_vptr':(0x7e72ac,b'\x50'),'actor_vptr':(0x4abe98,b'\x90'),'gsi_vptr':(0x6e776b,b'\x10')}
 address,data=patches[fault];rdata=MutatedBytes(rdata,address,data)
 row['nativeSha256']=sha(rdata.bytes_at(int(fn['address'],16),int(fn['bodyEndExclusive'],16)-int(fn['address'],16)))
 for h in row['helpers'].values():h['bytes']=rdata.bytes_at(int(h['address'],16),len(bytes.fromhex(h['bytes']))).hex()
 for p in row['reads']:
  for key in ['sequence','guard','tail']:
   for witness in p.get(key,[]):witness['bytes']=rdata.bytes_at(int(witness['site'],16),len(bytes.fromhex(witness['bytes']))).hex()
 with pytest.raises(ValueError):classify(source,todo,fn,row,rdata)

def test_first_rejected_speech_mapping_remains_rejected(inputs):
 source,todo,fn,row,rdata=inputs;row=copy.deepcopy(row);p=row['reads'][7]
 p['nativeSite']='0xed07c7';p['nativeCall']='0xed07df';p['literalAddress']='0x12f27ec';p['literal']=rdata.string_at(0x12f27ec)
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 p['sequence']=[dict(site=hex(i.address),bytes=i.bytes.hex()) for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(rdata.bytes_at(0xed07c2,0xed07e2-0xed07c2),0xed07c2)]
 with pytest.raises(ValueError,match='restored resource speech'):classify(source,todo,fn,row,rdata)
