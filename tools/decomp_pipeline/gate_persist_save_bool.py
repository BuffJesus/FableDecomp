"""Text field save and bool adapter: exact bodies plus linked composition."""
import json,struct,tempfile,subprocess
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
 w=Path(tempfile.mkdtemp(prefix='persist-save-bool-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled/00/9b/CPersistContext_TransferObjectSaveText_009bad60.cpp',ROOT/'rebuild/src/compiled/00/40/CPersistContext_BoolTransferOut_00403f90.cpp'];reports=[];raw=EXE.read_bytes();sections=pe_sections(raw)
 for index,(address,prefix,size) in enumerate([(0x9bad60,'?TransferObjectSaveText',59),(0x403f90,'?TransferOut',38)]):
  obj=w/('body'+str(index)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[index])],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(obj);key=next(k for k in c.symbols if k.startswith(prefix));sec,off=c.symbols[key];body,rels=c.sections[sec];patched=bytearray(body);assert len(body)==size and off==0,(key,len(body))
  for pos,(target,kind) in rels.items():
   if target.startswith('??_C@'):
    sec,off=c.symbols[target];literal=c.sections[sec][0][off:].split(b'\0')[0];dest={b' ':0x1258c1c,b';\r\n':0x12353b8,b'TRUE':0x122d6d4,b'FALSE':0x122d6cc}[literal]
   else:assert target=='??YCCharString@@QAEABV0@PBD@Z';dest=0x99f100
   struct.pack_into('<I',patched,pos,(dest+struct.unpack_from('<i',body,pos)[0]-address-pos-4 if kind==20 else dest)&0xffffffff)
  off=va_to_off(sections,address);assert bytes(patched)==raw[off:off+size];reports.append(dict(address=hex(address),bytes=size,grade='RELOCATION_MATCH'))
 test=ROOT/'rebuild/tests/00/9b/CPersistContext_TransferObjectSaveText_009bad60_test.cpp';r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/persist-save-bool.json').write_text(json.dumps(dict(providers=reports,linked_cases=2,mocked_boundary='CCharString storage and append'),indent=2)+'\n');print('PERSIST_SAVE_BOOL_PASS bytes=97 cases=2')
if __name__=='__main__':main()
