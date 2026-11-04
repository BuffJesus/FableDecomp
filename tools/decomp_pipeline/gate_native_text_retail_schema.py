"""Native retail schema106 metadata, decoded resources and retained-close ownership."""
from pathlib import Path
import sys
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
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
from native_text_retail_schema_fixture import create_archive
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
 b=R/'rebuild/build/native-text-retail-schema/00/9c/009c8890'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES]
 fixtures=[R/'rebuild/tests/integration/NativeTextRetailSchema_test.cpp',R/'rebuild/tests/integration/NativeTextBeginLoadingGlobals_test.cpp']
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),R/'tools/decomp_pipeline/native_text_retail_schema_fixture.py',EXE}|{R/'tools/decomp_pipeline'/name for name in ('native_text_public_getter_oracle.py','native_text_begin_loading_fixture.py')}
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
 exe=link(objects,'probe');positive=run(exe);(b/'initial-run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));assert not positive.returncode and 'NATIVE_TEXT_PUBLIC_GETTERS_PASS cases=256 cycles=4 retained_close=8' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr)
 (b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));linked=verify(b,objects,native,b/'Host0.cpp');roots=[]
 strict={'status':'UNCHANGED_PROVIDERS_EXISTING_GATES'}
 composition=dict(objects={str(p):sha(p) for p in objects},source_objects=source_objects,asset=str(b/'resource.big'),asset_sha256=asset_hash);(b/'composition.json').write_text(json.dumps(composition,indent=2));oracle=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_public_getter_oracle.py'),str(b)],capture_output=True,text=True,timeout=60);assert not oracle.returncode,oracle.stdout+oracle.stderr;print(oracle.stdout.strip(),flush=True)
 controls=[]
 for kind,source in [('Group',GETGROUP),('Text',GETTEXT)]:
  text=source.read_text();var=kind.lower();field='P'+kind;specs=[('loaded-state','if ('+var+'->IsLoaded() || load)','if (load)'),('lazy-start','BeginLoading'+kind+'(index);',';'),('decode',var+'->Build'+kind+'();',';'),('return-handle','return '+var+'->'+field+';','return CIVCountedPointer<const C'+kind+'Entry>();'),('return-reference','return '+var+'->'+field+';','{CIVCountedPointer<const C'+kind+'Entry> result;result.data_='+var+'->'+field+'.data_;return result;}'),('touch',var+'->'+field+'->TouchResource();',';')]
  for label,old,new in specs:
   assert old in text;name=kind.lower()+'-'+label;obj=compile(source,name,text=text.replace(old,new));fault=run(link([obj if str(p)==source_objects[str(source)] else p for p in objects],name));assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,(name,fault.returncode,fault.stderr);controls.append(dict(name=name,code=fault.returncode))
 for label,source,old,new in [
  ('schema',R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_Metadata_009c79d0.cpp','return 106;','return 105;'),
  ('narrator-index',R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','entry.NarratorIndex=*reinterpret_cast<const long*>(info.GetAsCArray());','entry.NarratorIndex=0;'),
  ('narrator-count',R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp','Narrators.resize(count);','Narrators.resize(count+1);')]:
  text=source.read_text();assert text.count(old)==1;obj=compile(source,label,text=text.replace(old,new));fault=run(link([obj if str(p)==source_objects[str(source)] else p for p in objects],label));record=dict(name=label,code=fault.returncode,stdout=fault.stdout,stderr=fault.stderr);controls.append(record);assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,record
 alignment_runs=[]
 for alignment in (4,16,2048):
  directory=b/('alignment-%d'%alignment);directory.mkdir();asset=directory/'resource.big';layout=create_archive(asset,alignment)
  for trial in range(2):
   result=subprocess.run([str(exe),str(asset)],cwd=b,capture_output=True,text=True,timeout=30);record=dict(alignment=alignment,trial=trial,code=result.returncode,stdout=result.stdout,stderr=result.stderr);assert not result.returncode and result.stdout==positive.stdout,record;alignment_runs.append(record)
  (directory/'probe.exe').write_bytes(exe.read_bytes());(directory/'probe.map').write_bytes((b/'probe.map').read_bytes());(directory/'run.json').write_text(json.dumps(dict(code=0,stdout=positive.stdout,stderr='')))
  (directory/'composition.json').write_text(json.dumps({**composition,'asset':str(asset),'asset_sha256':layout['sha256']},indent=2))
  original=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/native_text_public_getter_oracle.py'),str(directory)],capture_output=True,text=True,timeout=60);(directory/'oracle-run.json').write_text(json.dumps(dict(code=original.returncode,stdout=original.stdout,stderr=original.stderr)));assert not original.returncode,original.stdout+original.stderr
  assert sha(asset)==layout['sha256'];print('RETAIL_SCHEMA_ALIGNMENT_PASS',alignment,flush=True)
 again=run(exe);assert not again.returncode and again.stdout==positive.stdout;assert sha(b/'resource.big')==asset_hash
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),providers=len(profiles),cases=256,cycles=4,text_cases=128,group_cases=128,retained_close_handles=8,original_variants=4,original_runs=8,exact=strict,controls=controls,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},source_objects=source_objects,exe_sha256=sha(exe),fixture=fixture,alignment_runs=alignment_runs,alignments=[1,4,16,2048],metadata_entries=65,narrators=['ALPHA','','GAMMA'],boundary='Actual retail BIGB parsing and contained-bank lookup, concrete schema106, parsed type0 narrator indices -5 through -36, type1 metadata, three-name type2 table, retained spare slot, original reverse-record append order. Native public getters,4 lazy/forced/public-begin modes,wrong-type null returns,repeated identity/refcounts,later-frame touches,andretained resources across bankClose withbank alive until final handle release. Full native automatic buffer allocation/file worker/decoder/close cleanup. Disposal queue empty;eviction,renderer,andbank destruction withlive handles outside scope. Native allocator chunk retention until process exit.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-retail-schema.json').write_text(json.dumps(report,indent=2));print('NATIVE_RETAIL_TEXT_SCHEMA_GATE_PASS cases=256 cycles=4 retained_close=8 variants=4 controls=15 metadata_entries=65 alignments=4')
if __name__=='__main__':main()
