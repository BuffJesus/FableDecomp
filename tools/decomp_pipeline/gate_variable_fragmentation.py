"""Near-match fragmentation with native hardware and retail oracle validation."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_fragmentation as oracle
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_GetFragmentation_00a64590.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='variable-fragmentation-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Counts_00a64530.cpp','rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableFragmentation_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,'__real@00000000':0x122dedc,'__real@4f800000':0x122dcb4}
    parity=check(objects[0],'?GetFragmentation@CMemoryAllocatorVariableSize@@UAEMXZ',0xa64590,102,5,targets)
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for address,value in [(0x122dedc,0.0),(0x122dcb4,4294967296.0)]:assert struct.unpack_from('<f',raw,va_to_off(sections,address))[0]==value
    rows=oracle.collect();at=va_to_off(sections,0xa64590);data=bytearray(raw[at:at+102]);data.extend(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<9I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for kind,alignment,shift,state in row['events']:data.extend(struct.pack('<3I',kind,alignment,shift));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(objects)-1):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'VARIABLE_FRAGMENTATION_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-fragmentation.json').write_text(json.dumps(dict(parity=parity,cases=len(rows),native_count_cases=sum(row['args'][0]==1 for row in rows),omitted_provider_checks=3,compared_bytes_per_snapshot=oracle.WIDTH,invariants='both virtual count calls before guard; unsigned high-bit conversion; zero denominator; exact float32 bits versus retail hardware and emulator; native exception flags/control preservation under53/64-bit precision; callback owner/global mutation and fresh second vptr; real native counts included; complete pre/post states',boundary='focused count callbacks recorded; native-count mode executes recovered counts; native callback addresses normalized only after validating all four bindings; original102-byte body used only as bounded hardware oracle'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_FRAGMENTATION_PASS grade={parity["grade"]} similarity={parity["opcode_similarity"]:.6f} cases={len(rows)} native_counts={sum(row["args"][0]==1 for row in rows)} omitted=3')
if __name__=='__main__':main()
