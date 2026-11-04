from pathlib import Path
import copy,hashlib,json,sys
import pytest
R=Path(__file__).resolve().parents[2]
from tools.script_recovery.native_inline_thing_health_predicate import prove
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
read=lambda p:json.loads(p.read_text());F=Path(__file__).resolve().parent/'fixtures/witch_inline_health'
@pytest.fixture(autouse=True)
def no_retail(monkeypatch):monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def frame_inputs(changes=None):
 phase=read(F/'phase.json');fn=phase['fn'];parts=[(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(F/'image_parts.json')];original=FixtureImage(parts);a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=bytearray(original.bytes_at(a,b-a));assert hashlib.sha256(raw).hexdigest()==phase['nativeSha256']
 for site,data in (changes or {}).items():raw[site-a:site-a+len(data)]=data
 parts=[(p,q) for p,q in parts if p!=a]+[(a,bytes(raw))];image=FixtureImage(parts);depths={int(a,16):v for a,v in phase['depths'].items()};projected=dict(fn,**{key:[dict(c,depth=depths[int(c['site'],16)]) for c in fn[key]] for key in ['calls','indirectCalls']});frame=prove_frame(bytes(raw),projected,image,entity_vtable=int(phase['entityVtable'],16),scope_start=0xece7ff,scope_end=b,frame_depth=184,mask_offset=0,this_definition=0xece7f9,actor_definition=0xece7fc,actor_profile='entry-edi');return frame,image
def run(frame,image,**kw):
 args=dict(positive_exit=0xecf0f9,false_exit=0xecf178);args.update(kw);return prove(frame,0xecf089,image,**args)
def test_original_inline_cleanup_graph_and_zero_origins():
 frame,image=frame_inputs();result=run(frame,image);assert len(result['nodes'])==27 and result['objectOffset']==132 and result['controlOffset']==140 and result['frameDepth']==184 and result['relation']=='ordered-greater' and result['unordered'] is False
 assert result['zeroDefinitions']==['0xece7ff','0xeceed3','0xecf176'] and result['callback']=='0xecf0c8' and result['delete']=='0xecf0d3' and result['baseCleanup']=='0xecf0f0'
@pytest.mark.parametrize('fault',['mask','yes','no','control-field','zero-compare','header-vtable','decrement','control-reload','count-compare','callback-data','callback-slot','delete-pointer','delete-push','delete-target','stack-restore','object-address','clear-data-field','clear-data-register','clear-control-field','base-target','flag-consumer','false-exit','zero-origin'])
def test_original_instruction_mutations_reject(fault):
 mutations={'mask':(0xecf099,'40'),'yes':(0xecf09d,'00'),'no':(0xecf0a1,'c0'),'control-field':(0xecf0a5,'90'),'zero-compare':(0xecf0aa,'c6'),'header-vtable':(0xecf0b2,'908c2301'),'decrement':(0xecf0b9,'00'),'control-reload':(0xecf0bd,'90'),'count-compare':(0xecf0c2,'30'),'callback-data':(0xecf0c7,'0c'),'callback-slot':(0xecf0ca,'08'),'delete-pointer':(0xecf0ce,'90'),'delete-push':(0xecf0d2,'50'),'delete-target':(0xecf0d4,'e8f8d2ff'),'stack-restore':(0xecf0da,'08'),'object-address':(0xecf0de,'88'),'clear-data-field':(0xecf0e5,'90'),'clear-data-register':(0xecf0e3,'9c'),'clear-control-field':(0xecf0ec,'90'),'base-target':(0xecf0f1,'efb1acff'),'flag-consumer':(0xecf0f6,'c0'),'false-exit':(0xecf0f8,'7e'),'zero-origin':(0xeceed3,'33db')}
 site,data=mutations[fault]
 with pytest.raises((ValueError,AssertionError)):frame,image=frame_inputs({site:bytes.fromhex(data)});run(frame,image)
@pytest.mark.parametrize('fault',['callback-purge','delete-purge','base-purge','object-bound','extra-entry','wrong-zero-register','wrong-thing-table'])
def test_wrong_frame_or_profile_contract_rejects(fault):
 frame,image=frame_inputs();kw={}
 if fault.endswith('purge'):
  site={'callback-purge':'0xecf0c8','delete-purge':'0xecf0d3','base-purge':'0xecf0f0'}[fault];frame['contracts'][site]['derivedNormalPurge']=4
 elif fault=='object-bound':frame['depths'][frame['by'][0xecf08f]]=140
 elif fault=='extra-entry':frame['preds'][frame['by'][0xecf0a2]].append(frame['by'][0xece7ff])
 elif fault=='wrong-zero-register':kw['zero_register']='edi'
 else:kw['thing_vtable']=0x1238c90
 with pytest.raises(ValueError):run(frame,image,**kw)

