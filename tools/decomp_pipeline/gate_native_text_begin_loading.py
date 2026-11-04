"""Fresh native text/group payload parity; no installed asset dependency."""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,subprocess,sys
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_text_begin_loading_sources import SOURCES,ADDRESS
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
def main():
 b=R/'rebuild/build/native-text-begin-loading/00/9c/009c80f0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES]
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeTextBeginLoading_test.cpp','NativeTextBeginLoadingGlobals_test.cpp')]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE}|{R/'tools/decomp_pipeline'/name for name in ('native_text_begin_loading_oracle.py','native_text_begin_loading_fixture.py')}
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
 exe=link(objects,'probe');positive=run(exe);assert not positive.returncode and 'NATIVE_TEXT_BEGIN_LOADING_PASS cases=128 cycles=2' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr)
 (b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));linked=verify(b,objects,native,b/'Host0.cpp');coff=Coff(Path(source_objects[str(BEGIN)]));roots=[]
 for prefix,address in [('?BeginLoading@CDataBankGroupEntry',0x9c80f0),('?BeginLoading@CDataBankTextEntry',0x9c8340)]:
  symbol=next(n for n in coff.symbols if n.startswith(prefix));sec,off=coff.symbols[symbol];body,rels=coff.sections[sec];assert not off and len(body)==296 and len(rels)==12;roots.append(dict(name='begin_'+hex(address),source=BEGIN,symbol=symbol,address=address,size=296,instructions=109,relocations=12,flags=['/nologo','/c','/O2','/MT','/Gy']))
 priority_source=R/'rebuild/src/compiled/01/39/NGameText_LoadingPriority_01396314.cpp';priority=Coff(Path(source_objects[str(priority_source)]));sec,off=priority.symbols['?g_NativeTextLoadingPriority_01396314@@3W4EThreadedFilePriority@@A'];data,rels=priority.sections[sec];assert not off and not rels and data==read_virtual_data(EXE.read_bytes(),0x1396314,4)==struct.pack('<I',3)
 import pefile
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 pe=pefile.PE(str(exe));assert pe.get_data(mapping['?g_NativeTextLoadingPriority_01396314@@3W4EThreadedFilePriority@@A']-pe.OPTIONAL_HEADER.ImageBase,4)==data
 strict=native_exact_roots.run(__file__,roots,ADDRESS,'native-text-begin-loading-exact.json',['128 full native automatic-allocation loads,4 original/recreated variants,and12 fault controls.'],'Native BeginLoading roots with24 independently pinned call/data relocations. No renderer orhistorical rootcount claim.')
 composition=dict(objects={str(p):sha(p) for p in objects},source_objects=source_objects,asset=str(b/'resource.big'),asset_sha256=asset_hash);(b/'composition.json').write_text(json.dumps(composition,indent=2));oracle=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_begin_loading_oracle.py'),str(b)],capture_output=True,text=True,timeout=60);assert not oracle.returncode,oracle.stdout+oracle.stderr;print(oracle.stdout.strip(),flush=True)
 text=BEGIN.read_text();controls=[]
 for kind in ['Group','Text']:
  start=text.index('void NGameText::CDataBank'+kind+'Entry::BeginLoading');end=text.find('\nvoid NGameText::',start+1);body=text[start:end if end>=0 else len(text)]
  specs=[('adopt-reference','pointer.Release();pointer.data_=entry;','pointer.Reset(entry);'),('resource-list','bank.AddResource(*entry);',';'),('existing-owner','if(P'+kind+'.IsNull())','if(true)'),('priority',',g_NativeTextLoadingPriority_01396314)',',THREADED_FILE_PRIORITY_LOW)'),('length','bank.GetEntryDataSize(index)','bank.GetEntryDataSize(index)+1'),('offset','BeginReadDataAutoAlloc(0,','BeginReadDataAutoAlloc(1,')]
  for label,old,new in specs:
   assert body.count(old)==1;name=kind.lower()+'-'+label;obj=compile(BEGIN,name,text=text.replace(body,body.replace(old,new)));fault=run(link([obj if str(p)==source_objects[str(BEGIN)] else p for p in objects],name));assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,(name,fault.returncode,fault.stderr);controls.append(dict(name=name,code=fault.returncode))
 again=run(exe);assert not again.returncode and again.stdout==positive.stdout;assert sha(b/'resource.big')==asset_hash
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),providers=len(profiles),cases=128,cycles=2,text_cases=64,group_cases=64,original_variants=4,original_runs=8,exact=strict,priority_initial=3,priority_data_exact=True,controls=controls,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},source_objects=source_objects,exe_sha256=sha(exe),fixture=fixture,boundary='Native BeginLoading through real file worker,automatic read-buffer allocation,main allocator/30 size classes,resource construction/list adoption,typed decode,close/reopen andzero tracked allocation shutdown. No allocator substitute orinstalled dependency. Preserve stale ResourceSize after released readbuffer;native class-owned allocator chunks retained until process exit. Renderer,eviction,allocation failures andengine shutdown registration outside this gate.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-begin-loading.json').write_text(json.dumps(report,indent=2));print('NATIVE_TEXT_BEGIN_LOADING_GATE_PASS cases=128 cycles=2 variants=4 controls=12 exact=2')
if __name__=='__main__':main()
