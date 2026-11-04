import sys,tempfile,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,oracle_installer_worker_loops as oracle
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='installer-worker-replay-'));env=check_one.env()
sources=['rebuild/src/compiled/00/9c/CFileInstaller_DecompressionThreadMain_009c4230.cpp','rebuild/src/compiled/00/9c/CFileInstaller_ThreadMain_009c5690.cpp','rebuild/src/compiled/00/a3/CCompressorZlib_HasStreamInput_00a3a7e0.cpp','rebuild/tests/integration/FileInstallerWorkerLoops_test.cpp']
objects=[]
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
targets={'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,'?Release@CCriticalSectionLock@@QAEXXZ':0xa62960,'??1CCriticalSectionLock@@QAE@XZ':0xa629a0,'?HasStreamInput@CCompressorZlib@@QBE_NXZ':0xa3a7e0,'?ProcessActiveFileForDecompression@CFileInstaller@@QAEXAAVCActiveFile@1@@Z':0x9c3c20,'__imp__WaitForSingleObjectEx@12':0x143fe08,'?Lock@CCriticalSectionLock@@QAEXXZ':0xa62980,'?PostFileReadCompletion@CFileInstaller@@QAEXPAVCActiveRead@1@@Z':0x9c4120,'__imp__SetEvent@4':0x143fde0,'?ProcessActiveFileForWrite@CFileInstaller@@QAEXAAVCActiveFile@1@@Z':0x9c3fb0,'?ProcessReads@CFileInstaller@@QAEXXZ':0x9c53a0,'?ProcessAlways@CFileInstaller@@QAEXXZ':0x9c4f20}
reports=[check(objects[0],'?DecompressionThreadMain@CFileInstaller@@QAEXXZ',0x9c4230,155,6,targets),check(objects[1],'?ThreadMain@CFileInstaller@@QAEXXZ',0x9c5690,469,22,targets),check(objects[2],'?HasStreamInput@CCompressorZlib@@QBE_NXZ',0xa3a7e0,11,0,{})]
print(json.dumps(reports,indent=2),flush=True)
for obj in objects[:2]:
    raw=obj.read_bytes()
    for old,new in [('__imp__WaitForSingleObjectEx@12','_WaitSlot'),('__imp__SetEvent@4','_SignalSlot')]:
        needle=old.encode()+b'\0';value=new.encode()+b'\0'
        if needle in raw:assert raw.count(needle)==1;raw=raw.replace(needle,value+b'\0'*(len(needle)-len(value)))
    obj.write_bytes(raw)
exe=w/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(3):
    r=subprocess.run(link+[str(o)for i,o in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
    assert r.returncode!=0
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert r.returncode==0,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    diff=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,'diff',diff,'record',diff//4112,'offset',diff%4112,'lengths',len(actual),len(expected)))
report.update(parity=reports,omitted_provider_checks=3)
(ROOT/'rebuild/compile-gate/installer-worker-loops.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full state snapshots',flush=True)

