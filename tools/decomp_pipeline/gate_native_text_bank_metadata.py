"""Typed text-bank metadata and pointer-fill parity without installed assets."""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,subprocess,sys
import capstone
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_retail_readonly_sources import SOURCES
from native_text_metadata_sources import EXTRAS,EXACT_VECTORS,ADDRESS
from native_retail_readonly_verify import verify
import native_text_metadata_oracles as oracle
import native_exact_roots
import gate_native_bank_shared_worker as W
R=Path(__file__).resolve().parents[2]
META=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_Metadata_009c79d0.cpp'
ENTRY=R/'rebuild/src/compiled/00/9c/NGameText_CDataBank_NotifyEntryAdded_009c8c00.cpp'
VECTORS=R/'rebuild/src/compiled/00/9c/NGameText_MetadataVectors_009ca950.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 b=R/'rebuild/build/native-text-bank-metadata/00/9c/009c8c00'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES];chosen={p for p,f in profiles}
 for path in EXTRAS:
  p=R/'rebuild/src/compiled'/path
  if p not in chosen:profiles.append((p,[]));chosen.add(p)
 profiles += [(META,[]),(ENTRY,[]),(VECTORS,[])]
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeTextBankMetadata_test.cpp','NativeRetailReadOnlyGlobals_test.cpp','NativeTextPointerFill_test.cpp')]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE}
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
  p=b/(label+'.cpp');p.write_text(text if text is not None else source.read_text());obj=p.with_suffix('.obj');options=['/I'+str(extra_include)] if extra_include else [];run=V.cl(['/nologo','/c','/O2','/MT','/Gy',*flags,*options,'/I'+str(inc),'/Fo'+str(obj),str(p)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr,source=str(source),flags=list(flags))));assert not run.returncode,run.stdout+run.stderr;return obj
 def link(items,label,win=True):
  exe=b/(label+'.exe');libs=['kernel32.lib',str(W.WINMM)] if win else [];run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items),*libs],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;return exe
 def run(exe):return subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30)
 for i,(source,flags) in enumerate(profiles):
  obj=compile(source,'native%d'%i,flags);objects.append(obj);native.add(obj.name);source_objects[str(source)]=str(obj)
  if (i+1)%60==0:print('NATIVE_TEXT_PROVIDERS',i+1,flush=True)
 for i,source in enumerate(fixtures[:2]):objects.append(compile(source,'Host%d'%i))
 exe=link(objects,'probe');positive=run(exe);assert not positive.returncode and 'NATIVE_TEXT_METADATA_BOUNDARY_PASS size=512 entry=512 readonly_noop=512 version=106' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr);(b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)));linked=verify(b,objects,native,b/'Host0.cpp');original=oracle.metadata(b,positive.stdout)
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);exact=[]
 for row in EXACT_VECTORS:
  address=int(row['address'],16);instructions=len(list(decoder.disasm(read_virtual_data(EXE.read_bytes(),address,row['bytes']),address)));c=Coff(Path(source_objects[str(VECTORS)]));section,offset=c.symbols[row['symbol']];body,rels=c.sections[section];assert not offset
  exact.append(dict(name='typed_vector_'+hex(address),source=VECTORS,symbol=row['symbol'],address=address,size=row['bytes'],instructions=instructions,relocations=len(rels),flags=['/nologo','/c','/O2','/MT','/Gy']))
 exact.append(dict(name='text_format_106',source=META,symbol='?GetDataFormatVersion@CDataBank@NGameText@@MAEKXZ',address=0x9c79d0,size=6,instructions=2,relocations=0,flags=['/nologo','/c','/O2','/MT','/Gy']))
 exact_report=native_exact_roots.run(__file__,exact,ADDRESS,'native-text-metadata-exact.json',['Original/recreated metadata matrix and33048 pointer-fill cases.'],'Seventeen exact typed bodies;near callbacks andpointer-fill graded separately;no historical rootcount.')
 grades=[]
 for source,prefix,address,size in [(META,'?NotifyBankSize@CDataBank@',0x9c82c0,122),(ENTRY,'?NotifyEntryAdded@CDataBank@',0x9c8c00,469),(VECTORS,'?_M_fill_insert@?$vector@PAVCDataBankEntry',0x9caa30,276)]:
  c=Coff(Path(source_objects[str(source)]));symbol=next(n for n in c.symbols if n.startswith(prefix));section,offset=c.symbols[symbol];body,rels=c.sections[section];score=compare(body,read_virtual_data(EXE.read_bytes(),address,size),tuple(rels),retail_relocations('%08x'%address,size)).to_dict();assert not offset and score['near_match'] and score['complete_decode'];grades.append(dict(symbol=symbol,address=hex(address),bytes=len(body),score=score))
 pointer_fixture=compile(fixtures[2],'pointer-fixture');guard_flags=['/Dmalloc=TextFixtureMalloc','/Dfree=TextFixtureFree'];pointer_vector=compile(VECTORS,'pointer-vectors',guard_flags);pointer_exe=link([pointer_fixture,pointer_vector],'pointer',False);pointer=run(pointer_exe);assert not pointer.returncode and 'NATIVE_POINTER_FILL_PASS cases=33048' in pointer.stdout,(pointer.returncode,pointer.stdout,pointer.stderr);original_pointer=oracle.pointer_fill(b,pointer.stdout)
 specs=[('format-107',META,'return 106;','return 107;'),('size-readonly-inverted',META,'if(ReadOnly){','if(!ReadOnly){'),('wrong-type-reserve',META,'if(type==0)texts+=n;','if(type==0)groups+=n;'),('entry-readonly-inverted',ENTRY,'if(ReadOnly){','if(!ReadOnly){'),('wrong-narrator',ENTRY,'entry.NarratorIndex=*reinterpret_cast<const long*>(info.GetAsCArray());','entry.NarratorIndex=*reinterpret_cast<const long*>(info.GetAsCArray())+1;')];controls=[]
 for label,source,old,new in specs:
  text=source.read_text();assert text.count(old)==1;obj=compile(source,label,text=text.replace(old,new));items=[obj if str(p)==source_objects[str(source)] else p for p in objects];fault=run(link(items,label));assert fault.returncode==80 and 'TEXT_BOUNDARY_FAILURE' in fault.stderr,(label,fault.returncode,fault.stderr);controls.append(dict(name=label,code=fault.returncode))
 header=(inc/'fable_stlport_vector_impl.h').read_text()
 for label,old,new in [('alias-copy-removed','_Tp __x_copy = __x;','const _Tp& __x_copy = __x;'),('value-zeroed','_Tp __x_copy = __x;','_Tp __x_copy = _Tp();')]:
  assert header.count(old)==1;control_inc=b/(label+'-include');control_inc.mkdir();(control_inc/'fable_stlport_vector_impl.h').write_text(header.replace(old,new));obj=compile(VECTORS,label,guard_flags,extra_include=control_inc);fault=run(link([pointer_fixture,obj],label,False));assert fault.returncode==80 and 'POINTER_FILL_FAILURE' in fault.stderr,(label,fault.returncode,fault.stderr);controls.append(dict(name=label,code=fault.returncode))
 assert run(exe).stdout==positive.stdout and run(pointer_exe).stdout==pointer.stdout
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),metadata_size_cases=512,metadata_entry_cases=512,readonly_noop_cases=512,original_metadata_variants=original,pointer_cases=33048,original_pointer_cases=33048,original_pointer=original_pointer,exact=exact_report,near=grades,controls=controls,linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],providers=len(profiles),input_sha256=pins,object_sha256={str(p):sha(p) for p in objects+[pointer_fixture,pointer_vector]},exe_sha256=sha(exe),pointer_exe_sha256=sha(pointer_exe),source_objects=source_objects,boundary='Actual typed textbank andnative dependencies withborrowed bounded metadata views;guarded CRT callbacks forstandalone pointer model. Resource unload/allocation/date/enumeration paths cold;no textpayload decoding,resource loading,renderer,installedasset dependency ornew historical count.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-bank-metadata.json').write_text(json.dumps(report,indent=2));print('NATIVE_TEXT_BANK_METADATA_PASS size=512 entry=512 variants=4 pointer=33048 original_pointer=33048 controls=7 exact=17 near=3')
if __name__=='__main__':main()
