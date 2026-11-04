"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_defclass_partition
import struct
from pe_oracle import EXE,pe_sections,va_to_off




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/STL_UnguardedInsertionSort_DefClassEntry_009b80c0.cpp','00/9b/STL_UnguardedPartition_DefClassEntry_009b8630.cpp']
def main():
    retail=oracle_defclass_partition.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/STL_DefClassSortHelpers_test.cpp';obj=w/'enum.obj'
    fixture=test.read_text();table='static const unsigned retailCuts[29]={'+','.join(str(row['cut']) for row in retail)+'};\nstatic const unsigned retailKeys[29][32]={'+','.join('{'+','.join(str(v) for v in row['result'])+'}' for row in retail)+'};'
    fixture=fixture.replace('// RETAIL_EXPECTATIONS',table).replace('// RETAIL_COMPARE','if((unsigned)(cut-a)!=retailCuts[n-4])return 6;for(unsigned j=0;j<n;++j)if(a[j].first!=retailKeys[n-4][j])return 7;');test=w/'fixture.cpp';test.write_text(fixture)
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('??$__unguarded_insertion_sort_aux'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFCLASS_SORT_HELPERS_OK cases=61' in r.stdout,r.stdout
    patched=bytearray(b)
    for pos,(symbol,kind) in relocs.items():
        target=0x99ec30 if symbol.startswith('??0CCharString') else 0xbfea0e if symbol=='_malloc' else 0xbfeae6 if symbol=='_memmove' else 0x9b76f0 if symbol.startswith('??$__unguarded_linear_insert') else None
        assert target is not None and kind==20,(symbol,kind);struct.pack_into('<I',patched,pos,(target+struct.unpack_from('<i',b,pos)[0]-0x9b80c0-pos-4)&0xffffffff)
    raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0x9b80c0);assert len(b)==171 and bytes(patched)==raw[offset:offset+171]
    (ROOT/'rebuild/compile-gate/defclass-sort-helpers.json').write_text(json.dumps(dict(insertion_grade='RELOCATION_MATCH',insertion_bytes=171,partition_grade='BEHAVIORAL',linked_cases=61,retail_partition_cases=retail,mocked_boundaries='string storage; retail swap helper preserves full payload'),indent=2)+'\n')
    print('DEFCLASS_SORT_HELPERS_PASS cases=61 bytes='+str(len(b)))
if __name__=='__main__':main()
