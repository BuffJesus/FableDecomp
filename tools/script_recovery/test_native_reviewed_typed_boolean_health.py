from pathlib import Path
import copy,hashlib,json,math,struct,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R));from tools.script_recovery import native_reviewed_typed_boolean_health as MODULE
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
F=W/'fixtures/typed_boolean_health';read=lambda p:json.loads(p.read_text())
sha=lambda b:hashlib.sha256(b).hexdigest();serial=MODULE.serial

@pytest.fixture(autouse=True)
def no_retail(monkeypatch):
 from tools.script_recovery.lift_native_lua import RData
 monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))

def inputs():
 phase=read(F/'phase.json');row=read(F/'candidate_row.json')
 return dict(source=(F/'phase.lua').read_text(),todo=phase['todo'],calls=phase['calls'],fn=phase['fn'],row=row,unit=read(F/'candidate_unit.json'),owner=row['owner'],image=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read(F/'image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=read(F/'before.json'),after=read(F/'after.json'))

def run(d):return MODULE.recover(**d)
def repin(d):
 d['row']['sourceSha256']=sha(d['fn']['decompile'].encode());d['row']['phaseSha256']=sha(d['source'].encode())
 for name in ['calls','todo','before','after']:d['row'][name+'Sha256']=sha(serial(d[name]))

def test_opt_out_is_identity():
 assert MODULE.recover('text',['todo'],['call'],{},None,{},None,None,None,None,None)==('text',['todo'],['call'],[])

def test_exact_three_comparisons_only():
 d=inputs();out,todo,calls,reviews=run(d)
 assert out==d['source'].replace('if fVar14 <= 0.0 then','if not (fVar14 > 0.0) then')
 assert len(reviews)==7 and sum(r['changed'] for r in reviews)==3
 assert todo==d['todo'] and calls==d['calls']
 LuaRuntime().compile(out)

@pytest.mark.parametrize('key',['nativeSha256','sourceSha256','phaseSha256','callsSha256','todoSha256','beforeSha256','afterSha256'])
def test_stale_pin_rejects(key):
 d=inputs();d['row'][key]='0'*64
 with pytest.raises(ValueError):run(d)

@pytest.mark.parametrize('fault',['owner','slot','registered-address','registered-vtable','host-ABI','scope-site','scope-kind','scope-flag','scope-count','repair-count'])
def test_forged_certificate_rejects(fault):
 d=inputs()
 if fault=='owner':d['owner']='wrong'
 elif fault=='slot':d['row']['healthSlot']+=4
 elif fault=='registered-address':d['unit']['entities'][d['owner']]['functions']['Main']['address']='0x123'
 elif fault=='registered-vtable':d['unit']['entities'][d['owner']]['vtable']='0x123'
 elif fault=='host-ABI':d['manifest']=copy.deepcopy(d['manifest']);d['manifest']['GetHealth']['returnType']='int'
 elif fault=='scope-site':d['row']['scopes'][-1]['site']='0x0'
 elif fault=='scope-kind':d['row']['scopes'][-1]['kind']='unknown'
 elif fault=='scope-flag':d['row']['scopes'][-1]['flag']='not_the_flag'
 elif fault=='scope-count':d['row']['scopes'].pop()
 else:d['row']['repairCount']=2
 with pytest.raises(ValueError):run(d)

@pytest.mark.parametrize('fault',['lua-result','lua-actor','lua-true','lua-false','lua-inverse','lua-extra-query','AST-result','AST-actor','AST-true','AST-comparison','source-result','source-actor','source-type','source-comparison','call-inventory'])
def test_repinned_correspondence_forgery_rejects(fault):
 d=inputs()
 if fault.startswith('lua-'):
  if fault=='lua-result':d['source']=d['source'].replace('fVar14 = quest:GetHealth(uVar7)','forged = quest:GetHealth(uVar7)',1)
  elif fault=='lua-actor':d['source']=d['source'].replace('fVar14 = quest:GetHealth(uVar7)','fVar14 = quest:GetHealth(forged)',1)
  elif fault=='lua-true':d['source']=d['source'].replace('hb_stk_a8 = true','hb_stk_a8 = false',1)
  elif fault=='lua-false':d['source']=d['source'].replace('hb_stk_a8 = false','hb_stk_a8 = true',1)
  elif fault=='lua-inverse':d['source']=d['source'].replace('if fVar14 <= 0.0 then','if fVar14 < 0.0 then',1)
  else:d['source']+='\nfVar14 = quest:GetHealth(uVar7)\n'
 elif fault.startswith('AST-'):
  for key in ['before','after']:
   lines=d[key]
   if fault=='AST-result':index=next(n for n,s in enumerate(lines) if 'fVar14 = (float10)GSI->GetHealth(uVar7)' in s);lines[index]=lines[index].replace('fVar14','forged')
   elif fault=='AST-actor':index=next(n for n,s in enumerate(lines) if 'fVar14 = (float10)GSI->GetHealth(uVar7)' in s);lines[index]=lines[index].replace('(uVar7)','(forged)')
   elif fault=='AST-true':index=next(n for n,s in enumerate(lines) if 'hb_stk_a8 = true;' in s);lines[index]=lines[index].replace('true','false')
   else:index=next(n for n,s in enumerate(lines) if 'if (fVar14 <= (float10)0.0)' in s);lines[index]=lines[index].replace('<=','<')
 elif fault=='source-result':d['fn']['decompile']=d['fn']['decompile'].replace('fVar14 = (float10)','forged = (float10)',1)
 elif fault=='source-actor':d['fn']['decompile']=d['fn']['decompile'].replace('0x420))(uVar7)', '0x420))(forged)',1)
 elif fault=='source-type':d['fn']['decompile']=d['fn']['decompile'].replace('float10 fVar14;','int fVar14;')
 elif fault=='source-comparison':d['fn']['decompile']=d['fn']['decompile'].replace('if (fVar14 <= (float10)_DAT_0122dedc)','if (fVar14 < (float10)_DAT_0122dedc)',1)
 else:d['calls'].remove('GetHealth')
 repin(d)
 with pytest.raises(ValueError):run(d)

@pytest.mark.parametrize('index',range(7))
@pytest.mark.parametrize('fault',['mask','flag-value','cleanup-target','external-entry'])
def test_repinned_native_mutation_rejects(index,fault):
 d=inputs();row=d['row'];start=int(d['fn']['address'],16);raw=bytearray(d['image'].bytes_at(start,int(d['fn']['bodyEndExclusive'],16)-start));scope=row['scopes'][index];a=int(scope['native']['start'],16)
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw[a-start:int(scope['native']['endExclusive'],16)-start],a))
 if fault=='mask':raw[code[3].address-start+2]=0x40
 elif fault=='flag-value':raw[code[1].address-start+code[1].size-1]=0
 elif fault=='cleanup-target':raw[code[7].address-start+1]^=1
 else:
  # Redirect an existing entry conditional into the predicate interior.
  branch=0xecf843;raw[branch-start+2:branch-start+6]=struct.pack('<i',code[2].address-(branch+6))
 parts=read(F/'image_parts.json');parts[0]['bytes']=bytes(raw).hex();d['image']=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in parts]);row['nativeSha256']=sha(raw)
 with pytest.raises(ValueError):run(d)

BITS=[0,0x80000000,0x3f800000,0xbf800000,1,0x80000001,0x7f800000,0xff800000,0x7fc00000,0x7f800001,0xffc12345]
@pytest.mark.parametrize('bits',BITS)
@pytest.mark.parametrize('inverse',[False,True])
def test_actual_old_candidate_boolean_paths_and_nan_control(bits,inverse):
 value=struct.unpack('<f',struct.pack('<I',bits))[0];vm=LuaRuntime()
 old='local flag=true; if value <= 0.0 then flag=false end; return flag' if inverse else 'return 0.0 < value'
 new=old.replace('value <= 0.0','not (value > 0.0)')
 original=vm.eval('function(value) '+old+' end')(value);candidate=vm.eval('function(value) '+new+' end')(value)
 assert candidate==(value>0)
 assert (original!=candidate)==(inverse and math.isnan(value))
