import json,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
work=Path(tempfile.mkdtemp(prefix='installer-clear-shared-'));obj=work/'clear.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/FileInstallerClearBuffers_009c5110.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
report=check(obj,'?ClearBuffers@CFileInstaller@@QAEXXZ',0x9c5110,85,4,{'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,'?DeallocateMemory@CFileInstallerBufferArray@@QAEXXZ':0x9c76d0,'__imp__GlobalFree@4':0x143fdf8,'??1CCriticalSectionLock@@QAE@XZ':0xa629a0})
(ROOT/'rebuild/compile-gate/installer-cleanup-clear-buffers-shared-parity.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
