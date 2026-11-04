"""Fresh native BIGB open, queued reads and shared-file Close on a synthetic asset."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,subprocess,sys
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_retail_readonly_sources import SOURCES
from native_retail_readonly_fixture import create_archive
from native_retail_readonly_verify import verify
import gate_native_bank_shared_worker as W
R=Path(__file__).resolve().parents[2]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 b=R/'rebuild/build/native-retail-readonly-integration/00/9d/009d56c0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set()
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeRetailReadOnlyHost_test.cpp','NativeRetailReadOnlyInspect_test.cpp','NativeRetailReadOnlyGlobals_test.cpp')]
 sources=[R/'rebuild/src/compiled'/source for source,flags in SOURCES]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in sources+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE,R/'tools/parse_bigb.py'}
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
 for p in seen:
  if p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
  elif p.is_relative_to(R/'rebuild/tests/integration') and p not in fixtures:q=inc/p.relative_to(R/'rebuild/tests/integration')
  else:continue
  q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];native=set();source_objects={}
 def compile(source,label,flags=()):
  p=b/(label+'.cpp');p.write_bytes(source.read_bytes());obj=p.with_suffix('.obj');run=V.cl(['/nologo','/c','/O2','/MT','/Gy',*flags,'/I'+str(inc),'/Fo'+str(obj),str(p)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr,source=str(source),flags=list(flags))));assert not run.returncode,run.stdout+run.stderr;return obj
 for i,((path,flags),source) in enumerate(zip(SOURCES,sources)):
  obj=compile(source,'native%d'%i,flags);objects.append(obj);native.add(obj.name);source_objects[str(source)]=str(obj)
  if (i+1)%60==0:print('NATIVE_RETAIL_PROVIDERS',i+1,flush=True)
 for i,source in enumerate(fixtures):obj=compile(source,'Host%d'%i);objects.append(obj)
 run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib',str(W.WINMM)],cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 asset=b/'synthetic-retail.big';assert asset.resolve().is_relative_to(b.resolve());case=create_archive(asset);(b/'expected.json').write_text(json.dumps(case,indent=2));run=subprocess.run([str(b/'probe.exe'),str(asset)],cwd=b,capture_output=True,text=True,timeout=30);(b/'run.json').write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,(run.returncode,run.stdout[-1000:],run.stderr)
 def check(output):
  lines=output.splitlines()
  for label,rows in case['expected'].items():assert sorted(x for x in lines if x.startswith(label+'\t'))==sorted(rows),label
  assert lines[-1]=='COUNTERS\t0\t2'
 check(run.stdout);assert sha(asset)==case['asset_sha256'];linked=verify(b,objects,native,b/'Host0.cpp');grades=[]
 for filename,symbol,address,size in [('CBankFile_ReadEntryList_009cfbc0.cpp','?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z',0x9cfbc0,1969),('CBankFile_OpenReadOnly_009d06f0.cpp','?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z',0x9d06f0,385),('CBankFileAsync_OpenReadOnly_009d56c0.cpp','?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z',0x9d56c0,348)]:
  c=Coff(Path(next(obj for source,obj in source_objects.items() if Path(source).name==filename)));section,offset=c.symbols[symbol];body,rels=c.sections[section];assert not offset;score=compare(body,read_virtual_data(EXE.read_bytes(),address,size),tuple(rels),retail_relocations('%08x'%address,size)).to_dict();assert score['near_match'] and score['complete_decode'];grades.append(dict(symbol=symbol,address=hex(address),bytes=len(body),score=score))
 # A byte-location fault leaves native completion/guards intact; the independent oracle must reject it.
 source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_SetupRead_009d5970.cpp';text=source.read_text();old='desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset;';assert text.count(old)==2;mutant=b/'offset-control-source.cpp';mutant.write_text(text.replace(old,'desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset+1;'));obj=compile(mutant,'offset-control');controlled=[obj if str(p)==source_objects[str(source)] else p for p in objects];exe=b/'offset-control.exe';run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,controlled),'kernel32.lib',str(W.WINMM)],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;fault=subprocess.run([str(exe),str(asset)],cwd=b,capture_output=True,text=True,timeout=30);assert not fault.returncode,(fault.returncode,fault.stderr)
 rejected=False
 try:check(fault.stdout)
 except AssertionError:rejected=True
 assert rejected and [x for x in fault.stdout.splitlines() if x.startswith('ENTRY\t')]==[x for x in json.loads((b/'run.json').read_text())['stdout'].splitlines() if x.startswith('ENTRY\t')]
 assert sha(asset)==case['asset_sha256']
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 windows=sum(len(case['expected'][label]) for label in ('ASYNC','BATCH','SHARED'));report=dict(status='PASS',build=str(b),banks=case['banks'],entries=case['entries'],windows=windows,providers=len(sources),linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],controls=['offset-plus-one rejected byindependentqueued oracle;synchronousentryrows unchanged'],grades=grades,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},exe_sha256=sha(b/'probe.exe'),asset_sha256=case['asset_sha256'],asset_unchanged=True,boundary='Build-owned synthetic BIGB only. Genuine native disk/footer/entry parsing,global thread manager,async dispatcher/worker/wait/batch/shared Close andlifetime. Concrete schema-version getter controlled;allocator/pool/installer activation paths cold. Original dispatcher/parser permutations independently covered bysaved family receipts. No installed assets/game/save writes ornew historical count.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-retail-readonly-integration.json').write_text(json.dumps(report,indent=2));print('NATIVE_RETAIL_READONLY_INTEGRATION_PASS banks=%d entries=%d windows=%d providers=%d symbols=%d bodies=%d'%(case['banks'],case['entries'],windows,len(sources),linked['linked_symbols'],linked['unique_bodies']))
if __name__=='__main__':main()
