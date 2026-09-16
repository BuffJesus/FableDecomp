"""Compile the native timer owner with actual FSE types; engine calls are doubles."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def run():
    source=Path(__file__).parent;runtime=Path('D:/Code/ForgeFSE-retail-shadow')
    out=ROOT/'work/oakvale_timers_runtime_checks';out.mkdir(parents=True,exist_ok=True)
    inputs=[source/'retail_oakvale_timers.h',source/'oakvale_timers_runtime_harness.cpp',*(runtime/'FableScriptExtender').glob('*.h')]
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    hashes={str(p):digest(p) for p in inputs}
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    build=[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
           '/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(runtime/'Vendor/lua'),
           str(source/'oakvale_timers_runtime_harness.cpp'),'/Fe:timers.exe']
    records=[]
    for name,command in [('build',build),('test',[str(out/'timers.exe')])]:
        result=subprocess.run(command,cwd=out,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (out/(name+'.log')).write_text(result.stdout);records.append(dict(name=name,exitCode=result.returncode))
        if result.returncode:raise RuntimeError(result.stdout)
    raw=(out/'timers.exe').read_bytes();pe=int.from_bytes(raw[0x3c:0x40],'little');machine=int.from_bytes(raw[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Timer owner requires x86')
    if any(digest(Path(path))!=sha for path,sha in hashes.items()):raise ValueError('Timer test input changed')
    report=dict(status='passed',commands=records,machine=machine,inputs=hashes,executableSha256=digest(out/'timers.exe'),
        testOutput=(out/'test.log').read_text().strip(),
        limits='Independent timer owner with actual FSE types and virtual engine doubles. Must integrate before quest Init and close before speech lists; scheduler and save/load remain unverified.')
    (out/'result.json').write_text(json.dumps(report,indent=2)+'\n');return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
