"""Verify native compressed-definition offset sorting and its helper chain."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_compressed_offset_sort
ROOT=Path(__file__).resolve().parents[2]
UNITS=[('Sort','sort',0x9b85a0,115),('Introsort','__introsort_loop',0x9b7fa0,196),('InsertionSort','__insertion_sort',0x9b56f0,62),('UnguardedInsertionSort','__unguarded_insertion_sort_aux',0x9b2db0,73),('Partition','__unguarded_partition',0x9b3900,85),('PartialSort','__partial_sort',0x9b6be0,107),('LinearInsert','__linear_insert',0x9b46b0,99),('MakeHeap','__make_heap',0x9b3cf0,74),('AdjustHeap','__adjust_heap',0x9b3060,106),('SortHeap','sort_heap',0x9b5c10,96),('UnguardedLinearInsert','__unguarded_linear_insert',0x9b1b00,51),('PushHeap','__push_heap',0x9b1d80,79)]
NAMES=[f'00/9b/STL_{name}_CompressedOffset_{address:08x}.cpp'for name,_,address,_ in UNITS]

def compile_providers(work,env):
    raw=EXE.read_bytes();sections=pe_sections(raw);objects=[];reports=[]
    targets={function:address for _,function,address,_ in UNITS}
    for (name,function,address,size),source_name in zip(UNITS,NAMES):
        source=ROOT/'rebuild/src/compiled'/source_name;obj=work/(source.stem+'.obj')
        result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith('??$'+function+'@'))
        section,offset=coff.symbols[key];body,relocations=coff.sections[section];patched=bytearray(body)
        for position,(symbol,kind) in relocations.items():
            function_name=symbol.split('@',1)[0][3:];assert function_name in targets and kind==20,(symbol,kind)
            target=targets[function_name]
            struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-address-position-4)&0xffffffff)
        at=va_to_off(sections,address)
        assert len(body)==size and bytes(patched)==raw[at:at+size],(name,len(body),size)
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=size,grade='RELOCATION_MATCH',source=source_name))
    return objects,reports

def main():
    work=Path(tempfile.mkdtemp(prefix='compressed-offset-sort-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    objects,reports=compile_providers(work,env)
    retail=oracle_compressed_offset_sort.collect()
    fixture=(ROOT/'rebuild/tests/integration/CompressedOffsetSort_composed_test.cpp').read_text(encoding='utf-8')
    expected='static const unsigned long retail[84][128]={'+','.join('{'+','.join(str(v)for pair in row['result']for v in pair)+'}'for row in retail)+'};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',expected),encoding='utf-8');obj=work/'fixture.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    objects.append(obj);exe=work/'test.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    omitted_indices=[0,1,4,7,8,9]
    for omitted in omitted_indices:
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
        assert result.returncode!=0,('provider omitted',UNITS[omitted][0])
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True)
    assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'COMPRESSED_OFFSET_SORT_OK cases=84' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/compressed-offset-sort.json').write_text(json.dumps(dict(grade='RELOCATION_MATCH',providers=reports,linked_cases=84,retail_cases=84,omitted_provider_checks=len(omitted_indices),mocked_boundaries='none; retail sort and all callees execute directly',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('COMPRESSED_OFFSET_SORT_PASS cases=84 bytes='+str(sum(r['bytes']for r in reports))+' providers=12 omitted=6')

if __name__=='__main__':main()
