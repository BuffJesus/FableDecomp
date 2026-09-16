"""Compile the proposed Villager ambient methods with actual FSE types and Lua."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_talk_key import verify
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def run():
    native = verify(RData())
    runtime=Path('D:/Code/ForgeFSE-retail-shadow')
    source=Path(__file__).parent;output=ROOT/'work/villager_ambient_runtime_checks'
    output.mkdir(parents=True,exist_ok=True)
    implementation=source/'retail_villager_ambient.inc'
    body=implementation.read_text()
    lua=runtime/'Vendor/lua'
    lua_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    inputs=[implementation,source/'villager_ambient_runtime_harness.cpp',*lua_sources,
            *(runtime/'FableScriptExtender').glob('*.h')]
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    hashes={str(p):digest(p) for p in inputs}
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua_build=[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,lua_sources)]
    build=[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(source),
           '/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
           str(source/'villager_ambient_runtime_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in lua_sources],
           '/Fe:villager-ambient.exe']
    records=[]
    for name,command in [('lua-build',lua_build),('build',build),('test',[str(output/'villager-ambient.exe')])]:
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        records.append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
    exe=(output/'villager-ambient.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little')
    machine=int.from_bytes(exe[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Villager ambient check requires x86')
    if any(digest(Path(p))!=value for p,value in hashes.items()):raise ValueError('Villager ambient input changed during check')
    report={'status':'passed','native':native,'commands':records,'machine':machine,'inputs':hashes,
            'methodSha256':hashlib.sha256(body.encode()).hexdigest(),'executableSha256':digest(output/'villager-ambient.exe'),
            'limits':'Proposed method with actual FSE types and real Lua; engine calls are doubles. Complete resource-class registration, DLL and gameplay remain pending.'}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
