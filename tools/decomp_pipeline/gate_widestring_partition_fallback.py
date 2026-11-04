"""Retail parity and linked partition/partial-sort contracts."""
import json, struct, subprocess, tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
UNITS=(('STL_UnguardedPartition_WideString_009b3580.cpp','??$__unguarded_partition@PAVCWideString',0x009B3580,146,9),
       ('STL_PartialSort_WideString_009b6a40.cpp','??$__partial_sort@PAVCWideString',0x009B6A40,130,8))
TARGETS={'??MCWideString@@QBE_NABV0@@Z':0x0099AF70,'??0CWideString@@QAE@ABV0@@Z':0x0099B720,
         '??4CWideString@@QAEAAV0@ABV0@@Z':0x0099B7D0,'??1CWideString@@QAE@XZ':0x0099B510}
def main():
    work=Path(tempfile.mkdtemp(prefix='wide-partition-fallback-'));env=check_one.env()
    env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    sources=[ROOT/'rebuild/src/compiled/00/9b'/u[0] for u in UNITS]
    sources.append(ROOT/'rebuild/tests/00/9b/STL_WideStringPartitionFallback_009b3580_test.cpp')
    for source in sources:
        obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert r.returncode==0,r.stdout+r.stderr
        objects.append(obj)
    with pefile.PE(str(EXE)) as pe:
        for unit,obj in zip(UNITS,objects):
            leaf,prefix,address,size,count=unit;c=Coff(obj)
            symbol=next(s for s in c.symbols if s.startswith(prefix));section,offset=c.symbols[symbol]
            body,relocs=c.sections[section];assert offset==0 and len(body)==size and len(relocs)==count
            patched=bytearray(body)
            for pos,(target,kind) in relocs.items():
                assert kind==20
                if target.startswith('??$__make_heap@'):dest=0x009B3BB0
                elif target.startswith('??$__adjust_heap@'):dest=0x009B2EA0
                elif target.startswith('??$sort_heap@'):dest=0x009B5A80
                else:dest=TARGETS[target]
                value=dest+struct.unpack_from('<i',body,pos)[0]-address-pos-4
                struct.pack_into('<I',patched,pos,value&0xffffffff)
            assert patched==pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,size),leaf
            reports.append(dict(address=f'{address:08x}',bytes=size,relocations=count))
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'fixture.exe')]
    for omitted in (0,1):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omitted],env=env,capture_output=True,text=True)
        assert r.returncode!=0,'omitted provider linked'
    r=subprocess.run(link+[str(o) for o in objects],env=env,capture_output=True,text=True)
    assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(work/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and 'WIDE_PARTITION_FALLBACK_OK' in r.stdout,(r.returncode,r.stdout)
    (ROOT/'rebuild/compile-gate/widestring-partition-fallback.json').write_text(json.dumps(dict(grade='RELOCATION_MATCH',providers=reports,linked_cases=2,omitted_provider_checks=2),indent=2)+'\n')
    print('WIDE_PARTITION_FALLBACK_PASS bytes=276 cases=2 omitted=2')
if __name__=='__main__':main()
