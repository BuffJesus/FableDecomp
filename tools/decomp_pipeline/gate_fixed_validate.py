"""Native allocator validation plus recorded ABI/call-order boundary replay."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_validate as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Validate_00a66260.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InternalValidateArea_009f1f40.cpp','rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-validate-'));env=check_one.env();objects=[]
    for path in SOURCES:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?InternalValidateArea@CMemoryAllocator@@QAEXPAX@Z':0x9f1f40}
    reports=[check(objects[0],'?Validate@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z',0xa66260,62,2,targets),check(objects[0],'?Validate@CMemoryAllocatorFixedSize@@UAEXXZ',0xa663a0,34,1,{'?Validate@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa66260}),check(objects[1],'?InternalValidateArea@CMemoryAllocator@@QAEXPAX@Z',0x9f1f40,3,0,targets)]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();totals=[]
    for recorded in [False,True]:
        fixture=work/('recorded.obj'if recorded else'native.obj');source=ROOT/'rebuild/tests/integration/FixedValidate_test.cpp'
        r=check_one.cl(['/nologo','/c','/O2','/MT']+(['/DFABLE_RECORD_VALIDATE']if recorded else[])+['/Fo'+str(fixture),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr
        providers=[p for i,p in enumerate(objects)if not(recorded and i==1)]
        selected=[row for row in rows if bool(row['args'][0])==recorded];data=bytearray(struct.pack('<I',len(selected)))
        for row in selected:
            data.extend(struct.pack('<5I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
            for *args,state in row['events']:data.extend(struct.pack('<2I',*args));data.extend(state)
        binary=work/('recorded.bin'if recorded else'native.bin');binary.write_bytes(data)
        exe=work/('recorded.exe'if recorded else'native.exe');link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
        for omitted in range(len(providers)):
            r=subprocess.run(link+[str(p)for i,p in enumerate(providers)if i!=omitted]+[str(fixture)],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',recorded,omitted)
        r=subprocess.run(link+[str(p)for p in providers]+[str(fixture)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
        r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_VALIDATE_OK cases={len(selected)}'in r.stdout,(recorded,r.returncode,r.stdout,r.stderr)
        totals.append(dict(recorded=recorded,cases=len(selected),omitted_provider_checks=len(providers)))
    (ROOT/'rebuild/compile-gate/fixed-validate.json').write_text(json.dumps(dict(providers=reports,modes=totals,cases=len(rows),native_calls=sum(row['native_calls']for row in rows),recorded_calls=sum(len(row['events'])for row in rows),compared_bytes_per_snapshot=oracle.WIDTH,invariants='empty/one/three/seven reversed chunks; empty/single ID254/sparse forward-backward byte-ID chains; chunk and all-chunk entry points; actual shipping internal validator; full storage preserved; recorded call this/data/order/prestate; callback terminates current area chain and skips next chunk, changes singleton; fresh area/chunk links and singleton after calls',boundary='native mode executes recovered InternalValidateArea; separate recorded mode replaces only that method to validate call order and reload behavior; full heap snapshots in both modes'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_VALIDATE_PASS bytes=96 cases={len(rows)} native_cases={totals[0]["cases"]} recorded_calls={sum(len(row["events"])for row in rows)} omitted=5')
if __name__=='__main__':main()
