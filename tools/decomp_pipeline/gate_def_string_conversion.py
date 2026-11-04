"""Typed CDefString conversion matches the retail table delegation."""
import json,struct,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
 w=Path(tempfile.mkdtemp(prefix='def-string-conversion-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'convert.obj';source=ROOT/'rebuild/src/compiled/00/41/CDefString_ToCharStringTyped_00415dd0.cpp'
 r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);sec,off=c.symbols['??BCDefString@@QAE?AVCCharString@@XZ'];body,rels=c.sections[sec];patched=bytearray(body);assert len(body)==25 and off==0
 targets={'?GDefStringTable@@3VCDefStringTable@@A':0x13ca828,'?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z':0x9d49b0}
 for pos,(target,kind) in rels.items():
  dest=targets[target];struct.pack_into('<I',patched,pos,(dest+struct.unpack_from('<i',body,pos)[0]-0x415dd0-pos-4 if kind==20 else dest)&0xffffffff)
 raw=EXE.read_bytes();off=va_to_off(pe_sections(raw),0x415dd0);assert bytes(patched)==raw[off:off+25]
 (ROOT/'rebuild/compile-gate/def-string-conversion.json').write_text(json.dumps(dict(address='00415dd0',grade='RELOCATION_MATCH',bytes=25,donor_signature='??BCDefString@@QAE?AVCCharString@@XZ'),indent=2)+'\n');print('DEF_STRING_CONVERSION_PASS bytes=25')
if __name__=='__main__':main()
