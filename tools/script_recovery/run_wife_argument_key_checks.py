"""Compile the staged key scope against actual x86 FSE types; never install it."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars
from tools.script_recovery.prepare_wife_resource_extension import prepare


def run():
    runtime=Path('D:/Code/ForgeFSE-retail-shadow');source=Path(__file__).parent
    output=ROOT/'work/wife_argument_key_checks';output.mkdir(parents=True,exist_ok=True)
    (output/'result.json').write_text(json.dumps({'status':'running'})+'\n')
    prepare()
    proposal=ROOT/'work/wife_resource_integration'
    inputs=[source/'retail_wife_argument_key.h',source/'wife_argument_key_harness.cpp',
            source/'retail_wife_argument_key_lua.h',source/'wife_argument_key_lua_harness.cpp',
            *(runtime/'FableScriptExtender').glob('*.h')]
    lua=runtime/'Vendor/lua'
    lua_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    inputs.extend(lua_sources)
    inputs.extend([source/'wife_resource_methods_harness.cpp',
                   source/'runtime_checks/retail_resources_smoke.cpp',
                   *proposal.glob('*.h')])
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    hashes={str(p):digest(p) for p in inputs}
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    command=[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),
        '/I'+str(runtime/'Vendor/lua'),str(source/'wife_argument_key_harness.cpp'),'/Fe:argument-key.exe']
    records=[]
    lua_build=[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,lua_sources)]
    binding_build=command[:-2]+[str(source/'wife_argument_key_lua_harness.cpp'),
        *[str(output/(p.stem+'.obj')) for p in lua_sources],'/Fe:argument-key-lua.exe']
    resource_build=command[:6]+['/I'+str(proposal)]+command[6:-2]+[
        str(source/'wife_resource_methods_harness.cpp'),
        *[str(output/(p.stem+'.obj')) for p in lua_sources],'/Fe:wife-resource-methods.exe']
    for name,cmd in [('build',command),('test',[str(output/'argument-key.exe')]),
                     ('lua-build',lua_build),('binding-build',binding_build),
                     ('binding-test',[str(output/'argument-key-lua.exe')]),
                     ('resource-build',resource_build),
                     ('resource-test',[str(output/'wife-resource-methods.exe')])]:
        result=subprocess.run(cmd,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        records.append({'name':name,'exitCode':result.returncode})
        if result.returncode:
            (output/'result.json').write_text(json.dumps({'status':'failed','commands':records},indent=2)+'\n')
            raise RuntimeError(f'{name} exited {result.returncode}: {result.stdout}')
    executables={}
    for name in ('argument-key.exe','argument-key-lua.exe','wife-resource-methods.exe'):
        executable=(output/name).read_bytes();pe=int.from_bytes(executable[0x3C:0x40],'little')
        machine=int.from_bytes(executable[pe+4:pe+6],'little')
        if machine!=0x14C:raise ValueError('Argument key check requires x86: '+name)
        executables[name]={'machine':machine,'sha256':digest(output/name)}
    if any(digest(Path(p))!=h for p,h in hashes.items()):raise ValueError('Compiled inputs changed')
    report={'status':'passed','machine':machine,'inputs':hashes,'commands':records,'executables':executables,
            'limits':'Actual staged resource methods and real Lua with engine doubles; complete DLL/gameplay validation pending.'}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(run(),indent=2))
