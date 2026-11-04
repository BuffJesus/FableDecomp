"""Typed parsed-item accessor ABI checked against retail machine code."""
import json,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SYMBOLS={'?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ':0x9b9870,'?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,'?GetInteger@CParsedItem@@QBEJXZ':0x9b9890,'?GetFloat@CParsedItem@@QBEMXZ':0x9b98a0}
def main():
    w=Path(tempfile.mkdtemp(prefix='parsed-accessors-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'a.obj';s=ROOT/'rebuild/src/compiled/00/9b/CParsedItem_Accessors_009b9870.cpp';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);raw=EXE.read_bytes();sections=pe_sections(raw);reports=[]
    for symbol,address in SYMBOLS.items():
        section,offset=c.symbols[symbol];body,relocs=c.sections[section];assert len(body)==4 and offset==0 and not relocs
        start=va_to_off(sections,address);assert body==raw[start:start+4];reports.append(dict(address=hex(address),symbol=symbol,bytes=4))
    (ROOT/'rebuild/compile-gate/parsed-item-accessors.json').write_text(json.dumps(dict(grade='MATCH',providers=reports),indent=2)+'\n');print('PARSED_ACCESSORS_PASS bytes=16 providers=4')
if __name__=='__main__':main()
