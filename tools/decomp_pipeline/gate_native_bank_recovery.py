"""Exact recovery reader with bounded original execution and native disk lifetime."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, sys, subprocess
import check_one as V
import file_input_stream_sources as F
from pe_oracle import EXE
R = Path(__file__).resolve().parents[2]
SCRIPTS = [R/'tools/decomp_pipeline'/('native_bank_recovery_'+name+'.py') for name in ('build','verify','controls')]
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    inputs = {Path(__file__), EXE, *SCRIPTS, *F.SOURCES,
        R/'rebuild/src/compiled/00/9d/CBankFile_ReadRecoveryFile_009d0380.cpp',
        R/'rebuild/tests/integration/NativeBankRecovery_test.cpp',
        R/'rebuild/tests/integration/NativeBankRecovery_model.py',
        R/'rebuild/tests/integration/NativeBankRecoveryColdAliases.h'}
    inputs.update(R/'rebuild/src/compiled/00/99'/n for n in (
        'CDiskFileWin32_Read_0099a960.cpp','CDiskFileWin32_SetPosition_0099abf0.cpp',
        'CDiskFileWin32_GetPosition_0099abd0.cpp','CDiskFileWin32_Close_0099A920.cpp'))
    inputs.update(p for p in (R/'rebuild/include').rglob('*') if p.is_file())
    inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
    inputs.update(p for directory in (V.VC/'bin', V.VC/'lib') for p in directory.iterdir()
                  if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
    # Pin local evidence helpers, including imports loaded by child processes.
    inputs.update(R/'tools/decomp_pipeline'/n for n in (
        'check_one.py','file_input_stream_sources.py','gate_game_component_abi.py',
        'pe_oracle.py','native_exact_roots.py'))
    for module in tuple(sys.modules.values()):
        name = getattr(module,'__file__',None)
        if name:
            p=Path(name).resolve()
            if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'): inputs.add(p)
    pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
    b=R/'rebuild/build/native-bank-recovery/00/9d/009d0380'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    b.parent.mkdir(parents=True,exist_ok=True)
    b.with_suffix('.inputs.json').write_text(json.dumps(pins,indent=2)+'\n')
    for script in SCRIPTS:
        result=subprocess.run([sys.executable,str(script),str(b)],cwd=R,capture_output=True,text=True,timeout=120)
        (b/(script.stem+'.run.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2)+'\n')
        assert not result.returncode,result.stdout+result.stderr
        print(script.stem+' PASS',flush=True)
    report=json.loads((b/'verification.json').read_text())
    controls=json.loads((b/'controls-verification.json').read_text())
    run=json.loads((b/'run.json').read_text())
    assert 'cases=192 opens=96 allocations=96 frees=96' in run['stdout'] and run['returncode']==0
    assert report['model']['cases']==432 and len(report['model']['controls'])==4
    assert len(controls['controls'])==9 and len(report['omissions'])==13
    for p,h in pins.items(): assert sha(Path(p))==h,('input drift',p)
    for p,h in report['objects'].items(): assert sha(Path(p))==h
    assert sha(b/'probe.exe')==report['executable_sha256']
    for p in inputs:
        if p.suffix=='.py' and p.is_relative_to(R):
            q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
    report.update(status='PASS',grade='MATCH',build=str(b),host_cases=192,native_controls=controls['controls'],
        input_sha256=pins,object_sha256=report['objects'],
        boundary='679-byte original/final recovery reader. 432 bounded differential cases and192 native host cases over retained synthetic files; native stream plus disk Read/Seek/Position/Close. Path, existence, handle adoption, strings and entry-list decoding controlled. Normal cleanup only; exception unwinding untested. No game/save changes.')
    (b/'receipt.json').write_text(json.dumps(report,indent=2)+'\n')
    (R/'rebuild/compile-gate/native-bank-recovery.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_BANK_RECOVERY_PASS bytes=679 model=432 host=192 controls=13 grade=MATCH')

if __name__=='__main__': main()
