"""Typed character write: exact retail body and copy-on-write delegation."""
import json,struct,tempfile,subprocess
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
 w=Path(tempfile.mkdtemp(prefix='char-write-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'write.obj';source=ROOT/'rebuild/src/compiled/00/99/CCharString_SetCharTyped_0099f390.cpp'
 r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);sec,off=c.symbols['?SetChar@CCharString@@QAEXJD@Z'];body,rels=c.sections[sec];patched=bytearray(body);assert len(body)==32 and off==0
 for pos,(target,kind) in rels.items():
  assert target=='?MakeOwnCopyOfData@CCharString@@AAEXXZ' and kind==20;struct.pack_into('<I',patched,pos,(0x99eaf0+struct.unpack_from('<i',body,pos)[0]-0x99f390-pos-4)&0xffffffff)
 raw=EXE.read_bytes();off=va_to_off(pe_sections(raw),0x99f390);assert bytes(patched)==raw[off:off+32]
 test=w/'test.cpp';test.write_text('\n'.join(['#include "fable_string.h"','#include <stdio.h>','#include <string.h>',
 'static int calls;static CCharStringData detached;',
 'void CCharString::MakeOwnCopyOfData(){++calls;if(storage_->owners>1){detached=*storage_;static char copied[16];strcpy(copied,storage_->text);detached.text=copied;detached.owners=1;--storage_->owners;storage_=&detached;}}',
 'int main(){CCharStringData* raw=0;CCharString* string=(CCharString*)&raw;string->SetChar(0,1);if(calls)return 1;CCharStringData data;char text[16]="abc";data.text=text;data.owners=1;raw=&data;string->SetChar(1,88);if(calls!=1||strcmp(text,"aXc"))return 2;data.owners=2;string->SetChar(0,90);if(calls!=2||strcmp(text,"aXc")||strcmp(raw->text,"ZXc")||data.owners!=1)return 3;puts("CHAR_SET_TYPED_OK cases=3");return 0;}']))
 r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(source),str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/char-set-typed.json').write_text(json.dumps(dict(address='0099f390',grade='RELOCATION_MATCH',bytes=32,linked_cases=3,mocked_boundary='MakeOwnCopyOfData'),indent=2)+'\n');print('CHAR_SET_TYPED_PASS bytes=32 cases=3')
if __name__=='__main__':main()
