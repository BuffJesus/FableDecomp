"""Native CBankFile::InitBlockOrder with its STLport sort closure (near_match root, exact helpers)."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare as similarity,retail_relocations
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativeBankBlockOrder_model as M
C=R/"rebuild/src/compiled"
SOURCES=[C/"00/9d/CBankFile_InitBlockOrder_009d0630.cpp"]+[C/"00/9d"/("STLBlockOrder_%s.cpp"%n) for n in ("Partition_009d2ed0","PartialSort_009d4620","MakeHeap_009d3cf0","AdjustHeap_009d3750","PushHeap_009d3040","SortHeap_009d42b0","FinalInsertionSort_009d3ed0","InsertionSort_009d3b90","UnguardedLinearInsert_009d2fc0")]+[C/"00/a6/CPackedUIntArray_Get_00a629f0.cpp",C/"00/46/UlongVector_Overflow_00464aab.cpp"]
FIXTURE=R/"rebuild/tests/integration/NativeBankBlockOrder_test.cpp"
CMP='CompareDataOffset@CBankFile@@'
# symbol -> (owner source index, retail entry, size); the root is graded near_match, the rest exact.
BODIES={'?InitBlockOrder@CBankFile@@AAEXXZ':(0,0x9d0630,None),
 '??RCompareDataOffset@CBankFile@@QBE_NKK@Z':(0,0x9cc640,94),
 '??$__introsort_loop@PAKKHU'+CMP+'@_STL@@YIXPAK00HU'+CMP+'@Z':(0,0x9d48c0,136),
 '??$__median@KU'+CMP+'@_STL@@YIABKABK00U'+CMP+'@Z':(0,0x9d2e40,132),
 '??$__unguarded_partition@PAKKU'+CMP+'@_STL@@YIPAKPAK0KU'+CMP+'@Z':(1,0x9d2ed0,234),
 '??$__partial_sort@PAKKU'+CMP+'@_STL@@YIXPAK000U'+CMP+'@Z':(2,0x9d4620,168),
 '??$__make_heap@PAKU'+CMP+'KH@_STL@@YIXPAK0U'+CMP+'0PAH@Z':(3,0x9d3cf0,77),
 '??$__adjust_heap@PAKHKU'+CMP+'@_STL@@YIXPAKHHKU'+CMP+'@Z':(4,0x9d3750,186),
 '??$__push_heap@PAKHKU'+CMP+'@_STL@@YIXPAKHHKU'+CMP+'@Z':(5,0x9d3040,186),
 '??$sort_heap@PAKU'+CMP+'@_STL@@YIXPAK0U'+CMP+'@Z':(6,0x9d42b0,80),
 '??$__final_insertion_sort@PAKU'+CMP+'@_STL@@YIXPAK0U'+CMP+'@Z':(7,0x9d3ed0,75),
 '??$__insertion_sort@PAKU'+CMP+'@_STL@@YIXPAK0U'+CMP+'@Z':(8,0x9d3b90,217),
 '??$__unguarded_linear_insert@PAKKU'+CMP+'@_STL@@YIXPAKKU'+CMP+'@Z':(9,0x9d2fc0,127),
 '??ACPackedUIntArray@@QBEKK@Z':(10,0xa629f0,101),
 '?g_PackedUIntMasks_013979E8@@3PAKA':(10,0x13979e8,132),
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':(11,0x464ace,188)}
EXTERNAL={'_memmove':M.MEMMOVE,'_malloc':M.MALLOC,'_free':M.FREE}
# (name, symbol, offset, original bytes, replacement)
FAULTS=[('valid-field','?InitBlockOrder@CBankFile@@AAEXXZ',0x3f,b'\x09',b'\x08'),('size-check','?InitBlockOrder@CBankFile@@AAEXXZ',0x49,b'\x76',b'\x90\x90'),
 ('depth-limit','?InitBlockOrder@CBankFile@@AAEXXZ',0xab,b'\x8d\x04\x09',b'\x31\xc0\x90'),('linear-insert-store','??$__unguarded_linear_insert@PAKKU'+CMP+'@_STL@@YIXPAKKU'+CMP+'@Z',0x76,b'\x89\x10',b'\x90\x90'),
 ('heap-store','??$__push_heap@PAKHKU'+CMP+'@_STL@@YIXPAKHHKU'+CMP+'@Z',0xb1,b'\x89\x4c\x9d\x00',b'\x90'*4),('introsort-threshold','??$__introsort_loop@PAKKHU'+CMP+'@_STL@@YIXPAK00HU'+CMP+'@Z',0x0f,b'\x40',b'\x10'),
 ('insertion-memmove','??$__insertion_sort@PAKU'+CMP+'@_STL@@YIXPAK0U'+CMP+'@Z',0x8b,b'\x7e',b'\xeb')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inputs={Path(__file__),Path(M.__file__),EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in SOURCES+[FIXTURE]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-block-order/00/9d/009d0630'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];coffs=[]
 for i,p in enumerate(SOURCES+[FIXTURE]):
  name='root%d'%i if i<len(SOURCES) else 'Fixture';flags=['/O2','/MT','/Gy']
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c',*flags,'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);coffs.append(Coff(obj))
 image=EXE.read_bytes();address={s:e for s,(n,e,z) in BODIES.items()};address.update(EXTERNAL);candidate={};original={};proof=[]
 for symbol,(n,entry,size) in BODIES.items():
  section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];assert offset==0,symbol;fixed=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20),(symbol,name);struct.pack_into('<I',fixed,site,(address[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  candidate[entry]=bytes(fixed)
  if size is None:
   original[entry]=read_virtual_data(image,entry,192);score=similarity(body,original[entry],tuple(rels),retail_relocations('%08x'%entry,192)).to_dict();assert score['near_match'],score
   proof.append(dict(symbol=symbol,entry=hex(entry),bytes=len(body),retail_bytes=192,fixups=len(rels),grade='near_match',similarity=score))
  else:
   original[entry]=read_virtual_data(image,entry,size);assert len(body)==size and candidate[entry]==original[entry],symbol;proof.append(dict(symbol=symbol,entry=hex(entry),bytes=size,fixups=len(rels),grade='MATCH'))
 command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for symbol,(n,entry,size) in BODIES.items():
  section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];fixed=bytearray(body);at=mapping[symbol]
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body)),symbol;file_offsets[symbol]=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase)
 total=M.write_cases(b/'cases.bin')
 result=subprocess.run([str(b/'probe.exe'),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=408 packed=204 heap=6 prefilled=2' in result.stdout,result.stdout+result.stderr
 cases,heap=M.compare(candidate,original);assert cases==408 and heap==6
 controls=[]
 for i,(name,symbol,offset,expect,replacement) in enumerate(FAULTS):
  entry=BODIES[symbol][1];assert candidate[entry][offset:offset+len(expect)]==expect,name
  changed=dict(candidate);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in M.cases():
   want=M.execute(original,*args)
   try:got=M.execute(changed,*args)
   except Exception:detected=True;break
   if got!=want:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[symbol]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  try:result=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=60);code,err=result.returncode,result.stderr
  except subprocess.TimeoutExpired:code,err=None,'timeout'
  save('control-%d.run.json'%i,dict(name=name,returncode=code,stderr=err));assert code==80 and 'BANK_BLOCK_ORDER_FAILURE' in err,(name,code,err);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 # The final insertion pass (root7) instantiates the same __unguarded_linear_insert comdat as
 # its own unit (root9), so dropping root9 alone must still link; every other unit is required.
 redundant={9:7};omissions=0
 for n in range(len(SOURCES)):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  if n in redundant:
   symbol=next(s for s,(k,e,z) in BODIES.items() if k==n);assert not result.returncode and symbol in coffs[redundant[n]].symbols,n
  else:assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout),n;omissions+=1
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',grade='near_match',build=str(b),model_cases=cases,heap_fallback_cases=heap,host_cases=408,case_words=total,proof=proof,controls=controls,required_provider_omissions=omissions,redundant_providers={'root9':'root7'},input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='InitBlockOrder root near_match (203B vs retail192B, same behavior): the landed vector<unsigned long>::_M_insert_overflow is an explicit specialization, so the root declares it, and without its body VC7.1 reloads the address-taken loop index after the call; the exact192B form needs the generic overflow body in this unit, which conflicts with that landing (LNK2005). CompareDataOffset94B, __introsort_loop136B, __median132B (fastcall), partition234B, partial_sort168B, make_heap77B, adjust_heap186B, push_heap186B, sort_heap80B, final insertion75B, insertion217B (inline __linear_insert, memmove backward copy), unguarded linear insert127B exact; packed Get101B, mask table and overflow188B exact. 408 differential and native cases: counts0..500, all/mixed/none/zero-size validity, ascending/descending/random/equal/few-distinct/McIlroy-adversary offsets (six reach the depth-limit heap path), runtime and packed offset tables, prefilled no-op. Independent Python STLport sort predicts the exact order including ties; native host also checks ascending offsets through the real packed Get. CRT malloc/free/memmove are real in the host and modeled in the emulator. No allocation-failure claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-block-order.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_BLOCK_ORDER_PASS model=408 host=408 heap=6 controls=%d bodies=%d grade=near_match'%(2*len(controls),len(BODIES)))
if __name__=='__main__':main()
