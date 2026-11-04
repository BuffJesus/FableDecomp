"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import struct
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_defclass_partial_sort




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/STL_PartialSort_DefClassEntry_009b9030.cpp']
def main():
    retail=oracle_defclass_partial_sort.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/STL_DefClassPartialSort_test.cpp';obj=w/'enum.obj'
    fixture=test.read_text();table='static const unsigned retailKeys[82][40]={'+','.join('{'+','.join(str(v) for v in row['result'])+'}' for row in retail)+'};'
    fixture=fixture.replace('// RETAIL_EXPECTATIONS',table).replace('// RETAIL_COMPARE','for(unsigned j=0;j<n;++j)if(a[j].first!=retailKeys[n*2+mode][j])return 4;');test=w/'fixture.cpp';test.write_text(fixture)
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('??$__partial_sort'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFCLASS_PARTIAL_SORT_OK cases=82' in r.stdout,r.stdout
    patched=bytearray(b)
    for pos,(symbol,kind) in relocs.items():
        target=0x9b8820 if symbol.startswith('??$__make_heap') else 0x9b8770 if symbol.startswith('??$__pop_heap') else 0x9b8ca0 if symbol.startswith('??$sort_heap') else 0x99ec30 if symbol.startswith('??0CCharString') else 0xbfea0e if symbol=='_malloc' else 0xbfeae6 if symbol=='_memmove' else None
        assert target is not None and kind==20,(symbol,kind);struct.pack_into('<I',patched,pos,(target+struct.unpack_from('<i',b,pos)[0]-0x9b9030-pos-4)&0xffffffff)
    raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0x9b9030);assert len(b)==244 and bytes(patched)==raw[offset:offset+244]
    (ROOT/'rebuild/compile-gate/defclass-partial-sort.json').write_text(json.dumps(dict(address='009b9030',grade='RELOCATION_MATCH',compiled_bytes=len(b),retail_bytes=244,linked_cases=82,retail_cases=retail,mocked_boundaries='string storage; retail string/vector assignment for empty payloads'),indent=2)+'\n')
    print('DEFCLASS_PARTIAL_SORT_PASS cases=82 bytes='+str(len(b)))
if __name__=='__main__':main()
