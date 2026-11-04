"""Exercise inline scoped-handle ownership with only pool Free controlled."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path
import check_one as V
R=Path(__file__).resolve().parents[2]


def main():
    source=R/'rebuild/tests/integration/ScopedMemoryHandle_test.cpp'
    header=R/'rebuild/include/fable_handle_memory_pool.h'
    inputs=(Path(__file__),source,header)
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='scoped-memory-handle-') as tmp:
        build=Path(tmp)
        env=V.env()
        exe=build/'fixture.exe'
        result=V.cl(['/nologo','/O2','/MT','/I'+str(R/'rebuild/include'),
                     '/Fe'+str(exe),str(source)],env,build)
        assert result.returncode==0,result.stdout+result.stderr
        result=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
        assert result.returncode==0 and 'SCOPED_MEMORY_HANDLE_PASS scenarios=36' in result.stdout,(result.returncode,result.stdout,result.stderr)
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',scenarios=36,input_sha256=pins,behavior=result.stdout.strip(),
                boundary='Free controlled; ownership operations use canonical inline CScopedMemoryHandle.',
                evidence='FableWin constructor 03363260 initializes Pool=0, Handle=-1; destructor 03369910 calls Clear; retail Initialise A039F0 confirms release/reset order.',
                coverage='Inline completeness only; no additional retail root claim.')
    (R/'rebuild/compile-gate/scoped-memory-handle.json').write_text(json.dumps(report,indent=2)+'\n')
    print('SCOPED_MEMORY_HANDLE_GATE_PASS scenarios=36')


if __name__=='__main__':main()
