"""Native boot.ini scan: retail code/constant match and full boundary traces."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_memory_initializer
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/9f/InitializeMemorySystem_009f1d20.cpp'
TARGETS={'__imp__CreateFileA@28':0x143fd48,'__imp__GetLastError@0':0x143fd58,'__imp__GetFileSize@8':0x143fdf0,'__imp__GlobalAlloc@8':0x143fdd4,'__imp__ReadFile@20':0x143fed0,'__imp__CloseHandle@4':0x143fe30,'__imp__GlobalFree@4':0x143fdf8,'_isspace':0xbfedbe}

def main():
    work=Path(tempfile.mkdtemp(prefix='memory-init-'));env=check_one.env();obj=work/'native.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);section,offset=coff.symbols['?InitializeMemorySystem@@YIXXZ'];body,relocs=coff.sections[section];assert offset==0;patched=bytearray(body);raw=EXE.read_bytes();sections=pe_sections(raw)
    for position,(name,kind)in relocs.items():
        if name.startswith('??_C@'):
            assert coff_data(coff,name,9)==b'boot.ini\0';at=va_to_off(sections,0x129c770);assert raw[at:at+9]==b'boot.ini\0';target=0x129c770
        else:assert name in TARGETS;target=TARGETS[name]
        assert kind in [6,20];value=target+struct.unpack_from('<I',body,position)[0]
        if kind==20:value-=0x9f1d20+position+4
        struct.pack_into('<I',patched,position,value&0xffffffff)
    at=va_to_off(sections,0x9f1d20);assert len(body)==347 and bytes(patched)==raw[at:at+347]
    retail=oracle_memory_initializer.collect();fixture=(ROOT/'rebuild/tests/integration/MemoryInitializer_test.cpp').read_text(encoding='utf-8');maximum=max(len(r['events'])for r in retail)
    table='static const unsigned long expected[16]['+str(maximum)+'][4]={'+','.join('{'+','.join('{'+','.join(str(v)+'UL'for v in event)+'}'for event in row['events'])+'}'for row in retail)+'};\n'
    table+='static const unsigned char inputs[16][256]={'+','.join('{'+(','.join(str(v)for v in bytes.fromhex(row['data']))or'0')+'}'for row in retail)+'};\n'
    for name,values in [('lengths',[len(bytes.fromhex(r['data']))for r in retail]),('shorts',[r['short']for r in retail]),('failures',[int(r['fail'])for r in retail]),('readOK',[int(r['read_ok'])for r in retail]),('eventCounts',[len(r['events'])for r in retail])]:table+='static const unsigned long '+name+'[16]={'+','.join(str(v)for v in values)+'};\n'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',table),encoding='utf-8');testobj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    # Redirect only this test copy's import slots; all original relocations above
    # were checked before patching. Production Win32/CRT boundaries remain real.
    blob=obj.read_bytes()
    for index,name in enumerate(list(TARGETS)[:-1]):
        old=name.encode()+b'\0';new=('_I'+str(index)).encode()+b'\0';assert blob.count(old)==1;blob=blob.replace(old,new+b'\0'*(len(old)-len(new)))
    assert blob.count(b'_isspace')==1;blob=blob.replace(b'_isspace',b'_Space\0\0');recorded=work/'recorded.obj';recorded.write_bytes(blob)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    result=subprocess.run(link+[str(testobj)],env=env,capture_output=True,text=True);assert result.returncode!=0,'omitted native scan linked'
    result=subprocess.run(link+[str(recorded),str(testobj)],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'MEMORY_INITIALIZER_OK cases=16'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/memory-initializer.json').write_text(json.dumps(dict(address='009f1d20',grade='RELOCATION_MATCH',bytes=347,string_bytes=9,linked_cases=16,retail_cases=16,omitted_provider_checks=1,mocked_boundaries='seven Win32 calls and signed-character isspace; full traces compared; no filesystem writes',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('MEMORY_INITIALIZER_PASS bytes=347 cases=16 omitted=1')

if __name__=='__main__':main()
