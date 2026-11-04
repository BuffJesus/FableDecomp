"""Native bank runtime-data packing over the accepted packed-array bodies."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import gate_native_packed_uint_initialise as P
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativeBankPackRuntime_model as M
SOURCES=[R/"rebuild/src/compiled/00/9c/CBankFile_PackRuntimeData_009cd740.cpp"]+P.SOURCES
FIXTURE=R/"rebuild/tests/integration/NativeBankPackRuntime_test.cpp"
SPECS=[{'bytes': 270,'differences': [],'entry': '0x9cd740','fixups': 13,'owner': 'root0.obj','symbol': '?PackRuntimeData@CBankFile@@AAEXXZ',
  'targets': {'?Initialise@CPackedUIntArray@@QAEXPAXKJK@Z': '0xa62d80','?Sizeof@CPackedUIntArray@@QBEKXZ': '0xa62a60',
   '?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ': '0x9d3d90','?ClearPackedBankArray@@YIXPAVCPackedUIntArray@@@Z': '0xa629d0'}}]+[dict(r,owner='root%d.obj'%(int(r['owner'][4:-4])+1)) for r in P.SPECS]
# Byte mutations of the root: (name, offset, original bytes, replacement).
FAULTS=[('single-entry-guard',0x25,b'\x01',b'\x00'),('first-entry-skip',0x32,b'\x0c',b'\x00'),('count-decrement',0x33,b'\x4f',b'\x90'),('size-width',0x4a,b'\x20',b'\x10'),('valid-field',0x7a,b'\x09',b'\x0a'),('capacity-slot',0x9c,b'\x01',b'\x00'),('record-stride',0xa4,b'\x02',b'\x01'),('branch-sense',0xd0,b'\x7d',b'\x7c'),('clear-last',0x102,b'\xe9',b'\xc3')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inputs={Path(__file__),Path(M.__file__),Path(P.__file__),EXE};visited=set()
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
 b=R/'rebuild/build/native-bank-pack-runtime/00/9c/009cd740'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
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
   assert kind in (6,20);target=int(row['targets'][name],16);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  candidate[entry]=bytes(fixed);original[entry]=read_virtual_data(image,entry,len(body));assert candidate[entry]==original[entry],row['symbol'];proof.append(row)
 command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 # The release body is supplied through /alternatename by the fixture observer.
 mapping['?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ']=mapping['@FixtureDeallocateRuntime@8']
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for row in SPECS:
  n=int(row['owner'][4:-4]);symbol=row['symbol'];section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];fixed=bytearray(body);entry=mapping[symbol]
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),symbol;file_offsets[int(row['entry'],16)]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=320 none=80 packed=168 runtime=72' in result.stdout,result.stdout+result.stderr
 cases,branches=M.compare(candidate,original);assert cases==320
 controls=[]
 for i,(name,offset,expect,replacement) in enumerate(FAULTS):
  assert original[M.ENTRY][offset:offset+len(expect)]==expect,name
  changed=dict(original);body=bytearray(changed[M.ENTRY]);body[offset:offset+len(replacement)]=replacement;changed[M.ENTRY]=bytes(body);detected=False
  for args in itertools.product((2,1,9,33,100),(3,0,60),(1,2,4,0),(1,0)):
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except Exception:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[M.ENTRY]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'BANK_PACK_RUNTIME_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in range(len(SOURCES)):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',grade='MATCH',build=str(b),model_cases=cases,model_branches=branches,host_cases=320,proof=proof,controls=controls,required_provider_omissions=len(SOURCES),input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),code_bodies=8,table_bytes=132,boundary='Exact270B PackRuntimeData root composed with the seven accepted packed-array bodies and the132-byte mask table. 320 differential and native guarded cases cover counts0..100, reserve slack0..60 including the n=2 threshold, zero/ranged/high/constant/alternating records and previous packed storage; no-op(<=1 entry), keep-packed and keep-runtime branches all exercised. Independent bias/width/size and per-bit packing checks, decoded Get values, full4KiB bank and64KiB heap digests and allocation/free/release traces agree. Runtime release 009D3D90 is an observed stand-in here (accepted separately by native-bank-allocate); width32 packed contents are left to the packed gate. No allocation-failure/unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-pack-runtime.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_PACK_RUNTIME_PASS model=320 host=320 controls=%d bodies=8 table=132'%(2*len(controls)))
if __name__=='__main__':main()
