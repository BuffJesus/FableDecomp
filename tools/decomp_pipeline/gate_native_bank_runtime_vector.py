"""Native runtime-data vector composition, original replay and fault controls."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess
import pefile
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankRuntimeVector_model as M
SOURCES=[R/'rebuild/src/compiled/00/9d/STLBankRuntimeVector_Storage_009d3a20.cpp',R/'rebuild/src/compiled/00/9d/STLBankRuntimeVector_Resize_009d4010.cpp']
FIXTURE=R/'rebuild/tests/integration/NativeBankRuntimeVector_test.cpp'
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
 b=R/'rebuild/build/native-bank-runtime-vector/00/9d/009d3a20'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
 for name in ('root0','root1','Fixture'):
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT',*(['/Dmalloc=BankRuntimeMalloc','/Dfree=BankRuntimeFree','/Dmemmove=BankRuntimeMemmove'] if name!='Fixture' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj)
 specs=[]
 for n in range(2):
  c=Coff(objects[n])
  for symbol,(section,offset) in c.symbols.items():
   prefixes=[('??$fill@',(0x9d1bb0,49,0)),('??$__uninitialized_fill_n',(0x9d1ca0,52,0)),('??$__copy_backward',(0x9d1d40,76,0)),('??$__uninitialized_copy',(0x9d23f0,56,0)),('??$uninitialized_fill_n',(0x9d2450,52,0)),('?_M_insert_overflow',(0x9d3580,219,5)),('?_M_fill_insert',(0x9d3a20,311,7)),('?resize@',(0x9d4010,87,2)),('??$__copy@',(0x9d1c30,74,0))]
   for prefix,(entry,length,fixups) in prefixes:
    if symbol.startswith(prefix):specs.append((n,symbol,entry,length,fixups));break
 assert len(specs)==9
 targets={s:a for _,s,a,_,_ in specs};targets.update(_BankRuntimeMalloc=0xbfea0e,_BankRuntimeFree=0xbfea14,_BankRuntimeMemmove=0xbfeae6)
 proof=[check(objects[n],s,a,l,f,targets) for n,s,a,l,f in specs]
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib'];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for n,s,a,l,f in specs:
  c=Coff(objects[n]);section,offset=c.symbols[s];body,rels=c.sections[section];assert offset==0;fixed=bytearray(body);entry=mapping[s]
  for site,(name,kind) in rels.items():
   assert kind==20;struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body));file_offsets[a]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 data,expected,report=M.collect();(b/'input.bin').write_bytes(data);(b/'expected.bin').write_bytes(expected)
 result=subprocess.run([str(b/'probe.exe'),str(b/'input.bin'),str(b/'actual.bin')],cwd=b,capture_output=True,text=True,timeout=60);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=3680' in result.stdout;assert (b/'actual.bin').read_bytes()==expected
 assert report['cases']==3680
 compact,expected_small,small=M.collect(compact=True);(b/'control-input.bin').write_bytes(compact);save('control-cases.json',small)
 faults=[('fill-first-word',0x9d1bb0,0x16,b'\x90'*2),('copy-back-padding',0x9d1d40,0x39,b'\x90'*3),('copy-new-padding',0x9d23f0,0x25,b'\x90'*3),('fill-new-padding',0x9d2450,0x25,b'\x90'*3),('growth-fill-padding',0x9d1ca0,0x25,b'\x90'*3),('growth-free',0x9d3580,0xbc,b'\x90'*5),('spare-gap-write',0x9d3a20,0xaa,b'\x83\xc4\x04\x90\x90'),('spare-tail-write',0x9d3a20,0x107,b'\x83\xc4\x04\x90\x90')]

 controls=[];image=EXE.read_bytes()
 for i,(name,entry,offset,replacement) in enumerate(faults):
  length=next(l for _,_,a,l,_ in specs if a==entry);body=bytearray(read_virtual_data(image,entry,length));body[offset:offset+len(replacement)]=replacement
  _,changed,changed_report=M.collect({entry:bytes(body)},compact=True);assert changed!=expected_small,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered);output=b/('control-%d.bin'%i)
  result=subprocess.run([str(exe),str(b/'control-input.bin'),str(output)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,(name,result.returncode);assert output.read_bytes()==changed,(name,'native/original control drift');controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in range(2):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report.update(status='PASS',build=str(b),proof=proof,controls=controls,required_provider_omissions=2,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Nine exact original and final-linked bodies. Full16KiB replay at each malloc/memmove/free and return, including aliased fills and mutating callbacks. CRT callbacks controlled; no allocation failure or exception unwinding claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-runtime-vector.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_RUNTIME_VECTOR_PASS cases=3680 snapshots=%d controls=16 bodies=9'%report['snapshots'])
if __name__=='__main__':main()
