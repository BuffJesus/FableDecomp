"""Fresh native text/group payload parity; no installed asset dependency."""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,subprocess,sys
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_text_public_getter_sources import SOURCES,ADDRESS
CTORS=Path(__file__).resolve().parents[2]/'rebuild/src/compiled/00/9c/NGameText_ResourceConstructors_009c8060.cpp'
from native_retail_readonly_verify import verify
import gate_native_bank_shared_worker as W
import native_exact_roots
from native_text_begin_loading_fixture import create_archive
import capstone
import struct
R=Path(__file__).resolve().parents[2]
GROUP=R/'rebuild/src/compiled/00/9c/NGameText_CGroupEntry_ReadFromDataStream_009c8470.cpp'
TEXT=R/'rebuild/src/compiled/00/9c/NGameText_CTextEntry_ReadFromDataStream_009c8de0.cpp'
TAGS=R/'rebuild/src/compiled/00/9c/NGameText_TagVectors_009caf60.cpp'
LONGS=R/'rebuild/src/compiled/00/44/STLLongVector_Resize_004428f0.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
BUILD=R/'rebuild/src/compiled/00/9c/NGameText_DataBankEntry_BuildResources_009c8700.cpp'
LISTS=[R/'rebuild/src/compiled'/p for p in ('00/9f/CResource_ListRemove_009fbfb0.cpp','00/9f/CResource_ListInsert_009fbf90.cpp','00/9f/CResourceList_AddResource_009fbfe0.cpp','00/9f/CResourceList_RemoveResource_009fc020.cpp')]
BEGIN=R/'rebuild/src/compiled/00/9c/NGameText_DataBankEntry_BeginLoading_009c80f0.cpp'
GETGROUP=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_GetPGroupEntry_009c8890.cpp'
GETTEXT=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_GetPTextEntry_009c9070.cpp'
PUBLICBEGIN=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_BeginLoading_009c8630.cpp'
def main():
 b=R/'rebuild/build/native-text-public-getters/00/9c/009c8890'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES]
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeTextPublicGetters_test.cpp','NativeTextBeginLoadingGlobals_test.cpp')]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE}|{R/'tools/decomp_pipeline'/name for name in ('native_text_public_getter_oracle.py','native_text_begin_loading_fixture.py')}
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
  exe=b/(label+'.exe');libs=['kernel32.lib',str(W.WINMM)] if win else [];args=['/nologo','/OPT:REF','/include:?resize@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@_STL@@@_STL@@QAEXI@Z','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items),*libs];rsp=b/(label+'.rsp');rsp.write_text('\n'.join(chr(34)+arg+chr(34) for arg in args));run=subprocess.run([str(V.VC/'bin/link.exe'),'@'+str(rsp)],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;return exe
 def run(exe):return subprocess.run([str(exe),str(b/'resource.big')],cwd=b,capture_output=True,text=True,timeout=30)
 for i,(source,flags) in enumerate(profiles):
  obj=compile(source,'native%d'%i,flags);objects.append(obj);native.add(obj.name);source_objects[str(source)]=str(obj)
  if (i+1)%60==0:print('NATIVE_TEXT_PROVIDERS',i+1,flush=True)
 fixture=create_archive(b/'resource.big');asset_hash=sha(b/'resource.big');assert not (b/'boot.ini').exists()
 for i,source in enumerate(fixtures):objects.append(compile(source,'Host%d'%i))
 exe=link(objects,'probe');positive=run(exe);assert not positive.returncode and 'NATIVE_TEXT_PUBLIC_GETTERS_PASS cases=256 cycles=4 retained_close=8' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr)
 (b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));linked=verify(b,objects,native,b/'Host0.cpp');roots=[]
 for source,prefix,address,size,instructions in [(GETGROUP,'?GetPGroupEntry@',0x9c8890,247,92),(GETTEXT,'?GetPTextEntry@',0x9c9070,245,92),(PUBLICBEGIN,'?BeginLoadingGroup@',0x9c8220,44,16),(PUBLICBEGIN,'?BeginLoadingText@',0x9c8630,43,16)]:
  coff=Coff(Path(source_objects[str(source)]));symbol=next(n for n in coff.symbols if n.startswith(prefix));sec,off=coff.symbols[symbol];body,rels=coff.sections[sec];assert not off and len(body)==size;instructions=len(list(capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(read_virtual_data(EXE.read_bytes(),address,size),address)));roots.append(dict(name='public_'+hex(address),source=source,symbol=symbol,address=address,size=size,instructions=instructions,relocations=len(rels),flags=['/nologo','/c','/O2','/MT','/Gy']))
 strict=native_exact_roots.run(__file__,roots,ADDRESS,'native-text-public-getters-exact.json',['256 full loads through4 public getter modes,8 handles retained across bankClose,4 original/recreated variants,and12 controls.'],'Four original getter/public BeginLoading bodies with26 independently pinned call relocations;no renderer orhistorical rootcount claim.')
 composition=dict(objects={str(p):sha(p) for p in objects},source_objects=source_objects,asset=str(b/'resource.big'),asset_sha256=asset_hash);(b/'composition.json').write_text(json.dumps(composition,indent=2));oracle=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_public_getter_oracle.py'),str(b)],capture_output=True,text=True,timeout=60);assert not oracle.returncode,oracle.stdout+oracle.stderr;print(oracle.stdout.strip(),flush=True)
 controls=[]
 for kind,source in [('Group',GETGROUP),('Text',GETTEXT)]:
  text=source.read_text();var=kind.lower();field='P'+kind;specs=[('loaded-state','if ('+var+'->IsLoaded() || load)','if (load)'),('lazy-start','BeginLoading'+kind+'(index);',';'),('decode',var+'->Build'+kind+'();',';'),('return-handle','return '+var+'->'+field+';','return CIVCountedPointer<const C'+kind+'Entry>();'),('return-reference','return '+var+'->'+field+';','{CIVCountedPointer<const C'+kind+'Entry> result;result.data_='+var+'->'+field+'.data_;return result;}'),('touch',var+'->'+field+'->TouchResource();',';')]
  for label,old,new in specs:
   assert old in text;name=kind.lower()+'-'+label;obj=compile(source,name,text=text.replace(old,new));fault=run(link([obj if str(p)==source_objects[str(source)] else p for p in objects],name));assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,(name,fault.returncode,fault.stderr);controls.append(dict(name=name,code=fault.returncode))
 again=run(exe);assert not again.returncode and again.stdout==positive.stdout;assert sha(b/'resource.big')==asset_hash
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),providers=len(profiles),cases=256,cycles=4,text_cases=128,group_cases=128,retained_close_handles=8,original_variants=4,original_runs=8,exact=strict,controls=controls,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},source_objects=source_objects,exe_sha256=sha(exe),fixture=fixture,boundary='Native public getters,4 lazy/forced/public-begin modes,wrong-type null returns,repeated identity/refcounts,later-frame touches,andretained resources across bankClose withbank alive until final handle release. Full native automatic buffer allocation/file worker/decoder/close cleanup. Disposal queue empty;eviction,renderer,andbank destruction withlive handles outside scope. Native allocator chunk retention until process exit.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-public-getters.json').write_text(json.dumps(report,indent=2));print('NATIVE_TEXT_PUBLIC_GETTERS_GATE_PASS cases=256 cycles=4 retained_close=8 variants=4 controls=12 exact=4')
if __name__=='__main__':main()
