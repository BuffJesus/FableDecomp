"""Verify native save-data shrink and release bodies against retail."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/CArrayVector_Byte_DeallocateUnusedMemory_009b5db0.cpp','00/9b/CVectorMap_CompressedOffset_DeallocateUnusedMemory_009b7b10.cpp','00/9b/CArrayVector_SaveData_DeallocateMemory_009b4ae0.cpp']
PROVIDERS=[(0,'?DeallocateUnusedMemory@?$CArrayVector@E@@',0x9b5db0,129),(1,'?DeallocateUnusedMemory@?$CVectorMap@KK',0x9b7b10,135),(2,'?DeallocateMemory@?$CArrayVector@E@@',0x9b4ae0,68),(2,'?DeallocateMemory@?$CArrayVector@U?$pair@KK',0x9b5fb0,90)]

def main():
    work=Path(tempfile.mkdtemp(prefix='compressed-save-shrink-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    for name in NAMES:
        source=ROOT/'rebuild/src/compiled'/name;obj=work/(source.stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj)
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for index,prefix,address,size in PROVIDERS:
        coff=Coff(objects[index]);key=next(k for k in coff.symbols if k.startswith(prefix));section,offset=coff.symbols[key];body,relocs=coff.sections[section];patched=bytearray(body)
        for position,(symbol,kind) in relocs.items():
            target=0xbfea14 if symbol=='_free' else 0xbfeae6 if symbol=='_memmove' else (0x9b4e20 if index==0 else 0x9b4eb0) if symbol.startswith('?reserve@') else (0x9b3630 if index==0 else 0x9b54e0) if symbol.startswith('?_M_range_insert@') else (0x9b4ae0 if index==0 else 0x9b5fb0) if symbol.startswith('?DeallocateMemory@') else None
            assert target is not None and kind==20,(symbol,kind)
            struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-address-position-4)&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==size and bytes(patched)==raw[at:at+size],(prefix,len(body),size)
        reports.append(dict(address=f'{address:08x}',bytes=size,grade='RELOCATION_MATCH',source=NAMES[index]))
    test=ROOT/'rebuild/tests/integration/CompressedSaveShrink_test.cpp';obj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    objects.append(obj);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omitted in range(3):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',NAMES[omitted])
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'COMPRESSED_SAVE_SHRINK_OK cases=12' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/compressed-save-shrink.json').write_text(json.dumps(dict(grade='RELOCATION_MATCH',providers=reports,linked_cases=12,omitted_provider_checks=3,mocked_boundaries='none; native vector templates and CRT allocation/copy/free'),indent=2)+'\n',encoding='utf-8')
    print('COMPRESSED_SAVE_SHRINK_PASS cases=12 bytes=422 omitted=3')

if __name__=='__main__':main()
