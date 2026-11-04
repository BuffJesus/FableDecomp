"""Near-match virtual entry with real best-fit/retry dispatch and retail behavior."""
import struct,subprocess,json,tempfile
from pathlib import Path
root=Path(__file__).resolve().parents[2]
import check_one,oracle_variable_virtual_allocate as oracle
from variable_allocator_parity import check
from gate_variable_allocate_existing import TARGETS
work=Path(tempfile.mkdtemp(prefix='variable-virtual-allocate-'));env=check_one.env();objects=[]
# Keep the first inlined attempt separate from the original out-of-line retry.
sources=['rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Allocate_00a659c0.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_AllocateMemoryFromExistingChunks_00a656e0.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp','rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableVirtualAllocate_test.cpp']
for path in sources:
    source=root/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
targets=dict(TARGETS,**{'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,
 '?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z':0xa65720,
 '?AllocateMemoryFromExistingChunks@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@1@K@Z':0xa656e0})
parity=check(objects[0],'?Allocate@CMemoryAllocatorVariableSize@@UAEPAXK@Z',0xa659c0,140,8,targets)
rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
for row in rows:
    data.extend(struct.pack('<9I',*row['args'],len(row['events'])));data.extend(row['initial'])
    for event in row['events']:data.extend(struct.pack('<6I',*event))
binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(len(objects)-1):
    r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0,r.stdout+r.stderr
assert f'VARIABLE_VIRTUAL_ALLOCATE_OK cases={len(rows)}'in r.stdout;print(r.stdout.strip())
(root/'rebuild/compile-gate/variable-virtual-allocate.json').write_text(json.dumps(dict(parity=parity,cases=len(rows),native_sources=len(objects)-1,omitted_provider_checks=len(objects)-1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='current alignment replaces stale globals; minimum signed eligible size; original request forwarded to NewChunk; retry only after successful NewChunk; split failure triggers NewChunk; data pointer uses current global header alignment; null failure result; full storage preserved by recorded boundaries',boundary='real best-fit and original strong retry dispatcher; split/NewChunk recorded, including deliberate global alignment mutation; two field loads differ in order, not an exact-byte claim'),indent=2)+'\n',encoding='utf-8')
print(f'VARIABLE_VIRTUAL_ALLOCATE_PASS grade={parity["grade"]} similarity={parity["opcode_similarity"]:.6f} cases={len(rows)} omitted={len(objects)-1}')
