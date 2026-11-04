"""Fresh native text/group payload parity; no installed asset dependency."""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,subprocess,sys
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_text_payload_sources import SOURCES
from native_retail_readonly_verify import verify
import gate_native_bank_shared_worker as W
import native_exact_roots
from native_text_resource_fixture import create_archive
import capstone
R=Path(__file__).resolve().parents[2]
GROUP=R/'rebuild/src/compiled/00/9c/NGameText_CGroupEntry_ReadFromDataStream_009c8470.cpp'
TEXT=R/'rebuild/src/compiled/00/9c/NGameText_CTextEntry_ReadFromDataStream_009c8de0.cpp'
TAGS=R/'rebuild/src/compiled/00/9c/NGameText_TagVectors_009caf60.cpp'
LONGS=R/'rebuild/src/compiled/00/44/STLLongVector_Resize_004428f0.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
BUILD=R/'rebuild/src/compiled/00/9c/NGameText_DataBankEntry_BuildResources_009c8700.cpp'
LISTS=[R/'rebuild/src/compiled'/p for p in ('00/9f/CResource_ListRemove_009fbfb0.cpp','00/9f/CResource_ListInsert_009fbf90.cpp','00/9f/CResourceList_AddResource_009fbfe0.cpp','00/9f/CResourceList_RemoveResource_009fc020.cpp')]
def main():
 b=R/'rebuild/build/native-text-resource-build/00/9c/009c8700'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES]+[(p,[]) for p in [BUILD]+LISTS]
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeTextResourceBuild_test.cpp','NativeRetailReadOnlyGlobals_test.cpp')]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE}|{R/'tools/decomp_pipeline'/name for name in ('native_text_resource_build_oracle.py','native_text_resource_fixture.py')}
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'));pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
 for p in seen:
  if p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
  elif p.is_relative_to(R/'rebuild/tests/integration') and p not in fixtures:q=inc/p.relative_to(R/'rebuild/tests/integration')
  else:continue
  q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];source_objects={};native=set()
 def compile(source,label,flags=(),text=None,extra_include=None):
  if text is None and source in LISTS[2:]:text=source.read_text()+'\nvoid (CResourceList::*g_KeepResourceMethod'+source.stem+')(CResource&)=&CResourceList::'+('AddResource' if source==LISTS[2] else 'RemoveResource')+';\n'
  p=b/(label+'.cpp');p.write_text(text if text is not None else source.read_text());obj=p.with_suffix('.obj');options=['/I'+str(extra_include)] if extra_include else [];run=V.cl(['/nologo','/c','/O2','/MT','/Gy',*flags,*options,'/I'+str(inc),'/Fo'+str(obj),str(p)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr,source=str(source),flags=list(flags))));assert not run.returncode,run.stdout+run.stderr;return obj
 def link(items,label,win=True):
  exe=b/(label+'.exe');libs=['kernel32.lib',str(W.WINMM)] if win else [];run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/include:?resize@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@_STL@@@_STL@@QAEXI@Z','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items),*libs],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;return exe
 def run(exe):return subprocess.run([str(exe),str(b/'resource.big')],cwd=b,capture_output=True,text=True,timeout=30)
 for i,(source,flags) in enumerate(profiles):
  obj=compile(source,'native%d'%i,flags);objects.append(obj);native.add(obj.name);source_objects[str(source)]=str(obj)
  if (i+1)%60==0:print('NATIVE_TEXT_PROVIDERS',i+1,flush=True)
 fixture=create_archive(b/'resource.big');asset_hash=sha(b/'resource.big')
 for i,source in enumerate(fixtures):objects.append(compile(source,'Host%d'%i))
 exe=link(objects,'probe');positive=run(exe);assert not positive.returncode and 'NATIVE_TEXT_RESOURCE_BUILD_PASS cases=512 text=256 groups=256' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr)
 (b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));linked=verify(b,objects,native,b/'Host0.cpp');grades=[]
 c=Coff(Path(source_objects[str(BUILD)]))
 for prefix,address in [('?BuildGroup@',0x9c8700),('?BuildText@',0x9c8fb0)]:
  symbol=next(n for n in c.symbols if n.startswith(prefix));sec,off=c.symbols[symbol];body,rels=c.sections[sec];score=compare(body,read_virtual_data(EXE.read_bytes(),address,190),tuple(rels),retail_relocations('%08x'%address,190)).to_dict();assert not off and score['near_match'] and score['complete_decode'];grades.append(dict(symbol=symbol,address=hex(address),bytes=len(body),score=score))
 exact=[]
 for source,prefix,address,size in [(LISTS[0],'?ListRemove@',0x9fbfb0,47),(LISTS[1],'?ListInsert@',0x9fbf90,26),(LISTS[2],'?AddResource@CResourceList',0x9fbfe0,49),(LISTS[3],'?RemoveResource@CResourceList',0x9fc020,80)]:
  coff=Coff(Path(source_objects[str(source)]));symbol=next(n for n in coff.symbols if n.startswith(prefix));sec,off=coff.symbols[symbol];body,rels=coff.sections[sec];assert not off and not rels and body==read_virtual_data(EXE.read_bytes(),address,size);exact.append(dict(source=str(source),symbol=symbol,address=hex(address),bytes=size))
 composition=dict(objects={str(p):sha(p) for p in objects},source_objects=source_objects,asset=str(b/'resource.big'),asset_sha256=asset_hash);(b/'composition.json').write_text(json.dumps(composition,indent=2));oracle=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_resource_build_oracle.py'),str(b)],capture_output=True,text=True,timeout=60);assert not oracle.returncode,oracle.stdout+oracle.stderr;print(oracle.stdout.strip(),flush=True)
 text=BUILD.read_text();controls=[]
 for label,old,new in [('group-touch','PGroup->TouchResource();',';'),('text-touch','PText->TouchResource();',';'),('group-decode','PGroup->ReadFromDataStream(stream);',';'),('text-decode','PText->ReadFromDataStream(stream);',';'),('group-release','PGroup->PFileData.ReleaseData();',';'),('text-release','PText->PFileData.ReleaseData();',';')]:
  assert text.count(old)==1;obj=compile(BUILD,label,text=text.replace(old,new));fault=run(link([obj if str(p)==source_objects[str(BUILD)] else p for p in objects],label));assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,(label,fault.returncode,fault.stderr);controls.append(dict(name=label,code=fault.returncode))
 again=run(exe);assert not again.returncode and again.stdout==positive.stdout;assert sha(b/'resource.big')==asset_hash
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),providers=len(profiles),cases=512,text_cases=256,group_cases=256,original_variants=4,original_runs=8,near=grades,exact=exact,controls=controls,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},source_objects=source_objects,exe_sha256=sha(exe),fixture=fixture,boundary='Real queued caller-buffer reads, native build/decode, listed/unlisted resources, retained/final data references, prewait/no-prewait, repeated builds, unload owner reset andresource-list removal. No BeginLoading,auto-allocation,renderer orinstalled dependency. Pending reads observed before build;precise worker completion order not asserted.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-resource-build.json').write_text(json.dumps(report,indent=2));print('NATIVE_TEXT_RESOURCE_BUILD_GATE_PASS cases=512 variants=4 controls=6 exact=4 near=2')
if __name__=='__main__':main()
