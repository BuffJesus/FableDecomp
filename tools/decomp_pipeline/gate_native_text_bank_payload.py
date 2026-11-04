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
import capstone
R=Path(__file__).resolve().parents[2]
GROUP=R/'rebuild/src/compiled/00/9c/NGameText_CGroupEntry_ReadFromDataStream_009c8470.cpp'
TEXT=R/'rebuild/src/compiled/00/9c/NGameText_CTextEntry_ReadFromDataStream_009c8de0.cpp'
TAGS=R/'rebuild/src/compiled/00/9c/NGameText_TagVectors_009caf60.cpp'
LONGS=R/'rebuild/src/compiled/00/44/STLLongVector_Resize_004428f0.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 b=R/'rebuild/build/native-text-bank-payload/00/9c/009c8de0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();profiles=[(R/'rebuild/src/compiled'/path,flags) for path,flags in SOURCES]
 fixtures=[R/'rebuild/tests/integration'/name for name in ('NativeTextPayloadMatrix_test.cpp','NativeRetailReadOnlyGlobals_test.cpp','NativeTextTagVector_test.cpp')]
 def closure(p):
  if p in seen:return
  seen.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
 for p in [p for p,f in profiles]+fixtures:closure(p)
 inputs=set(seen)|{Path(__file__),EXE}|{R/'tools/decomp_pipeline'/name for name in ('native_text_payload_reader_oracle.py','native_text_tag_vector_oracle.py','native_text_tag_helper_audit.py')}
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
  exe=b/(label+'.exe');libs=['kernel32.lib',str(W.WINMM)] if win else [];run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/include:?resize@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@_STL@@@_STL@@QAEXI@Z','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items),*libs],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;return exe
 def run(exe):return subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30)
 for i,(source,flags) in enumerate(profiles):
  obj=compile(source,'native%d'%i,flags);objects.append(obj);native.add(obj.name);source_objects[str(source)]=str(obj)
  if (i+1)%60==0:print('NATIVE_TEXT_PROVIDERS',i+1,flush=True)
 for i,source in enumerate(fixtures[:2]):objects.append(compile(source,'Host%d'%i))
 exe=link(objects,'probe');positive=run(exe);assert not positive.returncode and 'NATIVE_TEXT_PAYLOAD_MATRIX_PASS text=512 groups=512 reused=1' in positive.stdout,(positive.returncode,positive.stdout,positive.stderr)
 (b/'run.json').write_text(json.dumps(dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr)))
 linked=verify(b,objects,native,b/'Host0.cpp');grades=[]
 for source,prefix,address,size,exact in [(GROUP,'?ReadFromDataStream@CGroupEntry',0x9c8470,292,True),(TEXT,'?ReadFromDataStream@CTextEntry',0x9c8de0,452,False),(TAGS,'?_M_fill_insert@?$vector@VCTextTag',0x9caf60,280,True),(LONGS,'?_M_fill_insert@?$vector@JV',0x440f00,276,False)]:
  c=Coff(Path(source_objects[str(source)]));symbol=next(n for n in c.symbols if n.startswith(prefix));section,offset=c.symbols[symbol];body,rels=c.sections[section];original=read_virtual_data(EXE.read_bytes(),address,size);score=compare(body,original,tuple(rels),retail_relocations('%08x'%address,size)).to_dict();assert not offset and score['near_match'] and score['complete_decode']
  if exact:
   left,right=bytearray(body),bytearray(original)
   for site in rels:left[site:site+4]=right[site:site+4]=bytes(4)
   assert left==right
  grades.append(dict(symbol=symbol,address=hex(address),bytes=len(body),exact=exact,score=score))
 # Preserve the provider map for original-reader transplants and helper audits.
 composition=dict(objects={str(p):sha(p) for p in objects},source_objects=source_objects)
 (b/'composition.json').write_text(json.dumps(composition,indent=2))
 def helper(name):
  result=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline'/name),str(b)],capture_output=True,text=True,timeout=60);assert not result.returncode,result.stdout+result.stderr;print(result.stdout.strip(),flush=True)
 helper('native_text_payload_reader_oracle.py');helper('native_text_tag_helper_audit.py')
 exact_address={'_memmove':0xbfeae6,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'?resize@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXIABJ@Z':0x4428f0,'?Sizeof@CBankFileAsyncData@@QBEKXZ':0x9d5620,'?SetResourceSize@CResource@@QAEXK@Z':0x9fbf20,'?_M_fill_insert@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXPAJIABJ@Z':0x440f00}
 roots=[]
 for source,prefix,address,size in [(GROUP,'?ReadFromDataStream@CGroupEntry',0x9c8470,292),(LONGS,'?resize@?$vector@JV',0x4428f0,87)]:
  c=Coff(Path(source_objects[str(source)]));symbol=next(n for n in c.symbols if n.startswith(prefix));section,offset=c.symbols[symbol];body,rels=c.sections[section];roots.append(dict(name='payload_'+hex(address),source=source,symbol=symbol,address=address,size=size,instructions=len(list(capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(read_virtual_data(EXE.read_bytes(),address,size),address))),relocations=len(rels),flags=['/nologo','/c','/O2','/MT','/Gy']))
 strict=native_exact_roots.run(__file__,roots,exact_address,'native-text-payload-exact.json',['512 text/group and2268 tag original parity cases.'],'Typed group decoder andlong resize exactbytes andindependently anchored calltargets;tag helper closure checked separately.')

 tag_host=compile(fixtures[2],'tag-host');tag_obj=compile(TAGS,'tag-vectors',['/Dmalloc=TagFixtureMalloc','/Dfree=TagFixtureFree']);tag_objects=[p for p in objects if p not in (Path(source_objects[str(TAGS)]),objects[-2])]+[tag_obj,tag_host]
 tag_exe=link(tag_objects,'tag');tag_run=run(tag_exe);assert not tag_run.returncode and 'NATIVE_TAG_VECTOR_PASS cases=2268' in tag_run.stdout,(tag_run.returncode,tag_run.stdout,tag_run.stderr)
 (b/'tag-run.json').write_text(json.dumps(dict(code=tag_run.returncode,stdout=tag_run.stdout,stderr=tag_run.stderr)))
 tag_sources=dict(source_objects);tag_sources[str(TAGS)]=str(tag_obj);(b/'composition.json').write_text(json.dumps(dict(objects={str(p):sha(p) for p in tag_objects},source_objects=tag_sources),indent=2));helper('native_text_tag_vector_oracle.py');(b/'composition.json').write_text(json.dumps(composition,indent=2))
 specs=[('tag-position',TEXT,'Tags[i].Pos=stream.ReadValue<long>();','Tags[i].Pos=stream.ReadValue<long>()+1;'),('string-fields',TEXT,'SpeechBank=stream.ReadPresizedString();\n    Narrator=stream.ReadPresizedString();','Narrator=stream.ReadPresizedString();\n    SpeechBank=stream.ReadPresizedString();'),('context-restore',TEXT,'g_NativeTextReadContext_0138DE48=first;','g_NativeTextReadContext_0138DE48=11;'),('group-value',GROUP,'Entries[i]=stream.ReadValue<long>();','Entries[i]=stream.ReadValue<long>()+1;'),('group-size',GROUP,'Entries.capacity()*sizeof(long)','Entries.size()*sizeof(long)')];controls=[]
 for label,source,old,new in specs:
  text=source.read_text();assert text.count(old)==1;obj=compile(source,label,text=text.replace(old,new));fault=run(link([obj if str(p)==source_objects[str(source)] else p for p in objects],label));assert fault.returncode==80 and 'TEXT_PAYLOAD_FAILURE' in fault.stderr,(label,fault.returncode,fault.stderr);controls.append(dict(name=label,code=fault.returncode))
 for label,old,new in [('tag-alias-copy','NGameText::CTextTag copyValue=value;','const NGameText::CTextTag& copyValue=value;'),('tag-copy-position','NGameText::CTextTag copyValue=value;','NGameText::CTextTag copyValue=value;copyValue.Pos=0;')]:
  text=TAGS.read_text();assert text.count(old)==1;obj=compile(TAGS,label,['/Dmalloc=TagFixtureMalloc','/Dfree=TagFixtureFree'],text=text.replace(old,new));fault=run(link([obj if p==tag_obj else p for p in tag_objects],label));assert fault.returncode==80 and 'TAG_VECTOR_FAILURE' in fault.stderr,(label,fault.returncode,fault.stderr);controls.append(dict(name=label,code=fault.returncode))
 assert run(exe).stdout==positive.stdout and run(tag_exe).stdout==tag_run.stdout
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),text_cases=512,group_cases=512,reused_objects=True,original_reader_variants=4,tag_cases=2268,original_tag_cases=2268,grades=grades,exact=strict,controls=controls,providers=len(profiles),linked_symbols=linked['linked_symbols'],unique_bodies=linked['unique_bodies'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects+[tag_host,tag_obj]},source_objects=source_objects,exe_sha256=sha(exe),tag_exe_sha256=sha(tag_exe),boundary='Native typed text/group payload decoding; CRT fixture allocation, null resource owner/list. Context word meanings provisional. Resource scheduler,renderer,loaded-resource lifetime andinstalled assets outside this gate.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-text-bank-payload.json').write_text(json.dumps(report,indent=2));print('NATIVE_TEXT_BANK_PAYLOAD_PASS text=512 groups=512 variants=4 tags=2268 original_tags=2268 controls=7')
if __name__=='__main__':main()
