"""Fresh bank constructor parity and observable initialization ordering."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,importlib.util,json,re,struct
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
SPECS=[
 ('bank','00/9c/CBankFile_CBankFile_009cd480.cpp','??0CBankFile@@QAE@XZ',0x9cd480,402),
 ('async','00/9d/CBankFileAsync_CBankFileAsync_009d5f80.cpp','??0CBankFileAsync@@QAE@XZ',0x9d5f80,224),
 ('packed','00/a6/CPackedUIntArray_CPackedUIntArray_00a629c0.cpp','??0CPackedUIntArray@@QAE@XZ',0xa629c0,16),
 ('wide','00/99/CWideString_Constructor_0099aed0.cpp','??0CWideString@@QAE@XZ',0x99aed0,15),
 ('char','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','??0CCharString@@QAE@XZ',0x99e4b0,15),
 ('cache','00/a6/CChecksumCacheFile_CChecksumCacheFile_00a60c90.cpp','??0CChecksumCacheFile@@QAE@J@Z',0xa60c90,73),
 ('base','00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClassNonCopyable@@QAE@XZ',0x99a2f0,9)]
ADDRESS={symbol:address for name,source,symbol,address,size in SPECS}
ADDRESS.update({'??_7CBankFile@@6B@':0x129b54c,'??_7CBankFileAsync@@6B@':0x129b8d4,'??_7CBaseClassNonCopyable@@6B@':0x129a7c4,'_malloc':0xbfea0e,'__imp__InitializeCriticalSection@4':0x143fee4,'?g_CWideStringInstanceCount_013BCA20@@3HA':0x13bca20,'?g_CCharStringInstanceCount_013BD800@@3HA':0x13bd800})
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--async-source',type=Path,default=R/'rebuild/src/compiled'/SPECS[1][1])
 ap.add_argument('--prior-source',type=Path)
 ap.add_argument('--output',type=Path,default=R/'rebuild/compile-gate/native-bank-initialization.json')
 args=ap.parse_args()
 build=R/'rebuild/build/native-bank-initialization/00/9d/009d5f80'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 sources=[R/'rebuild/src/compiled'/s[1] for s in SPECS];sources[1]=args.async_source.resolve()
 test=R/'rebuild/tests/integration/NativeBankInitialization_test.py'
 inputs=set()
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(p,name);closure(q)
 for p in sources:closure(p)
 inputs.update([Path(__file__),test,EXE,V.VC/'bin/cl.exe',V.VC/'bin/c1xx.dll',V.VC/'bin/c2.dll'])
 inputs.update((V.VC/'include').rglob('*.h'))
 inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'))
 if args.prior_source:inputs.add(args.prior_source.resolve())
 pins={str(p):sha(p) for p in sorted(inputs)}
 for p in inputs:
  if p.is_relative_to(R/'rebuild/include'):
   q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 for p in [Path(__file__),test,*[R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py')]]:
  q=build/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 def compile(name,text):
  p=src/(name+'.cpp');p.write_text(text);obj=build/(name+'.obj')
  result=V.cl(['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(inc),'/Fo'+str(obj),str(p)],env,build)
  (build/(name+'.compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2));assert result.returncode==0,result.stdout+result.stderr
  for line in result.stdout.splitlines():
   if 'including file:' in line.lower():
    q=Path(line.split(':',2)[-1].strip()).resolve();assert q.is_relative_to(build) or q.is_relative_to(V.VC/'include'),q
  return obj
 def patch(obj,index,exact):
  name,source,symbol,address,size=SPECS[index];c=Coff(obj);section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert kind in (6,20);value=ADDRESS[target]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0)
   struct.pack_into('<I',patched,site,value&0xffffffff)
  if exact:assert len(body)==size and bytes(patched)==read_virtual_data(EXE.read_bytes(),address,size),symbol
  return bytes(patched),dict(symbol=symbol,address=address,bytes=len(body),relocations=[dict(site=s,symbol=n,kind=k) for s,(n,k) in rels.items()])
 objects=[];candidate={};native={};records=[]
 for i,(name,source,symbol,address,size) in enumerate(SPECS):
  obj=compile(name,sources[i].read_text());objects.append(obj)
  candidate[address],record=patch(obj,i,True);native[address]=read_virtual_data(EXE.read_bytes(),address,size);records.append(record)
 spec=importlib.util.spec_from_file_location('bank_initialization_test',test);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 cases=module.compare(candidate,native);assert cases==72
 controls=[]
 mutations=[('bank-wrong-alignment',0,'Alignment = 1;','Alignment = 2;'),('bank-wrong-format',0,'BankFormatVersion = 0x73;','BankFormatVersion = 0x72;'),('async-noncached-default',1,'NonCached = false;','NonCached = true;'),('async-disabled-default',1,'DisableAsync = false;','DisableAsync = true;'),('async-stale-restart',1,'LastRestartingData = 0;','LastRestartingData = reinterpret_cast<CBankFileAsyncData*>(1);'),('packed-nonzero-bias',2,'Bias(0)','Bias(1)'),('wide-skip-counter',3,'++g_CWideStringInstanceCount_013BCA20;',';'),('char-skip-counter',4,'++g_CCharStringInstanceCount_013BD800;',';'),('cache-wrong-buffer',5,'(requestedBufferSize + 8) & ~8','requestedBufferSize + 8')]
 for name,index,old,new in mutations:
  text=sources[index].read_text();assert text.count(old)==1,(name,text.count(old));controls.append((name,index,text.replace(old,new)))
 early=sources[1].read_text().replace('CBankFileAsync::CBankFileAsync()','CBankFileAsync::CBankFileAsync() : LastRestartingData(0)').replace('    LastRestartingData = 0;\n','')
 controls.append(('async-early-restart-store',1,early))
 if args.prior_source:controls.append(('prior-early-restart-store',1,args.prior_source.read_text()))
 for name,index,text in controls:
  obj=compile(name,text);body,record=patch(obj,index,False);changed=dict(candidate);changed[SPECS[index][3]]=body
  try:module.compare(changed,native)
  except AssertionError as error:detail=str(error)
  else:raise AssertionError(('undetected control',name))
  (build/(name+'.control.json')).write_text(json.dumps(dict(detected=True,detail=detail),indent=2))
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),cases=cases,roots=records,controls=[name for name,index,text in controls],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},python_provenance_sha256={str(p):sha(p) for p in (build/'python-provenance').rglob('*.py')},new_roots=0,boundaries=['Original and rebuilt constructors and five actual helper bodies; exact bytes and named relocation targets checked.','Full64KiB object/heap snapshots at every allocation and critical-section boundary, final arena/global counters, nonvolatile registers, return value and stack cleanup compared across72 cases. Independent tree/list header links, allocation sizes/guards, defaults and counter-wrap checks.','malloc and InitializeCriticalSection are deterministic emulated boundaries. No host Windows bank construction/destruction, allocation-failure, full bank startup or new root credit claim.'])
 (build/'receipt.json').write_text(json.dumps(report,indent=2)+'\n');args.output.write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_INITIALIZATION_PASS cases=72 roots=7 controls='+str(len(controls)));print(build/'receipt.json')

if __name__=='__main__':main()
