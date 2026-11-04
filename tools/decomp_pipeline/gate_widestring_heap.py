"""Check wide-string heap providers against retail bytes and linked contracts."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
UNITS=[('STL_PushHeap_WideString_009b1c30.cpp','??$__push_heap@',0x9b1c30,110),('STL_AdjustHeap_WideString_009b2ea0.cpp','??$__adjust_heap@',0x9b2ea0,138),('STL_MakeHeap_WideString_009b3bb0.cpp','??$__make_heap@',0x9b3bb0,73),('STL_SortHeap_WideString_009b5a80.cpp','??$sort_heap@',0x9b5a80,112)]

def main():
    work=Path(tempfile.mkdtemp(prefix='wide-string-heap-'));env=check_one.env()
    env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for name,prefix,address,size in UNITS:
        source=ROOT/'rebuild/src/compiled/00/9b'/name;obj=work/(source.stem+'.obj')
        result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith(prefix));section,offset=coff.symbols[key]
        body,relocs=coff.sections[section];patched=bytearray(body)
        for position,(symbol,kind) in relocs.items():
            target=0x99af70 if symbol.startswith('??MCWideString') else 0x99b7d0 if symbol.startswith('??4CWideString') else 0x99b720 if symbol.startswith('??0CWideString') else 0x99b510 if symbol.startswith('??1CWideString') else 0x9b1c30 if symbol.startswith('??$__push_heap@') else 0x9b2ea0 if symbol.startswith('??$__adjust_heap@') else None
            assert target is not None and kind==20,(symbol,kind)
            struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-address-position-4)&0xffffffff)
        at=va_to_off(sections,address)
        assert len(body)==size and bytes(patched)==raw[at:at+size],(name,len(body),size)
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=size))
    test=ROOT/'rebuild/tests/00/9b/STL_WideStringHeap_009b1c30_test.cpp';obj=work/'test.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    objects.append(obj);exe=work/'test.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omitted in range(4):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
        assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'WIDESTRING_HEAP_OK cases=66' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/widestring-heap.json').write_text(json.dumps(dict(grade='RELOCATION_MATCH',providers=reports,linked_cases=66,omitted_provider_checks=4,mocked_boundaries='wide-string copy, assignment, comparison and destruction recorded as scalar-key boundaries'),indent=2)+'\n')
    print('WIDESTRING_HEAP_PASS cases=66 bytes=433 omitted=4')

if __name__=='__main__':main()
