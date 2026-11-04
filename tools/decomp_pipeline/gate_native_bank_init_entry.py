"""Exact bank entry initialization and duplicate-filename container ABI."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import native_bank_init_entry_host as H
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankInitEntry_model as M
SOURCE=R/'rebuild/src/compiled/00/9c/CBankFile_InitEntry_009ce050.cpp';FIXTURE=R/'rebuild/tests/integration/NativeBankInitEntry_test.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inputs={Path(__file__),Path(M.__file__),Path(H.__file__),EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   if name!='aliases.h':closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE);closure(FIXTURE)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-init-entry/00/9c/009ce050'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root.cpp' if p==SOURCE else 'Fixture.cpp') if p in (SOURCE,FIXTURE) else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(inc),'/Fo'+str(b/'root.obj'),str(b/'root.cpp')],env,b);save('root.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 c=Coff(b/'root.obj');symbol=next(s for s in c.symbols if s.startswith('?InitEntry'));section,offset=c.symbols[symbol];body,rels=c.sections[section];assert not offset and len(body)==302 and len(rels)==13
 prefixes=[('??A?$',0x42d131),('?FableCRC',0x4014a0),('?_M_insert_overflow',0x9b6300),('?ConvertPathToFilename',0x9cbe10),('?GetAsLowerCase',0x99ed10),('??1CCharString',0x99eae0),('??0CCharString',0x99ec30),('?insert_equal',0x9d34e0),('?g_FableEmpty',0x129aaf4)]
 targets={name:next(a for prefix,a in prefixes if name.startswith(prefix)) for name,kind in rels.values()};fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(M.ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),M.ENTRY,302);assert bytes(fixed)==original
 save('inventory.json',dict(symbol=symbol,bytes=302,fixups=13,targets=targets,differences=[]))
 report=M.compare(b);assert report['cases']==1728 and not report['difference_offsets'];host,mapping,aliases=H.build_host(b)
 # The helper COMDAT must come from the controlled fixture, not the emitted
 # production overflow body. Verify its complete final bytes and relocations.
 helper=next(s for s in targets if s.startswith('?_M_insert_overflow'));fixture=Coff(b/'Fixture.obj');section,offset=fixture.symbols[helper];helper_body,helper_rels=fixture.sections[section];assert not offset;helper_fixed=bytearray(helper_body);helper_entry=mapping[helper]
 for site,(name,kind) in helper_rels.items():struct.pack_into('<I',helper_fixed,site,(mapping[name]+struct.unpack_from('<i',helper_body,site)[0]-(helper_entry+site+4 if kind==20 else 0))&0xffffffff)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);assert bytes(helper_fixed)==pe.get_data(helper_entry-pe.OPTIONAL_HEADER.ImageBase,len(helper_body))
 faults=[('symbol-index',0x22,b'\x90'*2),('symbol-flag',0x13,b'\x20'),('crc-flag',0x29,b'\x02'),('crc-dirty',0x8a,b'\x00'),('filename-flag',0x91,b'\x88'),('first-copy',0xd6,b'\x83\xc4\x04\x90\x90'),('temporary-destroy',0xc8,b'\x90'*5),('lowercase-destroy',0x120,b'\x90'*5)]
 controls=[];baseline=(b/'expected.bin').read_bytes();entry=mapping[symbol];start=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 for i,(name,offset,replacement) in enumerate(faults):
  altered_body=bytearray(original);altered_body[offset:offset+len(replacement)]=replacement;detected=False
  for args in itertools.product((0,64,128,256,448),(0,2),(0,2),(0,1),(1,),(0,1)):
   expected=M.execute(original,args)
   try:actual=M.execute(bytes(altered_body),args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);altered[start+offset:start+offset+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered);out=b/('control-%d.bin'%i)
  result=subprocess.run([str(exe),str(b/'input.bin'),str(out)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode in (0,80),(name,result.returncode);assert (result.returncode==80 and 'INIT_ENTRY_FAILURE' in result.stderr) or out.read_bytes()!=baseline,(name,'native neutral');controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'missing-root.exe'),str(b/'Fixture.obj'),'kernel32.lib'];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-root.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report.update(status='PASS',build=str(b),host=host,controls=controls,missing_root_rejected=True,fixture_overflow_audited=True,input_sha256=pins,object_sha256={str(p):sha(p) for p in (b/'root.obj',b/'Fixture.obj')},executable_sha256=sha(b/'probe.exe'),boundary='Exact302B/13fixups, full4KiB original/native state and ordered helper events. Multimap insert_equal returns4B iterator, duplicate-key helper remains controlled. CRC/path/string/map boundaries controlled; no exception unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-init-entry.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_INIT_ENTRY_PASS bytes=302 model=1728 host=1728 controls=16 grade=MATCH')
if __name__=='__main__':main()
