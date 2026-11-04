"""Native bank filename path conversion with wide/narrow ownership."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativeSmallStringVector_model as M
SOURCES=[R/"rebuild/src/compiled"/n for n in ['00/9d/CSmallVector_CCharString_AllocateCopy_009d2160.cpp',
 '00/9d/CSmallVector_CCharString_Clear_009d2110.cpp',
 '00/99/CCharString_DefaultConstructor_0099e4b0.cpp',
 '00/99/CCharString_CopyAssignment_0099efb0.cpp',
 '00/99/CCharString_Destructor_0099eae0.cpp',
 '00/99/CCharString_UnassignString_0099e9b0.cpp']]
FIXTURE=R/"rebuild/tests/integration/NativeSmallStringVector_test.cpp"
SPECS=[{'bytes': 99,
  'differences': [],
  'entry': '0x9d2160',
  'fixups': 3,
  'owner': 'root0.obj',
  'symbol': '?Allocate@?$CSmallVector@VCCharString@@$07@@QAEXK@Z',
  'targets': {'??0CCharString@@QAE@XZ': '0x99e4b0',
              '??_U@YAPAXI@Z': '0xbfeb22',
              '?Clear@?$CSmallVector@VCCharString@@$07@@QAEXXZ': '0x9d2110'}},
 {'bytes': 68,
  'differences': [],
  'entry': '0x9d21d0',
  'fixups': 2,
  'owner': 'root0.obj',
  'symbol': '?Copy@?$CSmallVector@VCCharString@@$07@@QAEXABV1@@Z',
  'targets': {'??4CCharString@@QAEAAV0@ABV0@@Z': '0x99efb0',
              '?Allocate@?$CSmallVector@VCCharString@@$07@@QAEXK@Z': '0x9d2160'}},
 {'bytes': 69,
  'differences': [],
  'entry': '0x9d2110',
  'fixups': 2,
  'owner': 'root1.obj',
  'symbol': '?Clear@?$CSmallVector@VCCharString@@$07@@QAEXXZ',
  'targets': {'??1CCharString@@QAE@XZ': '0x99eae0', '??_V@YAXPAX@Z': '0xbfeb1c'}},
 {'bytes': 15,
  'differences': [],
  'entry': '0x99e4b0',
  'fixups': 1,
  'owner': 'root2.obj',
  'symbol': '??0CCharString@@QAE@XZ',
  'targets': {'?g_CCharStringInstanceCount_013BD800@@3HA': '0x13bd800'}},
 {'bytes': 40,
  'differences': [],
  'entry': '0x99efb0',
  'fixups': 1,
  'owner': 'root3.obj',
  'symbol': '??4CCharString@@QAEAAV0@ABV0@@Z',
  'targets': {'?UnassignString@CCharString@@AAEXXZ': '0x99e9b0'}},
 {'bytes': 12,
  'differences': [],
  'entry': '0x99eae0',
  'fixups': 2,
  'owner': 'root4.obj',
  'symbol': '??1CCharString@@QAE@XZ',
  'targets': {'?UnassignString@CCharString@@AAEXXZ': '0x99e9b0',
              '?g_CCharStringInstanceCount_013BD800@@3HA': '0x13bd800'}},
 {'bytes': 87,
  'differences': [],
  'entry': '0x99e9b0',
  'fixups': 2,
  'owner': 'root5.obj',
  'symbol': '?UnassignString@CCharString@@AAEXXZ',
  'targets': {'??3@YAXPAX@Z': '0xbfe9bc', '??_V@YAXPAX@Z': '0xbfeb1c'}}]
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
 b=R/'rebuild/build/native-small-string-vector/00/9d/009d2160'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];coffs=[]
 for name in ['root%d'%i for i in range(len(SOURCES))]+['Fixture']:
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);coffs.append(Coff(obj))

 candidate={};original={};image=EXE.read_bytes();proof=[]
 for row in SPECS:
  n=int(row['owner'][4:-4]);c=coffs[n];section,offset=c.symbols[row['symbol']];body,rels=c.sections[section];assert offset==0 and len(body)==row['bytes'] and len(rels)==row['fixups']
  entry=int(row['entry'],16);fixed=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);target=int(row['targets'][{'_PathMalloc':'_malloc','_PathFree':'_free'}.get(name,name)],16);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  candidate[entry]=bytes(fixed);original[entry]=read_virtual_data(image,entry,len(body));assert candidate[entry]==original[entry],row['symbol'];proof.append(row)
 command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for row in SPECS:
  n=int(row['owner'][4:-4]);symbol=row['symbol'];section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];fixed=bytearray(body);entry=mapping[symbol]
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),symbol;file_offsets[int(row['entry'],16)]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=240' in result.stdout
 cases=M.compare(candidate,original);assert cases==240



 faults=[('clear-before-allocate',0x9d2160,4,b'\x90'*5),('array-allocation-size',0x9d2160,0x10,b'\x08'),('size-byte-store',0x9d2160,0x4f,b'\x90'*3),('copy-assignment',0x9d21d0,0x30,b'\x83\xc4\x04\x90\x90'),('array-free',0x9d2110,0x2f,b'\x90'*5),('constructor-counter',0x99e4b0,8,b'\x90'*6),('assignment-owner',0x99efb0,0x1e,b'\x90'*3),('destructor-counter',0x99eae0,5,b'\x90'*6)]
 controls=[]
 for i,(name,entry,offset,replacement) in enumerate(faults):
  changed=dict(original);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in M.cases():
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'SMALL_STRING_VECTOR_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in range(6):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),model_cases=cases,host_cases=240,proof=proof,controls=controls,required_provider_omissions=6,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Seven exact original/final bodies compose small string-vector Allocate99B, Copy68B, Clear69B, default constructor15B, assignment40B, destructor12B and Unassign87B. 240 original/rebuilt/native cases cover replacement, zero-sized allocation, shared/null/distinct anchor keys, self-copy and array cookies. Allocate counts0..255 include128/255 whose stored byte wraps; Copy is bounded to0..127, retaining exact signed-byte reads via scoped casts without changing shared Size type/layout. Self-copy clears source contents before assignment and produces null elements. Full128KiB original/rebuilt arena digests and allocation/free traces compared; native guarded CRT arrays released once with every element destroyed. Sixteen fixture anchor owners remain with count baseline16. No negative-length Copy, allocation-failure/unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-small-string-vector.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SMALL_STRING_VECTOR_PASS model=240 host=240 controls=16 bodies=7')
if __name__=='__main__':main()
