"""Native bank filename path conversion with wide/narrow ownership."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativePackedUIntInitialise_model as M
SOURCES=[R/"rebuild/src/compiled/00/a6"/n for n in ['CPackedUIntArray_CalculateBias_00a62ae0.cpp',
 'CPackedUIntArray_CountBits_00a62b70.cpp',
 'CPackedUIntArray_PackArray_00a62c90.cpp',
 'CPackedUIntArray_Initialise_00a62d80.cpp',
 'CPackedUIntArray_Get_00a629f0.cpp',
 'CPackedUIntArray_Sizeof_00a62a60.cpp',
 'CPackedUIntArray_ClearPackedBankArray_00a629d0.cpp']]
FIXTURE=R/"rebuild/tests/integration/NativePackedUIntInitialise_test.cpp"
SPECS=[{'bytes': 130,
  'differences': [],
  'entry': '0xa62ae0',
  'fixups': 1,
  'owner': 'root0.obj',
  'symbol': '?CalculateBias@CPackedUIntArray@@QAEKPAXKJK@Z',
  'targets': {'?g_PackedUIntMasks_013979E8@@3PAKA': '0x13979e8'}},
 {'bytes': 285,
  'differences': [],
  'entry': '0xa62b70',
  'fixups': 1,
  'owner': 'root1.obj',
  'symbol': '?CountBits@CPackedUIntArray@@QAEKPAXKJK@Z',
  'targets': {'?g_PackedUIntMasks_013979E8@@3PAKA': '0x13979e8'}},
 {'bytes': 238,
  'differences': [],
  'entry': '0xa62c90',
  'fixups': 1,
  'owner': 'root2.obj',
  'symbol': '?PackArray@CPackedUIntArray@@QAEXPAXKJK@Z',
  'targets': {'?g_PackedUIntMasks_013979E8@@3PAKA': '0x13979e8'}},
 {'bytes': 152,
  'differences': [],
  'entry': '0xa62d80',
  'fixups': 5,
  'owner': 'root3.obj',
  'symbol': '?Initialise@CPackedUIntArray@@QAEXPAXKJK@Z',
  'targets': {'??_U@YAPAXI@Z': '0xbfeb22',
              '??_V@YAXPAX@Z': '0xbfeb1c',
              '?CalculateBias@CPackedUIntArray@@QAEKPAXKJK@Z': '0xa62ae0',
              '?CountBits@CPackedUIntArray@@QAEKPAXKJK@Z': '0xa62b70',
              '?PackArray@CPackedUIntArray@@QAEXPAXKJK@Z': '0xa62c90'}},
 {'bytes': 101,
  'differences': [],
  'entry': '0xa629f0',
  'fixups': 1,
  'owner': 'root4.obj',
  'symbol': '??ACPackedUIntArray@@QBEKK@Z',
  'targets': {'?g_PackedUIntMasks_013979E8@@3PAKA': '0x13979e8'}},
 {'bytes': 132,
  'differences': [],
  'entry': '0x13979e8',
  'fixups': 0,
  'owner': 'root4.obj',
  'symbol': '?g_PackedUIntMasks_013979E8@@3PAKA',
  'targets': {}},
 {'bytes': 22,
  'differences': [],
  'entry': '0xa62a60',
  'fixups': 0,
  'owner': 'root5.obj',
  'symbol': '?Sizeof@CPackedUIntArray@@QBEKXZ',
  'targets': {}},
 {'bytes': 29,
  'differences': [],
  'entry': '0xa629d0',
  'fixups': 1,
  'owner': 'root6.obj',
  'symbol': '?ClearPackedBankArray@@YIXPAVCPackedUIntArray@@@Z',
  'targets': {'??_V@YAXPAX@Z': '0xbfeb1c'}}]
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
 b=R/'rebuild/build/native-packed-uint-initialise/00/a6/00a62d80'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
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
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=3360' in result.stdout
 cases=M.compare(candidate,original);assert cases==3360




 clear=next(r for r in SPECS if r['entry']=='0xa629d0');c=coffs[6];sec,_=c.symbols[clear['symbol']];_,rels=c.sections[sec];clear_site=next(site for site,(name,kind) in rels.items() if name=='??_V@YAXPAX@Z')
 faults=[('source-width-clamp',0xa62d80,0x10,b'\x1f'),('minimum-bias',0xa62ae0,0x66,b'\x73'),('combined-range',0xa62b70,0x7b,b'\x90'*2),('packed-word-store',0xa62c90,0xa9,b'\x90'*3),('packed-carry',0xa62c90,0xbc,b'\x31\xff'),('previous-array-free',0xa62d80,0x17,b'\x90'*5),('clear-array-free',0xa629d0,clear_site-1,b'\x90'*5),('width-mask-table',0x13979e8,9*4,b'\xfe')]
 controls=[]
 for i,(name,entry,offset,replacement) in enumerate(faults):
  changed=dict(original);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in itertools.product((32,9,1,40,0),(3,7,1,0),(12,-5,5,0),(2,3,1,0),(1,0)):
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'PACKED_INITIALISE_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in range(7):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),model_cases=cases,host_cases=3360,proof=proof,controls=controls,required_provider_omissions=7,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),code_bodies=7,table_bytes=132,boundary='Seven exact original/final code bodies and the complete132-byte width-mask table compose bias130B, width285B, packing238B, initialization152B, Get101B, Sizeof22B and Clear29B. 3360 differential and native guarded cases cover zero/counts through33, widths0..32 and clamping40, zero/positive/negative strides, constant/high/ranged values and previous allocations. Independent per-bit packing/decoding checks, full128KiB original/rebuilt arena digests and allocation/free traces agree; all guarded arrays released and source/padding preserved. The original width32 carry behavior is deliberately retained and covered by120 host cases. Target VC71 x86 shift semantics are explicit; no allocation-failure/unwind, large-size arithmetic or out-of-range direct-helper claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-packed-uint-initialise.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PACKED_UINT_INITIALISE_PASS model=3360 host=3360 controls=16 bodies=7 table=132')
if __name__=='__main__':main()
