"""Compile the current Barrel Man setup methods with the recovered Lua calls."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def run():
    runtime=Path('D:/Code/ForgeFSE-retail-shadow')
    source=Path(__file__).parent;output=ROOT/'work/barrel_man_setup_runtime_checks'
    output.mkdir(parents=True,exist_ok=True)
    implementation=runtime/'FableScriptExtender/LuaQuestState.cpp'
    text=implementation.read_text();bodies=[]
    for name in ('SetCreatureBrain','SetWanderCentrePoint','SetWanderMinDistance','SetWanderMaxDistance','SetScriptingStateGroup'):
        start=text.index('void LuaQuestState::'+name+'(');end=text.index('\n}\n',start)+3
        bodies.append(text[start:end])
    body='\n'.join(bodies)
    (output/'barrel_man_setup_runtime_bodies.inc').write_text(body)
    from tools.script_recovery.native_barrel_man_setup import LOWERED
    (output/'barrel_man_setup.lua').write_text(LOWERED)
    lua=runtime/'Vendor/lua'
    lua_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    inputs=[implementation,source/'barrel_man_setup_runtime_harness.cpp',source/'native_barrel_man_setup.py',*lua_sources,
            *(runtime/'FableScriptExtender').glob('*.h')]
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    hashes={str(p):digest(p) for p in inputs}
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua_build=[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,lua_sources)]
    build=[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2','/I'+str(output),
           '/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
           str(source/'barrel_man_setup_runtime_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in lua_sources],
           '/Fe:barrel-wander-check.exe']
    records=[]
    for name,command in [('lua-build',lua_build),('build',build),('test',[str(output/'barrel-wander-check.exe')])]:
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        records.append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
    exe=(output/'barrel-wander-check.exe').read_bytes();pe=int.from_bytes(exe[0x3c:0x40],'little')
    machine=int.from_bytes(exe[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Barrel setup check requires x86')
    if any(digest(Path(p))!=value for p,value in hashes.items()):raise ValueError('Barrel setup input changed during check')
    report={'status':'passed','commands':records,'machine':machine,'inputs':hashes,
            'methodSha256':hashlib.sha256(body.encode()).hexdigest(),'executableSha256':digest(output/'barrel-wander-check.exe'),
            'limits':'Five unchanged method bodies in a minimal host shell, actual FSE argument types and real Lua; engine/home-position doubles, not complete host registration or gameplay.'}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
